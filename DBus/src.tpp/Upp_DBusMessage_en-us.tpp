topic "DBusMessage";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 DBusMessage]]}}&]
[s3; &]
[s1;:Upp`:`:DBusMessage: [@(0.0.255)3 class][3  ][*3 DBusMessage]&]
[s2;%% This class represents a [^https`:`/`/dbus`.freedesktop`.org`/doc`/dbus`-specification`.html^ D
`-Bus protocol] message. It provides facilities to construct, 
parse, and evaluate raw binary D`-Bus messages, covering method 
calls, replies, signals, and errors. It handles the serialization 
and unmarshaling of the D`-Bus binary wire format.&]
[s3;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor detail]]}}&]
[s3; &]
[s5;:Upp`:`:DBusMessage`:`:DBusMessage`(`): [* DBusMessage]()&]
[s2;%% Default constructor. Initializes and empty message.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMessage`:`:DBusMessage`(const String`&`): [* DBusMessage]([@(0.0.255) con
st] String[@(0.0.255) `&] [*@3 rawdata])&]
[s2;%% Constructor overload. Initializes and parses a D`-Bus message 
directly from raw binary socket data.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:DBusMessage`:`:DBusMessage`(const Nuller`&`): [* DBusMessage]([@(0.0.255) con
st] Nuller[@(0.0.255) `&])&]
[s2;%% Null constructor. Constructs a Null message.&]
[s3; &]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Upp`:`:DBusMessage`:`:CreateMethodCall`(dword`,const String`&`,const String`&`,const String`&`,const String`&`,const DBusValueArray`&`): [@(0.0.255) s
tatic] DBusMessage [* CreateMethodCall]([@(0.128.128) dword ][*@3 serial], 
[@(0.0.255) const] String[@(0.0.255) `&] [*@3 dest], [@(0.0.255) const] 
String[@(0.0.255) `&] [*@3 path], [@(0.0.255) const] String[@(0.0.255) `&] 
[*@3 iface], [@(0.0.255) const] String[@(0.0.255) `&] [*@3 method], [@(0.0.255) const] 
DBusValueArray[@(0.0.255) `&] args `= `{`})&]
[s2; Constructs a method call message to invoke a remote procedure. 
[*@3 serial ]is the unique message identifier assigned by the connection 
engine. [*@3 dest ]is the well`-known or unique bus name of the 
receiving service (e.g., [C `"][C@5 org.freedesktop.Notifications][C `"]). 
[*@3 path ]is the object path on the destination. [*@3 iface ]is 
the interface containing the method. [*@3 method ]is the exact 
method name. [*@3 args ][C c]ontains optional payload parameters, 
which are automatically marshaled into the binary message body 
alongside their generated D`-Bus signatures[C .] Returns [@(0.128.128) Null 
]on failure.&]
[s2;%% &]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMessage`:`:CreateMethodReturn`(dword`,dword`,const String`&`,const DBusValueArray`&`): [@(0.0.255) s
tatic] DBusMessage [* CreateMethodReturn]([@(0.128.128) dword ][*@3 serial], 
dword [*@3 replyserial], [@(0.0.255) const] String[@(0.0.255) `&] [*@3 dest], 
[@(0.0.255) const] DBusValueArray[@(0.0.255) `&] args `= `{`})&]
[s2; Constructs a method return message indicating successful execution 
of a remote call. [*@3 serial ]is the unique identifier for this 
new outgoing message. [*@3 replyserial ]must exactly match the 
serial number of the incoming method call being answered. [*@3 dest 
]is the unique bus name of the original caller. [*@3 args ]contains 
the return values to be serialized into the payload body. Returns 
[@(0.128.128) Null ]on failure.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMessage`:`:CreateSignal`(dword`,const String`&`,const String`&`,const String`&`,const DBusValueArray`&`): [@(0.0.255) s
tatic] DBusMessage [* CreateSignal]([@(0.128.128) dword ][*@3 serial], 
[@(0.0.255) const] String[@(0.0.255) `&] [*@3 path], [@(0.0.255) const] 
String[@(0.0.255) `&] [*@3 iface], [@(0.0.255) const] String[@(0.0.255) `&] 
[*@3 name], [@(0.0.255) const] DBusValueArray[@(0.0.255) `&] args `= 
`{`})&]
[s2; Constructs a signal message for bus`-wide broadcasting. [*@3 serial 
]is the unique identifier for this message. [*@3 path ]is the local 
object path emitting the signal. [*@3 iface ]is the interface defining 
the signal. [*@3 name ]is the signal name. [*@3 args ][%% contains 
the optional signal payload. Unlike method calls, signals do 
not specify a destination field. ]Returns [@(0.128.128) Null ]on 
failure.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMessage`:`:CreateError`(dword`,dword`,const String`&`,const String`&`,const String`&`): [@(0.0.255) s
tatic] DBusMessage [* CreateError]([@(0.128.128) dword ][*@3 serial], 
dword [*@3 replyserial], [@(0.0.255) const] String[@(0.0.255) `&] [*@3 dest], 
[@(0.0.255) const] String[@(0.0.255) `&] [*@3 errname], [@(0.0.255) const] 
String[@(0.0.255) `&] [*@3 errmsg])&]
[s2; [%% Constructs an error message rejecting an incoming method call. 
][*@3 serial ][%% is the unique identifier for this message. ][*@3 replyserial 
][%% must exactly match the serial number of the incoming method 
call. ][*@3 dest ][%% is the unique bus name of the original caller. 
][*@3 errname ][%% is the fully qualified D`-Bus error name (e.g., 
`"][%%C@5 org.freedesktop.DBus.Error.UnknownMethod][%% `"). ][*@3 errmsg 
][%% is an optional human`-readable description appended as a single 
string argument in the message body. ]Returns [@(0.128.128) Null 
]on failure.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMessage`:`:IsOK`(`)const: [@(0.0.255) bool] [* IsOK]() 
[@(0.0.255) const]&]
[s2;%% Returns true if the message type is [*/ not ]error.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMessage`:`:IsMethodCall`(`)const: [@(0.0.255) bool] 
[* IsMethodCall]() [@(0.0.255) const]&]
[s2;%% Returns true if the message is a method call.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMessage`:`:IsMethodReturn`(`)const: [@(0.0.255) bool] 
[* IsMethodReturn]() [@(0.0.255) const]&]
[s2;%% Returns true if the message type is a method return.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMessage`:`:IsSignal`(`)const: [@(0.0.255) bool] [* IsSignal]() 
[@(0.0.255) const]&]
[s2;%% Returns true if the message type is a signal.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMessage`:`:IsError`(`)const: [@(0.0.255) bool] [* IsError]() 
[@(0.0.255) const]&]
[s2;%% Returns true if the message type is error.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMessage`:`:IsNullInstance`(`)const: [@(0.0.255) bool] 
[* IsNullInstance]() [@(0.0.255) const]&]
[s2;%% Returns true if the message is in invalid state. This is useful 
for detecting message creation errors. All default constructed 
DBusMessage instances are in invalid (Null) state.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMessage`:`:GetType`(`)const: Type [* GetType]() [@(0.0.255) const]&]
[s2;%% Returns the message type. Can be one of the following: [C@5 METHOD`_CALL], 
[C@5 METHOD`_RETURN], [C@5 ERROR], [C@5 SIGNAL].&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMessage`:`:GetSerial`(`)const: [@(0.128.128) dword 
][* GetSerial]() [@(0.0.255) const]&]
[s2;%% Returns the unique serial number of the message.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMessage`:`:GetHeader`(`)const: [@(0.0.255) const] Header[@(0.0.255) `&] 
[* GetHeader]() [@(0.0.255) const]&]
[s2;%% Returns a constant reference to the parsed 16`-byte fixed 
header structure.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMessage`:`:GetRawData`(`)const: [@(0.0.255) const] 
String[@(0.0.255) `&] [* GetRawData]() [@(0.0.255) const]&]
[s5;:Upp`:`:DBusMessage`:`:operator`~`(`)const:%% [%-@(0.0.255) const][%-  
String][%-@(0.0.255) `&][%-  ][%-@(0.0.255) operator][%- `~() ][%-@(0.0.255) const] 
.&]
[s2;%% Returns a constant reference to the underlying raw binary 
data of the message.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMessage`:`:ParseFields`(`)const: FieldData [* ParseFields]() 
[@(0.0.255) const]&]
[s2;%% Parses and extracts the variable header fields (path, interface, 
member, sender, destination, signature, error, reply.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMessage`:`:ParseBody`(`)const: Vector<DBusValue> 
[* ParseBody]() [@(0.0.255) const]&]
[s2;%% Unmarshals the binary payload body into a vector of typed 
[^topic`:`/`/DBus`/src`/Upp`_DBusValue`_en`-us`#Upp`:`:DBusValue^ DBusValue] 
objects based on the message signature.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMessage`:`:ParseString`(`)const: String [* ParseString]() 
[@(0.0.255) const]&]
[s2;%% Convenience helper to extract the payload of a message that 
strictly consists of a single string (signature `'[C@5 s]`').&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMessage`:`:ParseStringArray`(`)const: DBusValueArray 
[* ParseStringArray]() [@(0.0.255) const]&]
[s2;%% Convenience helper to extract the payload of a message that 
strictly consists of an array of strings (signature `"[C@5 as]`").&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMessage`:`:GetErrorName`(`)const: String [* GetErrorName]() 
[@(0.0.255) const]&]
[s2;%% Returns the D`-Bus error name from the header fields if the 
message is an error type. Otherwise returns an empty string.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMessage`:`:GetErrorDesc`(`)const: String [* GetErrorDesc]() 
[@(0.0.255) const]&]
[s2;%% Returns the human`-readable error description parsed from 
the message body. Falls back to the error name if no description 
payload is provided.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMessage`:`:MatchRule`(const String`&`)const: [@(0.0.255) bool] 
[* MatchRule]([@(0.0.255) const] String[@(0.0.255) `&] [*@3 rule]) [@(0.0.255) const]&]
[s2;%% Evaluates the message against a standard D`-Bus match [%-*@3 rule] 
string (e.g., `"[C@5 type`=`'signal`',interface`=`'org.freedesktop.DBus`']`"). 
Returns true if the message matches the rule parameters.&]
[s3;%% &]
[s0;%% &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 DBusMessage`::Header]]}}&]
[s3;%% &]
[s1;:Upp`:`:DBusMessage`:`:Header: [@(0.0.255)3 struct][3  ][*3 Header]&]
[s2;%% Represents the parsed 16`-byte fixed header of a D`-Bus message.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMessage`:`:Header`:`:endian: [@(0.128.128) byte ][* endian]&]
[s2;%% Endianness flag (`'l`' for little`-endian, `'B`' for big`-endian).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMessage`:`:Header`:`:type: [@(0.128.128) byte ][* type]&]
[s2;%% The message type. Can be one of the following: [C@5 METHOD`_CALL], 
[C@5 METHOD`_RETURN], [C@5 ERROR], [C@5 SIGNAL].&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMessage`:`:Header`:`:flags: [@(0.128.128) byte ][* flags]&]
[s2;%% Bitmask of message flags.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMessage`:`:Header`:`:version: [@(0.128.128) byte ][* version]&]
[s2;%% Major version of the D`-Bus protocol.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMessage`:`:Header`:`:bodylen: [@(0.128.128) dword ][* bodylen]&]
[s2;%% Length of the message payload body in bytes.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMessage`:`:Header`:`:serial: [@(0.128.128) dword ][* serial]&]
[s2;%% The unique serial number of the message..&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMessage`:`:Header`:`:fieldslen: [@(0.128.128) dword 
][* fieldslen]&]
[s2;%% Length of the variable header fields array in bytes.&]
[s3;%% &]
[s3;%% &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 DBusMessage`::FieldData]]}}&]
[s3;%% &]
[s1;:Upp`:`:DBusMessage`:`:FieldData: [@(0.0.255)3 struct][3  ][*3 FieldData]&]
[s2;%% Represents the variable header fields extracted from a D`-Bus 
message.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMessage`:`:FieldData`:`:path: String [* path]&]
[s2;%% The object path of the message target or sender (e.g., `"/com/example/Object`"
).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMessage`:`:FieldData`:`:interface: String [* interface]&]
[s2;%% The interface name of the method call or signal (e.g., `"com.example.Interface
`").&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMessage`:`:FieldData`:`:member: String [* member]&]
[s2;%% The method or signal name.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMessage`:`:FieldData`:`:sender: String [* sender]&]
[s2;%% The unique bus name of the connection that sent the message.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMessage`:`:FieldData`:`:destination: String [* destination]&]
[s2;%% The unique bus name of the connection the message is addressed 
to.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMessage`:`:FieldData`:`:signature: String [* signature]&]
[s2;%% The D`-Bus type signature defining the layout of the message 
payload body (e.g., `"s`", `"a`{sv`}`").&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMessage`:`:FieldData`:`:error: String [* error]&]
[s2;%% The fully qualified error name (applicable only to error type 
messages).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMessage`:`:FieldData`:`:reply: [@(0.128.128) dword 
][* reply]&]
[s2;%% The serial number of the message this message is replying 
to.&]
[s3; &]
[s2;%% ]]