class DBusMessage {
public:
    enum Type {
        METHOD_CALL = 1,
        METHOD_RETURN = 2,
        ERROR = 3,
        SIGNAL = 4
    };

    struct Header {
        byte endian;
        byte type;
        byte flags;
        byte version;
        dword bodylen;
        dword serial;
        dword fieldslen;
    };

    struct FieldData {
        String path;
        String interface;
        String member;
        String sender;
        String destination;
        String signature;
        String error;
        dword  reply;
        FieldData() : reply(0) {}
    };
    
public:
    DBusMessage();
    DBusMessage(const String& rawdata);

    static DBusMessage CreateMethodCall(dword serial, const String& dest, const String& path, const String& iface, const String& method, const DBusValueArray& args = {});
    static DBusMessage CreateMethodReturn(dword serial, dword replyserial, const String& dest, const DBusValueArray& args = {});
    static DBusMessage CreateSignal(dword serial, const String& path, const String& iface, const String& name, const DBusValueArray& args = {});
    static DBusMessage CreateError(dword serial, dword replyserial, const String& dest, const String& errname, const String& errmsg);

    bool               IsOK() const             { return GetType() != ERROR; }
    bool               IsMethodCall() const     { return GetType() == METHOD_CALL; }
    bool               IsMethodReturn() const   { return GetType() == METHOD_RETURN; }
    bool               IsSignal() const         { return GetType() == SIGNAL; }
    bool               IsError() const          { return GetType() == ERROR; }

    Type               GetType() const          { return (Type) header.type; }
    dword              GetSerial() const        { return header.serial; }
    const Header&      GetHeader() const        { return header; }
    const String&      GetRawData() const       { return data; }
    const String&      operator~() const        { return data; }
	
    FieldData          ParseFields() const;
    DBusValueArray     ParseBody() const;
    String             ParseString() const;
    Vector<String>     ParseStringArray() const;

    bool               MatchRule(const String& rule) const;

    String             GetErrorName() const     { return ParseFields().error; }
    String             GetErrorDesc() const;
    
private:
    static DBusMessage Create(byte type, byte flags, dword serial, String fields, const DBusValueArray& args);
    
    String data;
    Header header;
};
