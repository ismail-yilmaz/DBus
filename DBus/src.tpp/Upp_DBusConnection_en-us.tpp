topic "DBusConnection";
[i448;a25;kKO9;2 $$1,0#37138531426314131252341829483380:class]
[l288;2 $$2,2#27521748481378242620020725143825:desc]
[0 $$3,0#96390100711032703541132217272105:end]
[H6;0 $$4,0#05600065144404261032431302351956:begin]
[i448;a25;kKO9;2 $$5,0#37138531426314131252341829483370:item]
[l288;a4;*@5;1 $$6,6#70004532496200323422659154056402:requirement]
[l288;i1121;b17;O9;~~~.1408;2 $$7,0#10431211400427159095818037425705:param]
[i448;b42;O9;2 $$8,8#61672508125594000341940100500538:tparam]
[b42;2 $$9,9#13035079074754324216151401829390:normal]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_} 
[ {{10000@(113.42.0) [s0;%% [*@7;4 DBusConnection]]}}&]
[s3; &]
[s1;:Upp`:`:DBusConnection: [@(0.0.255)3 class][3  ][*3 DBusConnection]&]
[s0;l288;%% This class manages a low`-level UNIX domain socket connection 
to the [^https`:`/`/dbus`.freedesktop`.org`/doc`/dbus`-specification`.html^ D`-Bus] 
daemon. &]
[s2;%% &]
[s0;l288;i150;O0;%% Supports both blocking and non`-blocking operation 
modes.&]
[s2;i150;O0;%% Handles SASL EXTERNAL authentication and the handshake.&]
[s2;i150;O0;%% Allows client`-side method calls, property fetches, 
signal broadcasts, and match rule subscriptions. &]
[s2;i150;O0;%% Provides server`-side method handling with reply and 
error dispatching.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor detail]]}}&]
[s3; &]
[s5;:Upp`:`:DBusConnection`:`:DBusConnection`(`): [* DBusConnection]()&]
[s2;%% Default constructor.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Upp`:`:DBusConnection`:`:Timeout`(int`): DBusConnection[@(0.0.255) `&] 
[* Timeout]([@(0.0.255) int] [*@3 ms])&]
[s2;%% Sets the operation timeout in milliseconds. Acts as an idle 
transfer tracker (resets as bytes flow). Default timeout value 
is 60000 milliseconds (one minute). Returns `*this for method 
chaining.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusConnection`:`:WaitStep`(int`): DBusConnection[@(0.0.255) `&] 
[* WaitStep]([@(0.0.255) int] [*@3 ms])&]
[s2; Sets the polling interval in milliseconds used by the internal 
synchronous wait loop when checking socket readiness. Default 
value is 10 milliseconds. Returns `*this for method chaining.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusConnection`:`:NonBlocking`(bool`): DBusConnection[@(0.0.255) `&] 
[* NonBlocking]([@(0.0.255) bool] [*@3 b] [@(0.0.255) `=] [@(0.0.255) true])&]
[s2;%% Enables or disables asynchronous execution mode. When active, 
operations queue internally without blocking the calling thread. 
Returns `*this for method chaining.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusConnection`:`:IsBlocking`(`)const: [@(0.0.255) bool] 
[* IsBlocking]() [@(0.0.255) const]&]
[s2;%% Returns true if the connection is operating in blocking mode 
(i.e., timeout is Null).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusConnection`:`:Connect`(const String`&`,bool`): [@(0.0.255) bool] 
[* Connect]([@(0.0.255) const] String[@(0.0.255) `&] [*@3 path], [@(0.0.255) bool] 
[*@3 abstract] [@(0.0.255) `=] [@(0.0.255) false])&]
[s2;%% Connects directly to the D`-Bus UNIX domain socket at the 
specified [%-*@3 path] . Executes socket initialization, SASL authentication, 
and the Hello handshake. If [%-*@3 abstract] flag is true then 
DBusConnection will first attempt to connect to abstract unix 
socket endpoint. Returns true on success.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusConnection`:`:Connect`(const String`&`): [@(0.0.255) bool] 
[* Connect]([@(0.0.255) const] String[@(0.0.255) `&] [*@3 path])&]
[s2;%% Connects directly to the D`-Bus UNIX domain socket at the 
specified file [%-*@3 path] . Executes socket initialization, SASL 
authentication, and the Hello handshake. Returns true on success.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusConnection`:`:ConnectSession`(`): [@(0.0.255) bool] 
[* ConnectSession]()&]
[s2;%% Automatically detects and connects to the current user`'s 
session bus by evaluating the [C@5 DBUS`_SESSION`_BUS`_ADDRESS] 
environment variable or falling back to [C@5 /run/user/`{uid`}/bus]. 
Returns true on success.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusConnection`:`:ConnectSystem`(`): [@(0.0.255) bool] 
[* ConnectSystem]()&]
[s2;%% Automatically detects and connects to the system`-wide bus 
by evaluating the [C@5 DBUS`_SYSTEM`_BUS`_ADDRESS ]environment 
variable or falling back to [C@5 /var/run/dbus/system`_bus`_socket]. 
Returns true on success. .&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusConnection`:`:Disconnect`(`): [@(0.0.255) void] [* Disconnect]()&]
[s2;%% Safely terminates the connection by closing the underlying 
socket, resetting operational status to IDLE, and clearing buffered 
data.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusConnection`:`:Listen`(`): [@(0.0.255) void] [* Listen]()&]
[s2;%% Switches the connection into listening (server) mode to intercept 
and dispatch incoming broadcast signals and server`-side method 
invocations.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusConnection`:`:RequestName`(const String`&`): [@(0.0.255) bool] 
[* RequestName]([@(0.0.255) const] String[@(0.0.255) `&] [*@3 name])&]
[s2;%% Requests a well`-known service [%-*@3 name ]on the bus. Returns 
true on success.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusConnection`:`:SendReply`(const DBusMessage`&`,const DBusValueArray`&`): [@(0.0.255) v
oid] [* SendReply]([@(0.0.255) const] DBusMessage[@(0.0.255) `&] [*@3 req], 
[@(0.0.255) const] DBusValueArray[@(0.0.255) `&] args `= `{`})&]
[s2;%% Buffers and queues a successful method return response directed 
back to the unique sender of the incoming request [%-*@3 req].&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusConnection`:`:SendError`(const DBusMessage`&`,const String`&`,const String`&`): [@(0.0.255) v
oid] [* SendError]([@(0.0.255) const] DBusMessage[@(0.0.255) `&] [*@3 req], 
[@(0.0.255) const] String[@(0.0.255) `&] [*@3 errname], [@(0.0.255) const] 
String[@(0.0.255) `&] [*@3 errmsg] [@(0.0.255) `=] Null)&]
[s2;%% Buffers and queues an error rejection reply back to the sender 
of [%-*@3 req], specifying the D`-Bus error name [%-*@3 errname] 
and an optional message [%-*@3 errmsg].&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusConnection`:`:MethodCall`(const String`&`,const String`&`,const String`&`,const String`&`,const DBusValueArray`&`): [@(0.0.255) b
ool] [* MethodCall]([@(0.0.255) const] String[@(0.0.255) `&] [*@3 dest], 
[@(0.0.255) const] String[@(0.0.255) `&] [*@3 path], [@(0.0.255) const] 
String[@(0.0.255) `&] [*@3 iface], [@(0.0.255) const] String[@(0.0.255) `&] 
[*@3 method], [@(0.0.255) const] DBusValueArray[@(0.0.255) `&] args 
`= `{`})&]
[s2;%% Invokes a remote D`-Bus method on [%-*@3 destination ]service 
[%-*@3 dest], object [%-*@3 path] , [%-*@3 interface], and [%-*@3 method] 
name. Blocks in synchronous mode; queues asynchronously and returns 
immediately in non`-blocking mode. Returns true on success.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusConnection`:`:BroadcastSignal`(const String`&`,const String`&`,const String`&`,const DBusValueArray`&`): [@(0.0.255) b
ool] [* BroadcastSignal]([@(0.0.255) const] String[@(0.0.255) `&] [*@3 path], 
[@(0.0.255) const] String[@(0.0.255) `&] [*@3 iface], [@(0.0.255) const] 
String[@(0.0.255) `&] [*@3 name], [@(0.0.255) const] DBusValueArray[@(0.0.255) `&] 
args `= `{`})&]
[s2;%% Emits a broadcast signal across the bus from object [%-*@3 path] 
and [%-*@3 interface]. Returns true on success.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusConnection`:`:AddMatch`(const String`&`,Event`): [@(0.0.255) bool] 
[* AddMatch]([@(0.0.255) const] String[@(0.0.255) `&] [*@3 rule], Event<[@(0.0.255) const] 
DBusMessage[@(0.0.255) `&]> [*@3 cb] [@(0.0.255) `=] Null)&]
[s2;%% Transmits an AddMatch request to the daemon using the specified 
[%-*@3 rule] string. If a callback [%-*@3 cb] is provided, incoming 
matching signals are routed directly to it. Returns true on success.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusConnection`:`:RemoveMatch`(const String`&`): [@(0.0.255) bool] 
[* RemoveMatch]([@(0.0.255) const] String[@(0.0.255) `&] [*@3 rule])&]
[s2;%% Unregisters the specified matching [%-*@3 rule] added by [^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:AddMatch`(const String`&`,Event`)^ A
ddMatch()] method. Returns true on success.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusConnection`:`:FetchProperty`(const String`&`,const String`&`,const String`&`,const String`&`): [@(0.0.255) b
ool] [* FetchProperty]([@(0.0.255) const] String[@(0.0.255) `&] [*@3 destination], 
[@(0.0.255) const] String[@(0.0.255) `&] [*@3 path], [@(0.0.255) const] 
String[@(0.0.255) `&] [*@3 interface], [@(0.0.255) const] String[@(0.0.255) `&] 
[*@3 property])&]
[s2;%% Convenience wrapper that queries a remote object [%-*@3 property 
]via [C@5 org.freedesktop.DBus.Properties.Get]. Returns true on 
success. &]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusConnection`:`:Do`(`): [@(0.0.255) bool] [* Do]()&]
[s2;%% Progresses a single step of the asynchronous operation. Returns 
true while processing is not finished. Can only be called in 
non`-blocking mode.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusConnection`:`:WhenDo: Event<> [* WhenDo]&]
[s2;%% Dispatched each time [^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:Do`(`)^ D
o()] method exits. Useful for updating status or showing progress.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusConnection`:`:InProgress`(`)const: [@(0.0.255) bool] 
[* InProgress]() [@(0.0.255) const]&]
[s2;%% Returns true if any operation is in progress.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusConnection`:`:IsSuccess`(`)const: [@(0.0.255) bool] 
[* IsSuccess]() [@(0.0.255) const]&]
[s2;%% Returns true if the previous operation is a success.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusConnection`:`:IsFailure`(`)const: [@(0.0.255) bool] 
[* IsFailure]() [@(0.0.255) const]&]
[s2;%% Returns true if the previous operation has failed.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusConnection`:`:Finish`(`): [@(0.0.255) void] [* Finish]()&]
[s2;%% Finishes the current operation. (Useful especially in listening 
mode.)&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusConnection`:`:Abort`(`): [@(0.0.255) void] [* Abort]()&]
[s2;%% Aborts the current operation.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusConnection`:`:GetMessage`(`)const: [@(0.0.255) const] 
DBusMessage[@(0.0.255) `&] [* GetMessage]() [@(0.0.255) const]&]
[s2;%% Returns a constant reference to the reply message received 
from the most recently completed method call.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusConnection`:`:WhenSignal: Event<[@(0.0.255) const] 
DBusMessage[@(0.0.255) `&]> [* WhenSignal]&]
[s2;%% Fallback event callback triggered when an incoming broadcast 
signal arrives that does not have a dedicated AddMatch callback.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusConnection`:`:WhenMethodCall: Event<[@(0.0.255) const] 
DBusMessage[@(0.0.255) `&]> [* WhenMethodCall]&]
[s2;%% Event callback triggered when an incoming remote method invocation 
targeted at this application is received over the bus.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusConnection`:`:GetSocket`(`): Socket[@(0.0.255) `&] 
[* GetSocket]()&]
[s2;%% Returns a reference to the underlying [^topic`:`/`/Core`/src`/Socket`_en`-us`#Socket`:`:class^ S
ocket].&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusConnection`:`:GetWaitEvents`(`)const: dword [* GetWaitEvents]() 
[@(0.0.255) const]&]
[s2;%% Returns a combination of [C@5 WAIT`_READ] and [C@5 WAIT`_WRITE] 
flags to indicate what is blocking operation. Can be used with 
[^topic`:`/`/Core`/src`/SocketWaitEvent`_en`-us`#SocketWaitEvent`:`:class^ SocketWa
itEvent].&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusConnection`:`:AddTo`(SocketWaitEvent`&`): DBusConnection[@(0.0.255) `&] 
[* AddTo](SocketWaitEvent[@(0.0.255) `&] [*@3 e])&]
[s2;%% Adds underlying connection socket to [^topic`:`/`/Core`/src`/SocketWaitEvent`_en`-us`#SocketWaitEvent`:`:class^ S
ocketWaitEvent] for waiting on it. Returns `*this for method 
chaining.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusConnection`:`:IsError`(`)const: [@(0.0.255) bool] [* IsError]() 
[@(0.0.255) const]&]
[s2;%% Returns true if the connection has encountered a fatal socket, 
authentication, protocol, or timeout error.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusConnection`:`:GetError`(`)const: [@(0.0.255) int] [* GetError]() 
[@(0.0.255) const]&]
[s2;%% Returns the numeric error code associated with the current 
error.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:DBusConnection`:`:GetErrorDesc`(`)const: String [* GetErrorDesc]() 
[@(0.0.255) const]&]
[s2;%% Returns the text description of the current error .&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusConnection`:`:GetUniqueName`(`)const: String [* GetUniqueName]() 
[@(0.0.255) const]&]
[s2;%% Returns the unique client connection name assigned by the 
D`-Bus daemon (e.g., `":1.42`".&]
[s3; &]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Error Codes]]}}&]
[s3;%% &]
[s0;%% On error, [^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection^ D
BusConnection] can return the following error codes via [^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:GetError`(`)const^ G
etError()] method:&]
[s0;%% &]
[ {{3297:6703 [s0;%% [* Enum]]
:: [s0;%% [* Description]]
:: [s0;%% [C@5 CONNECTION`_FAILED]]
:: [s0;%% Couldn`'t connect to D`-Bus server.]
:: [s0;%% [C@5 AUTH`_FAILED]]
:: [s0;%% Authentication failed.]
:: [s0;%% [C@5 HELLO`_FAILED]]
:: [s0;%% Hello request failed.]
:: [s0;%% [C@5 CONNECTION`_TIMED`_OUT]]
:: [s0;%% Connection timed out.]
:: [s0;%% [C@5 INVALID`_PACKET]]
:: [s0;%% Invalid packet received.]
:: [s0;%% [C@5 SOCKET`_FAILURE]]
:: [s0;%% Socket error occured.]
:: [s0;%% [C@5 ABORTED]]
:: [s0;%% Operation was aborted.]
:: [s0;%% [C@5 EXCEPTION]]
:: [s0;%% Unknown exception.]}}]]