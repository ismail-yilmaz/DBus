#include "DBus.h"

namespace Upp {

namespace UPPDBUS {
bool trace = false;
bool beverbose = false;
}

#define LLOG(x)       do { if(UPPDBUS::trace) RLOG("DBusConnection: " << x); } while(false)
#define LDUMPHEX(x)	  do { if(UPPDBUS::beverbose) RDUMPHEX(x); } while(false)

namespace {

constexpr const dword DBUS_SIZECAP = 128 * (1024 * 1024); // 128 MiB, max len.

struct EventLock {
	bool& latch,  prev;
	EventLock(bool& l) : latch(l), prev(l) { latch = true; }
	~EventLock()                           { latch = prev; }
};

String GetDBusAuthExternalPayload()
{
	String id;

#ifdef PLATFORM_POSIX
	id = AsString(getuid());
#elif PLATFORM_WIN32
	HANDLE hToken = nullptr;

	if(OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
		DWORD dwLength = 0;
		GetTokenInformation(hToken, TokenUser, nullptr, 0, &dwLength);
		if(GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
			Buffer<byte> pTokenUser(dwLength);
			if(GetTokenInformation(hToken, TokenUser, ~pTokenUser, dwLength, &dwLength)) {
				TOKEN_USER* ptu = (TOKEN_USER*) ~pTokenUser;
				SID* sid = (SID*) ptu->User.Sid;

				if(IsValidSid(sid)) {
					// Format: S-Revision-Authority-Sub1-Sub2...
					id << "S-" << (int) sid->Revision << "-";

					int64 auth = 0;
					for(int i = 0; i < 6; i++)
						auth = (auth << 8) + sid->IdentifierAuthority.Value[i];
					id << auth;

					for(int i = 0; i < sid->SubAuthorityCount; i++)
						id << "-" << (int64) sid->SubAuthority[i];
				}
			}
		}
		CloseHandle(hToken);
	}
#endif

	return HexEncode(id);
}

#ifdef PLATFORM_POSIX

void ParseDBusInfo(String path, String& buspath, bool& abstract, bool systembus)
{
	abstract = false;
	if(IsNull(path)) {
		if(systembus) {
			buspath = "/var/run/dbus/system_bus_socket";
			abstract = false;
		}
		else {
			buspath = Format("/run/user/%d/bus", (int) getuid());
			abstract = false;
		}
		return;
	}

	DBusPathInfo info(path);
	if(info.method == "unix") {
		if(!IsNull(info["abstract"])) {
			buspath = info["abstract"];
			abstract = true;
		}
		else {
			buspath = info["path"];
			abstract = false;
		}
	}
};

#elif PLATFORM_WIN32

void ParseDBusInfo(String path, String& host, int& port, String& noncefile)
{
	LLOG("Path string: " << path);

	DBusPathInfo info(path);
	if(info.method == "tcp" || info.method == "nonce-tcp") {
		host = info["host"];
		port = StrInt(info["port"]);
		noncefile = info["noncefile"];
		return;
	}
	host = "localhost";
	port = 0;
	noncefile = Null;
}
#endif

}

const char* DBusConnection::GetErrorMsg(int code)
{
	static const Tuple<int, const char*> errors[] = {
		{ CONNECTION_FAILED, t_("Couldn't connect to D-Bus server") },
		{ DNS_FAILED, t_("DNS lookup failed") },
		{ AUTH_FAILED, t_("Authentication failed") },
		{ HELLO_FAILED, t_("Hello request failed") },
		{ CONNECTION_TIMED_OUT, t_("Connection timed out") },
		{ INVALID_PACKET, t_("Invalid packet received") },
		{ SOCKET_FAILURE, t_("Socket error occured") },
		{ ABORTED, t_("Operation was aborted") },
		{ EXCEPTION, t_("Unknown exception") }
	};
	const Tuple<int, const char*>* x = FindTuple(errors, __countof(errors), code);
	return x ? x->b : "-1";
}

DBusConnection::DBusConnection()
: packlen(0)
, extpacklen(0)
, status(IDLE)
, starttime(0)
, timeout(60000)
, waitstep(10)
, async(false)
, dispatching(false)
, serial(1)
, callserial(0)
#ifdef PLATFORM_WIN32
, port(0)
#endif
{
}

DBusConnection::~DBusConnection()
{
	Disconnect();
}

bool DBusConnection::Init()
{
	socket.Timeout(0);
	packet.Clear();
	packlen = 0;
	extpacket.Clear();
	extpacklen = 0;
	status = WORKING;
	starttime = msecs();
	dispatching = false;
#ifdef PLATFORM_WIN32
	ipinfo.Start(buspath, port);
	LLOG("Connection to " << buspath << ":" << port);
#endif
	return true;
}

#ifdef PLATFORM_WIN32

bool DBusConnection::Dns()
{
	if(ipinfo.InProgress())
		return false;
	if(ipinfo.GetResult())
		return true;
	ThrowError(DNS_FAILED);
	return true;
}

bool DBusConnection::TcpConnect()
{
	if(socket.Connect(ipinfo)) {
		LLOG("Successfully connected to D-Bus (tcp) at " << buspath << ":" << port);
		ipinfo.Clear();
		return true;
	}
	return false;
}

#elif PLATFORM_POSIX

bool DBusConnection::FsyConnect()
{
	if(socket.ConnectFileSystem(buspath)) {
		LLOG("Successfully connected to D-Bus at " << buspath);
		return true;
	}
	return false;
}

bool DBusConnection::AbsConnect()
{
	if(socket.ConnectAbstract(buspath)) {
		LLOG("Successfully connected to D-Bus (abstract) at " << buspath);
		return true;
	}
	return false;
}

#endif

bool DBusConnection::Get()
{
	while(!IsTimeout()) {
		if(IsEof())
			return true;
		int c = socket.Get();
		if(c < 0)
			break;
		packet.Cat(c);
		Check(packet);
		starttime = msecs();
	}
	return false;
}

bool DBusConnection::Put()
{
	while(!IsTimeout()) {
		int n = packet.GetLength() - packlen;
		n = socket.Put(~packet + packlen, n);
		if(n == 0)
			break;
		packlen += n;
		if(packlen == packet.GetLength()) {
			packet.Clear();
			packlen = 0;
			return true;
		}
		starttime = msecs();
	}
	return false;
}

bool DBusConnection::Drain()
{
	// Server "Put"

	if(!extpacket.GetCount()) {
		extpacklen = 0;
		return false;
	}
	while(!IsTimeout()) {
		int n = extpacket.GetLength() - extpacklen;
		if(n <= 0) {
			extpacket.Clear();
			extpacklen = 0;
			break;
		}
		n = socket.Put(~extpacket + extpacklen, n);
		if(n <= 0)
			break;
		extpacklen += n;
		if(extpacklen == extpacket.GetLength()) {
			extpacket.Clear();
			extpacklen = 0;
			break;
		}
		starttime = msecs();
	}
	return extpacket.GetCount() > 0;;
}

void DBusConnection::PutGet()
{
	queue.AddTail() = [this] { return Put(); };
	queue.AddTail() = [this] { return Get(); };
}

void DBusConnection::Check(const String& s)
{
	if(s.GetLength() > DBUS_SIZECAP)
		ThrowError(INVALID_PACKET);
}

void DBusConnection::Check()
{
	if(status != WORKING)
		return;
	if(IsTimeout())
		ThrowError(CONNECTION_TIMED_OUT);
	if(socket.IsError())
		ThrowError(SOCKET_FAILURE);
	if(socket.IsAbort())
		ThrowError(ABORTED);
}

bool DBusConnection::Connect(const String& path, bool abstract)
{
	if(InProgress()) {
		LLOG("Aborting. Connect attempted while a previous operation is still in progress.");
		return false;
	}

	buspath = path;
	queue.Clear();
	IsEof = [this] { return AuthIsEof(); };
	queue.AddTail([this] { return Init(); });
#ifdef PLATFORM_POSIX
	if(abstract)
		queue.AddTail([this] { return AbsConnect(); });
	else
		queue.AddTail([this] { return FsyConnect(); });
#elif PLATFORM_WIN32
	queue.AddTail([this] { return Dns(); });
	queue.AddTail([this] { return TcpConnect(); });
#endif
	queue.AddTail([this] { return AuthRequest(); });
	return Run();
}

bool DBusConnection::ConnectSession()
{
	String path, var = GetEnv("DBUS_SESSION_BUS_ADDRESS");
	bool abstract = false;
#ifdef PLATFORM_POSIX
	ParseDBusInfo(var, path, abstract, false);
#elif PLATFORM_WIN32
	ParseDBusInfo(var, path, port, noncefile);
#endif
	return Connect(path, abstract);
}

bool DBusConnection::ConnectSystem()
{
	String path, var = GetEnv("DBUS_SYSTEM_BUS_ADDRESS");
	bool abstract = false;
#ifdef PLATFORM_POSIX
	ParseDBusInfo(var, path, abstract, true);
#elif PLATFORM_WIN32
	ParseDBusInfo(var, path, port, noncefile);
#endif
	return Connect(path, abstract);
}

void DBusConnection::Disconnect()
{
	LLOG("Disconnecting...");
	if(socket.IsOpen())
		socket.Close();
	status = IDLE;
	packet.Clear();
	packlen = 0;
	extpacket.Clear();
	extpacklen = 0;
	queue.Clear();
}

bool DBusConnection::AuthRequest()
{
	LLOG("Starting authentication...");
	packet.Clear();
#ifdef PLATFORM_WIN32
	if(!IsNull(noncefile)) {
		if(String nonce = LoadFile(noncefile); nonce.GetCount() == 16) {
			packet << nonce;
			LLOG("Injected 16-byte TCP nonce.");
		}
		else
			LLOG("Warning: Failed to read 16-byte nonce from " << noncefile);
	}
#endif
	packet << '\0';
	String payload = GetDBusAuthExternalPayload();
	packet << (IsNull(payload) ? "AUTH ANONYMOUS\r\n" : "AUTH EXTERNAL " << payload << "\r\n");
	packlen = 0;
	LLOG(">> AUTH: Sending request.");
	LDUMPHEX(packet);
	PutGet();
	return true;
}

bool DBusConnection::AuthIsEof()
{
	if(packet.GetCount() > 1) {
		const char* c = packet.Last();
		if(c[-1] == '\r' && c[0] == '\n') {
			return AuthParse();
		}
	}
	return false;
}

bool DBusConnection::AuthParse()
{
	LLOG("<< AUTH: Reply received.");
	LDUMPHEX(packet);

	if(packet.StartsWith("OK")) {
		packet = "BEGIN\r\n";
		packlen = 0;
		IsEof = [this] { return HelloIsEof(); };
		queue.AddTail([this] { return Put(); });
		queue.AddTail([this] { return HelloRequest(); });
		return true;
	}

	ThrowError(AUTH_FAILED);
	return false;
}

bool DBusConnection::HelloRequest()
{
	LLOG("Starting Hello request...");
	packet = ~DBusMessage::CreateMethodCall(
		serial++,
		StdDBusName,
		StdDBusPath,
		StdDBusInterface,
		"Hello"
	);
	packlen = 0;
	LLOG(">> HELLO: Sending request.");
	LDUMPHEX(packet);
	PutGet();
	return true;
}

bool DBusConnection::HelloIsEof()
{
	while(MsgIsEof()) {
		if(HelloParse())
			return true;
	}
	return false;
}

bool DBusConnection::HelloParse()
{
	DBusMessage msg = ExtractMessage();
	LLOG("<< HELLO: Reply or signal received.");
	LDUMPHEX(~msg);

	if(msg.IsSignal()) {
		DispatchSignal(msg);
		return false;
	}
	else
	if(msg.IsMethodCall()) {
		WhenMethodCall(msg);
		return false;
	}
	else
	if(msg.IsMethodReturn()) {
		uniquename = msg.ParseString();
		LLOG("Hello successful, unique name: " << uniquename);
		RestoreMatches();
		return true;
	}

	ThrowError(HELLO_FAILED);
	return false;
}

bool DBusConnection::GetMessageLength(int& tot)
{
	if(packet.GetCount() < 16)
		return false;

	BParser bp(packet);
	bp.BigEndian(bp.PeekByte() == 'B');

	bp.Skip(4);
	dword body = bp.ReadDword();
	bp.Skip(4);
	dword flds = bp.ReadDword();

	if(flds > DBUS_SIZECAP || body > DBUS_SIZECAP)
		ThrowError(INVALID_PACKET);

	int64 hdr = 16 + (int64) flds;
	int64 pad = (hdr % 8 != 0) ? 8 - (hdr % 8) : 0;
	int64 total = hdr + pad + (int64) body;

	if(total > DBUS_SIZECAP)
		ThrowError(INVALID_PACKET);

	tot = (int) total;

	return true;
}

bool DBusConnection::MsgIsEof()
{
	int total;
	if(!GetMessageLength(total))
		return false;
	return packet.GetCount() >= total;
}

DBusMessage DBusConnection::ExtractMessage()
{
	int total;
	if(!GetMessageLength(total))
		return Null;
	String res = packet.Left(total);
	packet.Remove(0, total);
	return DBusMessage(res);
}

bool DBusConnection::MethodCall(const String& dest, const String& path,	const String& iface,
												const String& method, const DBusValueArray& args)
{
	ASSERT(socket.IsOpen());

	if(dispatching) {
		LLOG("Aborting. Synchronous MethodCall attempted inside an active IPC callback.");
		return false;
	}

	if(InProgress()) {
		LLOG("Aborting. MethodCall attempted while a previous operation is still in progress.");
		return false;
	}

	replymsg = Null;
	callserial = serial++;

	DBusMessage msg = DBusMessage::CreateMethodCall(
		callserial,
		dest,
		path,
		iface,
		method,
		args
	);
	if(IsNull(msg))
		return false;

	packet = ~msg;
	packlen = 0;
	queue.Clear();
	IsEof = [this] { return MethodIsEof(); };
	queue.AddTail([this] { return InitCall(); });
	queue.AddTail([this] { return MethodRequest(); });
	return Run();
}

bool DBusConnection::BusMethodCall(const String& method, const DBusValueArray& args)
{
	return MethodCall(StdDBusName, StdDBusPath, StdDBusInterface, method, args);
}

bool DBusConnection::InitCall()
{
	LLOG("Starting method call...");
	Touch();
	return true;
}

bool DBusConnection::MethodRequest()
{
	LLOG(">> METHOD: Sending request.");
	LDUMPHEX(packet);
	PutGet();
	return true;
}

void DBusConnection::DispatchSignal(const DBusMessage& msg)
{
	EventLock __(dispatching);
	bool handled = false;
	for(const auto& sm : signalmatches) {
		if(msg.MatchRule(sm.rule)) {
			sm.cb(msg);
			handled = true;
		}
	}
	if(!handled)
		WhenSignal(msg);
}

void DBusConnection::DispatchMethodCall(const DBusMessage& msg)
{
	EventLock __(dispatching);
	WhenMethodCall(msg);
}

bool DBusConnection::MethodIsEof()
{
	while(MsgIsEof()) {
		LLOG("<< METHOD: Reply or signal received.");
		DBusMessage msg = ExtractMessage();
		if(msg.IsSignal()) {
			DispatchSignal(msg);
		}
		else
		if(msg.IsMethodCall()) {
			DispatchMethodCall(msg);
		}
		else
		if(msg.IsMethodReturn() || msg.IsError()) {
			if(dword rep = msg.ParseFields().reply; rep == callserial) {
				replymsg = msg;
				return true;
			}
		}
	}
	return false;
}

void DBusConnection::Listen()
{
	if(InProgress()) {
		LLOG("Aborting. Listen attempted while a previous operation is still in progress.");
		return;
	}

	queue.Clear();
	IsEof = [this] { return ListenIsEof(); };
	queue.AddTail([this] { return InitCall(); });
	queue.AddTail([this] { return Get(); });
	Run();
}

bool DBusConnection::ListenIsEof()
{
	while(MsgIsEof()) {
		DBusMessage msg = ExtractMessage();
		if(msg.IsSignal()) {
			DispatchSignal(msg);
		}
		else
		if(msg.IsMethodCall()) {
			DispatchMethodCall(msg);
		}
	}
	return false;
}

bool DBusConnection::Do0()
{
	try {
		Check();
		Drain();
		if(!queue.IsEmpty() && queue.Head()()) {
			queue.DropHead();
			starttime = msecs();
		}
		bool pending = Drain();
		if(queue.IsEmpty() && !pending) {
			LLOG("DBus operation successful.");
			status = FINISHED;
		}
		else
			WhenDo();
	}
	catch(const DBusError& e) {
		LLOG("Failed: " << e);
		SetError(e.code, e);
	}
	catch(...) {
		LLOG("Unknown exception.");
		SetError(EXCEPTION, GetErrorMsg(-1));
	}

	return status == WORKING;
}

bool DBusConnection::Run()
{
	if(async)
		return true;

	SocketWaitEvent we;
	AddTo(we);
	while(Do0())
		we.Wait(waitstep);

	return !IsError();
}

void DBusConnection::SetError(int code, const String& reason)
{
	status = FAILED;
	queue.Clear();
	packet.Clear();
	packlen = 0;
	extpacket.Clear();
	extpacklen = 0;
	socket.ClearAbort();
	error = MakeTuple<int, String>(code, reason);
}

bool DBusConnection::AddMatch(const String& rule, Event<const DBusMessage&> cb)
{
	if(cb) {
		SignalMatch& sm = signalmatches.Add();
		sm.rule = rule;
		sm.cb = cb;
	}

	if(!socket.IsOpen())
		return true;

	if(dispatching) {
		DBusMessage msg = DBusMessage::CreateMethodCall(
			serial++,
			StdDBusName,
			StdDBusPath,
			StdDBusInterface,
			"AddMatch", { rule }
		);
		if(IsNull(msg))
			return false;

		extpacket.Cat(~msg);
		Touch();
		return true;
	}
	else
		return MethodCall(StdDBusName, StdDBusPath, StdDBusInterface, "AddMatch", { rule });
}

bool DBusConnection::RemoveMatch(const String& rule)
{
	signalmatches.RemoveIf([&](int i) { return signalmatches[i].rule == rule; });

	if(!socket.IsOpen())
		return true;

	if(dispatching) {
		DBusMessage msg = DBusMessage::CreateMethodCall(
			serial++,
			StdDBusName,
			StdDBusPath,
			StdDBusInterface,
			"RemoveMatch", { rule }
		);
		if(IsNull(msg))
			return false;

		extpacket.Cat(~msg);
		Touch();
		return true;
	}
	else
		return MethodCall(StdDBusName, StdDBusPath, StdDBusInterface, "RemoveMatch", { rule });
}

void DBusConnection::RestoreMatches()
{
	if(signalmatches.IsEmpty())
		return;

	for(const SignalMatch& sm : signalmatches)
		extpacket << ~DBusMessage::CreateMethodCall(
			serial++,
			StdDBusName,
			StdDBusPath,
			StdDBusInterface,
			"AddMatch", { sm.rule }
		);

	LLOG("Sent server-side match rules: " << signalmatches.GetCount());
	Touch();
}

bool DBusConnection::FetchProperty(const String& dest, const String& path, const String& iface,
																			const String& prop)
{
	return MethodCall(dest, path, StdDBusProperties, "Get", { iface, prop });
}

bool DBusConnection::BroadcastSignal(const String& path, const String& iface, const String& name,
																	const DBusValueArray& args)
{
	ASSERT(socket.IsOpen());

	DBusMessage msg = DBusMessage::CreateSignal(
		serial++,
		path,
		iface,
		name,
		args
	);
	if(IsNull(msg))
		return false;

	extpacket.Cat(~msg);
	Touch();
	return dispatching ? true : Run();
}

bool DBusConnection::RequestName(const String& name)
{
	return MethodCall(
		StdDBusName,
		StdDBusPath,
		StdDBusInterface,
		"RequestName",
		{ name, (uint32) 3 }
	);
}

void DBusConnection::SendReply(const DBusMessage& req, const DBusValueArray& args)
{
	ASSERT(socket.IsOpen());

	DBusMessage::FieldData fd = req.ParseFields();
	DBusMessage msg = DBusMessage::CreateMethodReturn(
		serial++,
		req.GetSerial(),
		fd.sender,
		args
	);
	if(IsNull(msg))
		return;

	extpacket.Cat(~msg);
	Touch();
}

void DBusConnection::SendError(const DBusMessage& req, const String& errname, const String& errmsg)
{
	ASSERT(socket.IsOpen());

	DBusMessage::FieldData fd = req.ParseFields();
	DBusMessage msg =  DBusMessage::CreateError(
		serial++,
		req.GetSerial(),
		fd.sender,
		errname,
		errmsg
	);
	if(IsNull(msg))
		return;

	extpacket.Cat(~msg);
	Touch();

}

}