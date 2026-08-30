class DBusConnection {
public:
    DBusConnection();
    virtual ~DBusConnection();
    
    DBusConnection&     Timeout(int ms)                                 { timeout = ms; return *this; }
    DBusConnection&     WaitStep(int ms)                                { waitstep = max(0, ms); return *this; }

    DBusConnection&     NonBlocking(bool b = true)                      { async = b; return *this; }
    bool                IsBlocking() const                              { return IsNull(timeout);  }

    bool                Connect(const String& path);
    bool                ConnectSession();
    bool                ConnectSystem();
    void                Disconnect();

    void                Listen();

    bool                RequestName(const String& name);
    void                SendReply(const DBusMessage& req, const DBusValueArray& args = {});
    void                SendError(const DBusMessage& req, const String& errname, const String& errmsg = Null);

    bool                MethodCall(const String& dest, const String& path, const String& iface, const String& method, const DBusValueArray& args = {});

    bool                BroadcastSignal(const String& path, const String& iface, const String& name, const DBusValueArray& args = {});
    
    bool                AddMatch(const String& rule, Event<const DBusMessage&> cb = Null);

    bool                FetchProperty(const String& dest, const String& path, const String& iface, const String& prop);
    
    bool                Do()                                            { ASSERT(async); return Do0(); }
    Event<>             WhenDo;

	void                Abort()                                         { socket.Abort();  }
	
    const DBusMessage&  GetMessage() const                              { return replymsg; }
    
    Event<const DBusMessage&> WhenSignal;
    Event<const DBusMessage&> WhenMethodCall;

    Socket&             GetSocket()                                     { return socket; }
    dword               GetWaitEvents() const                           { return WAIT_READ | !!(packet.GetCount() + outpacket.GetCount()) * WAIT_WRITE; }
    DBusConnection&     AddTo(SocketWaitEvent& e)                       { e.Add(socket, GetWaitEvents()); return *this; }

    bool                IsError() const                                 { return status == FAILED; }
    int                 GetError() const                                { return error.Get<int>(); }
    String              GetErrorDesc() const                            { return error.Get<String>(); }
    String              GetUniqueName() const                           { return uniquename; }

    enum ErrorCodes {
        CONNECTION_FAILED = 10000,
        AUTH_FAILED,
        HELLO_FAILED,
        CONNECTION_TIMED_OUT,
        INVALID_PACKET,
        SOCKET_FAILURE,
        ABORTED,
        EXCEPTION = -1,
   };

    struct Error : Exc {
        int code;
        Error() : Exc(Null), code(-1) {}
        Error(const String& reason) : Exc(reason), code(-1) {}
        Error(int rc, const String& reason) : Exc(reason), code(rc) {}
    };

private:
    static const char*  GetMsg(int code);
    void                SetError(int code)                              { throw Error(code, GetMsg(code)); }
    bool                Init();
    bool                FsConnect();
    bool                Get();
    bool                Put();
    bool                Drain();
    void                PutGet();
    void                Check();
    bool                Do0();
    bool                Run();
    bool                IsTimeout() const                               { return !IsNull(timeout) && msecs(starttime) >= timeout; }

    bool                AuthRequest();
    bool                AuthParse();
    bool                AuthIsEof();

    bool                HelloRequest();
    bool                HelloParse();
    bool                HelloIsEof();

    bool                InitCall();
    bool                MethodRequest();
    bool                MethodIsEof();

    bool                ListenIsEof();
    void                DispatchSignal(const DBusMessage& msg);

    bool                MsgIsEof();
    DBusMessage         ExtractMessage();
    
    enum State          { IDLE, WORKING, FINISHED, FAILED };

    struct SignalMatch : Moveable<SignalMatch> {
        String rule;
        Event<const DBusMessage&> cb;
    };

    Socket              socket;
    String              packet;
    int                 packlen;
    String              outpacket;
    int                 outpacklen;
    String              buspath;
    int                 status;
    int                 starttime;
    int                 timeout;
    int                 waitstep;
    bool                async:1;
    bool                dispatching;
    Tuple<int, String>  error;
    Gate<>              IsEof;
    BiVector<Gate<>>    queue;

    dword               serial;
    dword               callserial;
    String              uniquename;
    DBusMessage         replymsg;
    Vector<SignalMatch> signalmatches;
};