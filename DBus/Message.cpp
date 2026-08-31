#include "DBus.h"

#define LLOG(x)      // RLOG("D-Bus: " << x)
#define LDUMPHEX(x)  // RDUMPHEX(x)

namespace Upp {

namespace {

enum DBusValueType : char {
	DBUS_BYTE          = 'y',
	DBUS_BOOL          = 'b',
	DBUS_INT16         = 'n',
	DBUS_UINT16        = 'q',
	DBUS_INT32         = 'i',
	DBUS_UINT32        = 'u',
	DBUS_INT64         = 'x',
	DBUS_UINT64        = 't',
	DBUS_DOUBLE        = 'd',
	DBUS_STRING        = 's',
	DBUS_OBJECT_PATH   = 'o',
	DBUS_SIGNATURE     = 'g',
	DBUS_ARRAY         = 'a',
	DBUS_VARIANT       = 'v'
};

void AppendAlign(String& buf, int alignment)
{
	while(buf.GetLength() % alignment != 0)
		buf.Cat(0);
}

void AppendString(String& buf, const String& str)
{
	dword len = str.GetLength();
	buf.Cat((char*)&len, 4);
	buf.Cat(str);
	buf.Cat(0);
}

void AppendField(String& fields, byte code, char sig, const String& val)
{
	AppendAlign(fields, 8);
	fields.Cat(code);
	fields.Cat(1);
	fields.Cat(sig);
	fields.Cat(0);
	AppendAlign(fields, 4);
	AppendString(fields, val);
}

void AppendField(String& fields, byte code, char sig, dword val)
{
	AppendAlign(fields, 8);
	fields.Cat(code);
	fields.Cat(1);
	fields.Cat(sig);
	fields.Cat(0);
	AppendAlign(fields, 4);
	fields.Cat((char*)&val, 4);
}

char GetValueSignature(const DBusValue& val)
{
	if(val.Is<String>())
		return DBUS_STRING;
	if(val.Is<bool>())
		return DBUS_BOOL;
	if(val.Is<byte>())
		return DBUS_BYTE;
	if(val.Is<int16>())
		return DBUS_INT16;
	if(val.Is<uint16>())
		return DBUS_UINT16;
	if(val.Is<int32>())
		return DBUS_INT32;
	if(val.Is<uint32>())
		return DBUS_UINT32;
	if(val.Is<int64>())
		return DBUS_INT64;
	if(val.Is<uint64>())
		return DBUS_UINT64;
	if(val.Is<double>())
		return DBUS_DOUBLE;
	if(val.Is<DBusValueArray>())
		return DBUS_VARIANT;
	if(val.Is<DBusValueMap>())
		return DBUS_ARRAY;

	return DBUS_STRING;
}

void MarshalParam(String& body, String& signature, const DBusValue& val)
{
	if(val.Is<String>()) {
		signature.Cat(DBUS_STRING);
		AppendAlign(body, 4);
		AppendString(body, (String)val);
	}
	else
	if(val.Is<byte>()) {
		signature.Cat(DBUS_BYTE);
		// DBUS_BYTE does not require padding, it aligns to 1
		byte v = val;
		body.Cat((char*)&v, 1);
	}
	else
	if(val.Is<bool>()) {
		signature.Cat(DBUS_BOOL);
		AppendAlign(body, 4);
		dword b = (bool) val ? 1 : 0;
		body.Cat((char*)&b, 4);
	}
	else
	if(val.Is<int16>()) {
		signature.Cat(DBUS_INT16);
		AppendAlign(body, 2);
		int16 v = val;
		body.Cat((char*)&v, 2);
	}
	else
	if(val.Is<uint16>()) {
		signature.Cat(DBUS_UINT16);
		AppendAlign(body, 2);
		uint16 v = val;
		body.Cat((char*)&v, 2);
	}
	else
	if(val.Is<int32>()) {
		signature.Cat(DBUS_INT32);
		AppendAlign(body, 4);
		int32 v = val;
		body.Cat((char*)&v, 4);
	}
	else
	if(val.Is<uint32>()) {
		signature.Cat(DBUS_UINT32);
		AppendAlign(body, 4);
		uint32 v = val;
		body.Cat((char*) &v, 4);
	}
	else
	if(val.Is<int64>()) {
		signature.Cat(DBUS_INT64);
		AppendAlign(body, 8);
		int64 v = val;
		body.Cat((char*)&v, 8);
	}
	else
	if(val.Is<uint64>()) {
		signature.Cat(DBUS_UINT64);
		AppendAlign(body, 8);
		uint64 v = val;
		body.Cat((char*)&v, 8);
	}
	else
	if(val.Is<double>()) {
		signature.Cat(DBUS_DOUBLE);
		AppendAlign(body, 8);
		double v = val;
		body.Cat((char*)&v, 8);
	}
	else
	if(val.Is<DBusValueArray>()) {
		signature.Cat(DBUS_ARRAY);
		DBusValueArray arr = val;
		String elmsig;
		if(arr.GetCount() > 0) {
			String dummy;
			MarshalParam(dummy, elmsig, arr[0]);
		}
		else
			elmsig = String(DBUS_STRING, 1);

		signature.Cat(elmsig);
		AppendAlign(body, 4);
		int lenpos = body.GetLength();
		body.Cat("    ", 4);

		// Determine correct alignment for the D-Bus array payload
		int elmaln = 4;
		char bsig = elmsig[0];
		if(bsig == DBUS_INT64
		|| bsig == DBUS_UINT64
		|| bsig == DBUS_DOUBLE
		|| bsig == '('
		|| bsig == '{')
			elmaln = 8;
		else
		if(bsig == DBUS_INT16
		|| bsig == DBUS_UINT16)
			elmaln = 2;
		else
		if(bsig == DBUS_BYTE
		|| bsig == 'v'
		|| bsig == 'g')
			elmaln = 1;

		AppendAlign(body, elmaln);
		int startpos = body.GetLength();

		for(int i = 0; i < arr.GetCount(); i++) {
			AppendAlign(body, elmaln);
			String unused;
			MarshalParam(body, unused, arr[i]);
		}

		int endpos = body.GetLength();
		dword arrlen = endpos - startpos;

		String p1 = body.Left(lenpos);
		String p2 = body.Mid(lenpos + 4);
		body = p1;
		body.Cat((char*)&arrlen, 4);
		body.Cat(p2);
	}
	else
	if(val.Is<DBusValueMap>()) {
		signature.Cat("a{sv}");
		DBusValueMap map = val;

		AppendAlign(body, 4);
		int lenpos = body.GetLength();
		body.Cat("    ", 4);

		AppendAlign(body, 8);
		int startpos = body.GetLength();

		for(int i = 0; i < map.GetCount(); i++) {
			AppendAlign(body, 8);
			AppendString(body, map.GetKey(i));

			DBusValue v = map[i];
			String varsig;
			String dummy;
			MarshalParam(dummy, varsig, v);

			body.Cat((byte) varsig.GetLength());
			body.Cat(varsig);
			body.Cat(0);

			// Directly marshal into the body to correctly maintain byte alignment
			String unused;
			MarshalParam(body, unused, v);

			if(i < map.GetCount() - 1)
				AppendAlign(body, 8);
		}

		int endpos = body.GetLength();
		dword arrlen = endpos - startpos;

		String p1 = body.Left(lenpos);
		String p2 = body.Mid(lenpos + 4);
		body = p1;
		body.Cat((char*)&arrlen, 4);
		body.Cat(p2);
	}
	else
	if(val.Is<DBusValueStruct>()) {
		signature.Cat('(');
		DBusValueStruct fields = val;
		// Structs always align to an 8-byte boundary when nested or started
		AppendAlign(body, 8);
		for(const DBusValue& v : fields)
			MarshalParam(body, signature, v);
		signature.Cat(')');
	}

}

void MarshalParams(String& body, String& signature, const DBusValueArray& args)
{
	for(const DBusValue& v : args)
		MarshalParam(body, signature, v);
}

void SkipSignature(BParser& sigbp)
{
	if(!sigbp.IsAvail(1))
		return;

	char c = (char)sigbp.GetByte();
	if(c == DBUS_ARRAY) {
		SkipSignature(sigbp);
	}
	else
	if(c == '{' || c == '(') {
		char endc = (c == '{') ? '}' : ')';
		while(sigbp.IsAvail(1) && sigbp.PeekByte() != endc) {
			SkipSignature(sigbp);
		}
		if(sigbp.IsAvail(1) && sigbp.PeekByte() == endc)
			sigbp.SkipByte();
	}
}

DBusValue ParseType(BParser& bp, BParser& sigbp)
{
	if(!sigbp.IsAvail(1))
		return Null;

	char t = (char) sigbp.GetByte();
	if(!t)
		return Null;

	if(t == DBUS_STRING || t == DBUS_OBJECT_PATH) {
		bp.Align(4);
		dword len = bp.ReadDword();
		String res = bp.ReadString(len);
		bp.SkipByte();
		return res;
	}
	if(t == DBUS_INT32) {
		bp.Align(4);
		return bp.ReadInt32();
	}
	if(t == DBUS_UINT32) {
		bp.Align(4);
		return bp.ReadUInt32();
	}
	if(t == DBUS_BOOL) {
		bp.Align(4);
		return (bool) bp.ReadDword() != 0;
	}
	if(t == DBUS_INT64) {
		bp.Align(8);
		return bp.ReadInt64();
	}
	if(t == DBUS_UINT64) {
		bp.Align(8);
		return bp.ReadUInt64();
	}
	if(t == DBUS_DOUBLE) {
		bp.Align(8);
		return bp.ReadDouble();
	}
	if(t == DBUS_BYTE) {
		return bp.GetByte();
	}
	if(t == DBUS_INT16) {
		bp.Align(2);
		return bp.ReadInt16();
	}
	if(t == DBUS_UINT16) {
		bp.Align(2);
		return bp.ReadInt16();
	}
	if(t == DBUS_SIGNATURE) {
		byte len = bp.GetByte();
		String res = bp.ReadString(len);
		bp.SkipByte();
		return res;
	}
	if(t == DBUS_VARIANT) {
		byte len = bp.GetByte();
		String vsig = bp.ReadString(len);
		bp.SkipByte();                        // Skip null terminator in payload body
		if(len > 0) {
			BParser subbp(vsig);
			return ParseType(bp, subbp);
		}
		return Null;
	}
	if(t == DBUS_ARRAY) {
		bp.Align(4);
		dword len = bp.ReadDword();

		int elmsigoff = (int) sigbp.GetOffset();
		SkipSignature(sigbp);                // Advance parent pointer past the element signature

		int savedoff = (int)sigbp.GetOffset();
		sigbp.Seek(elmsigoff);
		char c = (char)sigbp.GetByte();
		sigbp.Seek(savedoff);

		int aln = 1;
		if(c == DBUS_INT64
		|| c == DBUS_UINT64
		|| c == DBUS_DOUBLE
		|| c == '{'
		|| c == '(')
			aln = 8;
		else
		if(c == DBUS_INT32
		|| c == DBUS_UINT32
		|| c == DBUS_BOOL
		|| c == DBUS_STRING
		|| c == DBUS_OBJECT_PATH
		|| c == DBUS_ARRAY)
			aln = 4;
		else
		if(c == DBUS_INT16
		|| c == DBUS_UINT16)
			aln = 2;

		bp.Align(aln);
		int endpos = (int)bp.GetOffset() + len;

		if(c == '{') {
			DBusValueMap map;
			while((int) bp.GetOffset() < endpos) {
				bp.Align(8);
				sigbp.Seek(elmsigoff + 1); // Skip '{'
				DBusValue key = ParseType(bp, sigbp);
				DBusValue val = ParseType(bp, sigbp);
				map.Add(key, val);
			}
			sigbp.Seek(savedoff); // Loop body always leaves sigbp one byte short, sitting on '}'
			return map;
		}
		else {
			DBusValueArray arr;
			while((int) bp.GetOffset() < endpos) {
				sigbp.Seek(elmsigoff);
				DBusValue val = ParseType(bp, sigbp);
				arr.Add(val);
			}
			return arr;
		}
	}
	if(t == '(') {
		DBusValueStruct strc;
		bp.Align(8);
		while(sigbp.IsAvail(1) && sigbp.PeekByte() != ')')
			strc.Add(ParseType(bp, sigbp));
		if(sigbp.IsAvail(1) && sigbp.PeekByte() == ')')
			sigbp.SkipByte();
		return strc;
	}

	return Null;
}

}

DBusMessage::DBusMessage()
{
	Zero(header);
}

DBusMessage::DBusMessage(const String& rawdata)
	: data(rawdata)
{
	Zero(header);
	if(BParser bp(data); bp.IsAvail(16)) {
		header.endian = bp.GetByte();
		header.type = bp.GetByte();
		header.flags = bp.GetByte();
		header.version = bp.GetByte();
		bp.BigEndian(header.endian == 'B');
		header.bodylen = bp.ReadDword();
		header.serial = bp.ReadDword();
		header.fieldslen = bp.ReadDword();
	}
}

DBusMessage DBusMessage::Create(byte type, byte flags, dword serial, String fields, const DBusValueArray& args)
{
	DBusMessage msg;
	String body;
	String signature;

	if(args.GetCount() > 0) {
		MarshalParams(body, signature, args);
		AppendAlign(fields, 8);
		fields.Cat(8);
		fields.Cat(1);
		fields.Cat(DBUS_SIGNATURE);
		fields.Cat(0);
		AppendAlign(fields, 4);
		byte siglen = signature.GetLength();
		fields.Cat(siglen);
		fields.Cat(signature);
		fields.Cat(0);
	}

	dword bodylen = body.GetLength();
	dword fieldslen = fields.GetLength();

	msg.header.endian = 'l';
	msg.header.type = type;
	msg.header.flags = flags;
	msg.header.version = 1;
	msg.header.bodylen = bodylen;
	msg.header.serial = serial;
	msg.header.fieldslen = fieldslen;

	msg.data.Cat('l');
	msg.data.Cat(type);
	msg.data.Cat(flags);
	msg.data.Cat((byte) 1);
	msg.data.Cat((char *)&bodylen, 4);
	msg.data.Cat((char *)&serial, 4);
	msg.data.Cat((char *)&fieldslen, 4);
	msg.data.Cat(fields);
	AppendAlign(msg.data, 8);

	if(bodylen > 0)
		msg.data.Cat(body);

	return msg;
}

DBusMessage DBusMessage::CreateMethodCall(dword serial, const String& dest, const String& path,
										const String& iface, const String& method,
										const DBusValueArray& args)
{
	String fields;
	AppendField(fields, 1, DBUS_OBJECT_PATH, path);
	AppendField(fields, 2, DBUS_STRING, iface);
	AppendField(fields, 3, DBUS_STRING, method);
	AppendField(fields, 6, DBUS_STRING, dest);

	return Create(METHOD_CALL, 0, serial, fields, args);
}

DBusMessage DBusMessage::CreateSignal(dword serial, const String& path, const String& iface,
										const String& name, const DBusValueArray& args)
{
	String fields;
	AppendField(fields, 1, DBUS_OBJECT_PATH, path);
	AppendField(fields, 2, DBUS_STRING, iface);
	AppendField(fields, 3, DBUS_STRING, name);

	// Message Type: SIGNAL (4)
	return Create(SIGNAL, 0, serial, fields, args);
}

DBusMessage DBusMessage::CreateMethodReturn(dword serial, dword replyserial, const String& dest,
										const DBusValueArray& args)
{
	String fields;
	AppendField(fields, 6, DBUS_STRING, dest);
	AppendField(fields, 5, DBUS_UINT32, replyserial);

	// NO_REPLY_EXPECTED = 1
	return Create(METHOD_RETURN, 1, serial, fields, args);
}

DBusMessage DBusMessage::CreateError(dword serial, dword replyserial, const String& dest,
										const String& errname, const String& errmsg)
{
	String fields;
	AppendField(fields, 6, DBUS_STRING, dest);
	AppendField(fields, 5, DBUS_UINT32, replyserial);
	AppendField(fields, 4, DBUS_STRING, errname);

	DBusValueArray args;
	if(!errmsg.IsEmpty())
		args.Add(errmsg);

	// NO_REPLY_EXPECTED = 1
	return Create(ERROR, 1, serial, fields, args);
}

DBusMessage::FieldData DBusMessage::ParseFields() const
{
	FieldData fields;
	if(data.GetLength() < 16)
		return fields;

	try {
		BParser bp(data);
		bp.BigEndian(header.endian == 'B');
		bp.Seek(16);

		int end = 16 + header.fieldslen;

		while(bp.GetOffset() < end && bp.IsAvail(2)) {
			bp.Align(8);
			if(bp.GetOffset() >= end)
				break;

			byte code = bp.GetByte();
			byte slen = bp.GetByte();
			String sig = bp.ReadString(slen);
			bp.SkipByte();

			if(sig.GetCount() == 1 && (sig[0] == DBUS_OBJECT_PATH || sig[0] == DBUS_STRING)) {
				bp.Align(4);
				dword len = bp.ReadDword();
				String val = bp.ReadString(len);
				bp.SkipByte();

				switch(code) {
				case 1:
					fields.path = val;
					break;
				case 2:
					fields.interface = val;
					break;
				case 3:
					fields.member = val;
					break;
				case 4:
					fields.error = val;
					break;
				case 6:
					fields.destination = val;
					break;
				case 7:
					fields.sender = val;
					break;
				}
			}
			else
			if(sig.GetCount() == 1 && sig[0] == DBUS_SIGNATURE) {
				byte len = bp.GetByte();
				String val = bp.ReadString(len);
				bp.SkipByte();
				if(code == 8)
					fields.signature = val;
			}
			else
			if(sig.GetCount() == 1 && sig[0] == DBUS_UINT32) {
				bp.Align(4);
				dword val = bp.ReadDword();
				if(code == 5)
					fields.reply = val;
			}
		}
	}
	catch(const BParser::Error& e) {
		LLOG("ParseFields() failed: " << e);
	}

	return fields;
}

DBusValueArray DBusMessage::ParseBody() const
{
	DBusValueArray res;

	FieldData flds = ParseFields();
	if(flds.signature.IsEmpty())
		return res;

	try {
		BParser bp(data);
		bp.BigEndian(header.endian == 'B');

		int bodypos = 16 + header.fieldslen;
		while(bodypos % 8 != 0)
			bodypos++;
		bp.Seek(bodypos);

		BParser sigbp(flds.signature);
		while(sigbp.IsAvail(1)) {
			if(DBusValue v = ParseType(bp, sigbp); !IsNull(v))
				res.Add(v);
		}
	}
	catch(const BParser::Error& e) {
		LLOG("ParseBody() failed: " << e);
	}

	return res;
}

bool DBusMessage::MatchRule(const String& rule) const
{
	FieldData fd = ParseFields();
	Vector<String> parts = Split(rule, ',');

	for(const String& p : parts) {
		int eq = p.Find('=');
		if(eq < 0)
			continue;

		String key = TrimBoth(p.Left(eq));
		String val = TrimBoth(p.Mid(eq + 1));

		if(val.StartsWith("'") && val.EndsWith("'"))
			val = val.Mid(1, val.GetLength() - 2);

		if(key == "type") {
			if((val == "signal" && !IsSignal())
			|| (val == "method_call" && !IsMethodCall())
			|| (val == "method_return" && !IsMethodReturn())
			|| (val == "error" && !IsError()))
				return false;
		}
		else
		if((key == "interface" && fd.interface != val)
		|| (key == "member" && fd.member != val)
		|| (key == "path" && fd.path != val)
		|| (key == "sender" && fd.sender != val)
		|| (key == "destination" && fd.destination != val))
			return false;
	}
	return true;
}

String DBusMessage::GetErrorDesc() const
{
	if(!IsError())
		return String();

	DBusValueArray out = ParseBody();
	if(out.GetCount() > 0 && out[0].Is<String>())
		return out[0];

	return GetErrorName();
}

Vector<String> DBusMessage::ParseStringArray() const
{
	Vector<String> result;

	try {
		BParser bp(data);
		bp.BigEndian(header.endian == 'B');

		int bodypos = 16 + header.fieldslen;
		while(bodypos % 8 != 0)
			bodypos++;
		bp.Seek(bodypos);

		dword arrlen = bp.ReadDword();
		int end = (int)bp.GetOffset() + arrlen;

		while(bp.GetOffset() < end && bp.IsAvail(4)) {
			bp.Align(4);
			if(!bp.IsAvail(4) || bp.GetOffset() >= end)
				break;

			dword slen = bp.ReadDword();
			result.Add(bp.ReadString(slen));
			bp.SkipByte();
		}
	}
	catch(const BParser::Error& e) {
		LLOG("ParseStringArray() failed: " << e);
	}

	return result;
}

String DBusMessage::ParseString() const
{
	try {
		BParser bp(data);
		bp.BigEndian(header.endian == 'B');

		int bodypos = 16 + header.fieldslen;
		while(bodypos % 8 != 0)
			bodypos++;
		bp.Seek(bodypos);

		dword len = bp.ReadDword();
		return bp.ReadString(len);
	}
	catch(const BParser::Error& e) {
		LLOG("ParseString() failed: " << e);
		return String::GetVoid();
	}
}

}