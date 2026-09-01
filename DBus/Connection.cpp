#include "DBus.h"

#define LLOG(x)      //   RLOG("D-Bus: " << x)
#define LDUMPHEX(x)  //   RDUMPHEX(x)

namespace Upp {

namespace {
	
struct EventLock {
	bool& latch,  prev;
	EventLock(bool& l) : latch(l), prev(l) { latch = true; }
	~EventLock()                           { latch = prev; }
};

String ParseDBusAddress(String path, bool& abstract)
{
	abstract = false;

	if(path.IsEmpty())
		return path;

	int pos = path.FindAfter("unix:abstract=");
	if(pos >= 0)
		abstract = true;
	else
		pos = path.FindAfter("unix:path=");

	if(pos >= 0) {
		path = path.Mid(pos);
		if(int q = path.Find(','); q >= 0)
			path = path.Left(q);
	}

	return path;
}
}

const char* DBusConnection::GetMsg(int code)
{
	static const Tuple<int, const char*> errors[] = {
		{ CONNECTION_FAILED, t_("Couldn't connect to D-Bus server") },
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
	return true;
}

bool DBusConnection::FsConnect()
{
	if(socket.ConnectFileSystem(buspath)) {
		LLOG("Successfully connected to D-Bus at " << buspath);
		return true;
	}
	else
		SetError(CONNECTION_FAILED);
	return false;
}

bool DBusConnection::AsConnect()
{
	if(socket.ConnectAbstract(buspath)) {
		LLOG("Successfully connected to D-Bus (abstract) at " << buspath);
		return true;
	}
	else
		SetError(CONNECTION_FAILED);
	return false;
}

bool DBusConnection::Get()
{
	while(!IsTimeout()) {
		if(IsEof())
			return true;
		int c = socket.Get();
		if(c < 0)
			break;
		packet.Cat(c);
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

void DBusConnection::Check()
{
	if(status != WORKING)
		return;
	if(IsTimeout())
		SetError(CONNECTION_TIMED_OUT);
	if(socket.IsError())
		throw Error("Socket failure. " + socket.GetErrorDesc());
	if(socket.IsAbort())
		SetError(ABORTED);
}

bool DBusConnection::Connect(const String& path, bool abstract)
{
	buspath = path;
	queue.Clear();
	IsEof = [this] { return AuthIsEof(); };
	queue.AddTail([this] { return Init(); });

	if(abstract)
		queue.AddTail([this] { return AsConnect(); });
	else
		queue.AddTail([this] { return FsConnect(); });

	queue.AddTail([this] { return AuthRequest(); });
	return Run();
}

bool DBusConnection::ConnectSession()
{
	bool abstract;
	String path = ParseDBusAddress(GetEnv("DBUS_SESSION_BUS_ADDRESS"), abstract);

	if(path.IsEmpty()) {
		path = Format("/run/user/%d/bus", (int) getuid());
		abstract = false; // System defaults are standard file sockets
	}

	return Connect(path, abstract);
}

bool DBusConnection::ConnectSystem()
{
	bool abstract;
	String path = ParseDBusAddress(GetEnv("DBUS_SYSTEM_BUS_ADDRESS"), abstract);

	if(path.IsEmpty()) {
		path = "/var/run/dbus/system_bus_socket";
		abstract = false;
	}

	return Connect(path, abstract);
}

void DBusConnection::Disconnect()
{
	LLOG("Disconnecting...");
	if(socket.IsOpen())
		socket.Close();
	status = IDLE;
	packet.Clear();
}

bool DBusConnection::AuthRequest()
{
	LLOG("Starting authentication...");
	uid_t uid = getuid();
	String uidstr = FormatInt(uid);
	String uidhex;
	for(int i = 0; i < uidstr.GetLength(); i++) {
		uidhex << Format("%02x", (byte) uidstr[i]);
	}

	packet.Clear();
	packet.Cat('\0');
	packet << "AUTH EXTERNAL " << uidhex << "\r\n";
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
	if(packet.Find("OK") >= 0) {
		packet = "BEGIN\r\n";
		packlen = 0;
		IsEof = [this] { return HelloIsEof(); };
		queue.AddTail([this] { return Put(); });
		queue.AddTail([this] { return HelloRequest(); });
		return true;
	}
	SetError(AUTH_FAILED);
	return false;
}

bool DBusConnection::HelloRequest()
{
	LLOG("Starting Hello request...");
	packet = DBusMessage::CreateMethodCall(serial++,
								"org.freedesktop.DBus",
								"/org/freedesktop/DBus",
								"org.freedesktop.DBus",
								"Hello").GetRawData();
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
	LDUMPHEX(msg.GetRawData());

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
		return true;
	}

	SetError(HELLO_FAILED);
	return false;
}

bool DBusConnection::MsgIsEof()
{
	if(packet.GetCount() >= 16) {
		BParser bp(packet);
		bp.BigEndian(bp.PeekByte() == 'B');
		bp.Skip(4);
		dword body = bp.ReadDword();
		bp.Skip(4);
		dword flds = bp.ReadDword();

		int hdr = 16 + flds;
		int pad = (hdr % 8 != 0) ? 8 - (hdr % 8) : 0;
		int tot = hdr + pad + body;

		return packet.GetCount() >= tot;
	}
	return false;
}

DBusMessage DBusConnection::ExtractMessage()
{
	BParser bp(packet);
	bp.BigEndian(bp.PeekByte() == 'B');
	bp.Skip(4);
	dword body = bp.ReadDword();
	bp.Skip(4);
	dword flds = bp.ReadDword();

	int hdr = 16 + flds;
	int pad = (hdr % 8 != 0) ? 8 - (hdr % 8) : 0;
	int tot = hdr + pad + body;

	String res = packet.Left(tot);
	packet.Remove(0, tot);
	return DBusMessage(res);
}

bool DBusConnection::MethodCall(const String& dest, const String& path,
								const String& iface, const String& method,
								const DBusValueArray& args)
{
	ASSERT(socket.IsOpen());
	ASSERT_(!dispatching, "FATAL: Reentrant synchronous D-Bus call detected inside a callback!");
	
	if(dispatching) {
		RLOG("!!!Aborting!!! Synchronous MethodCall attempted inside an active IPC callback.");
		return false;
	}
	
	replymsg = DBusMessage();
	callserial = serial++;
	packet = DBusMessage::CreateMethodCall(callserial, dest, path, iface, method, args).GetRawData();
	packlen = 0;
	queue.Clear();
	IsEof = [this] { return MethodIsEof(); };
	queue.AddTail([this] { return InitCall(); });
	queue.AddTail([this] { return MethodRequest(); });
	return Run();
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
			EventLock __(dispatching);
			WhenMethodCall(msg);
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
			EventLock __(dispatching);
			WhenMethodCall(msg);
		}
	}
	return false;
}

bool DBusConnection::Do0()
{
	try {
		Check();
		bool pending = Drain();
		if(!queue.IsEmpty() && queue.Head()()) {
			queue.DropHead();
			starttime = msecs();
		}
		if(queue.IsEmpty() && !pending) {
			LLOG("DBus operation successful.");
			status = FINISHED;
		}
		else
			WhenDo();
	}
	catch(const Error& e) {
		LLOG("Failed: " << e);
		status = FAILED;
		queue.Clear();
		extpacket.Clear();
		extpacklen = 0;
		socket.ClearAbort();
		error = MakeTuple<int, String>(e.code, e);
	}
	catch(...) {
		LLOG("Unknown exception.");
		status = FAILED;
		queue.Clear();
		extpacket.Clear();
		extpacklen = 0;
		socket.ClearAbort();
		error = MakeTuple<int, String>(EXCEPTION, GetMsg(-1));
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

bool DBusConnection::AddMatch(const String& rule, Event<const DBusMessage&> cb)
{
	if(cb) {
		SignalMatch& sm = signalmatches.Add();
		sm.rule = rule;
		sm.cb = cb;
	}
	
	if(!socket.IsOpen())
		return true;
	
	return MethodCall("org.freedesktop.DBus", "/org/freedesktop/DBus",
					"org.freedesktop.DBus", "AddMatch", DBusValueArray{ rule });
}

bool DBusConnection::FetchProperty(const String& dest, const String& path, const String& iface,
									const String& prop)
{
	return MethodCall(dest, path, "org.freedesktop.DBus.Properties", "Get", { iface, prop });
}

bool DBusConnection::BroadcastSignal(const String& path, const String& iface, const String& name,
									const DBusValueArray& args)
{
	ASSERT(socket.IsOpen());

	// We are using a form of "side-banding" here...
	extpacket << ~DBusMessage::CreateSignal(serial++, path, iface, name, args);
	Touch();
	return dispatching ? true : Run();
}

bool DBusConnection::RequestName(const String& name)
{
	return MethodCall("org.freedesktop.DBus", "/org/freedesktop/DBus",
					"org.freedesktop.DBus", "RequestName", { name, (uint32) 3 });
}

void DBusConnection::SendReply(const DBusMessage& req, const DBusValueArray& args)
{
	DBusMessage::FieldData fd = req.ParseFields();
	extpacket << ~DBusMessage::CreateMethodReturn(serial++, req.GetSerial(), fd.sender, args);
	Touch();
}

void DBusConnection::SendError(const DBusMessage& req, const String& errname, const String& errmsg)
{
	DBusMessage::FieldData fd = req.ParseFields();
	extpacket << ~DBusMessage::CreateError(serial++, req.GetSerial(), fd.sender, errname, errmsg);
	Touch();
}

}