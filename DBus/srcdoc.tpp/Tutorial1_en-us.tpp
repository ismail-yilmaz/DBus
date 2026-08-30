topic "Tutorial 1: Method Calling";
[l288;i1120;a17;O9;~~~.1408;2 $$1,0#10431211400427159095818037425705:param]
[a83;*R6 $$2,5#31310162474203024125188417583966:caption]
[H4;b83;*4 $$3,5#07864147445237544204411237157677:title]
[i288;O9;C2 $$4,6#40027414424643823182269349404212:item]
[b42;a42;ph2 $$5,5#45413000475342174754091244180557:text]
[l288;b17;a17;2 $$6,6#27521748481378242620020725143825:desc]
[l321;C@5+75 $$7,7#20902679421464641399138805415013:code]
[b2503;2 $$8,0#65142375456100023862071332075487:separator]
[*@(0.0.255)2 $$9,0#83433469410354161042741608181528:base]
[C2 $$10,0#37138531426314131251341829483380:class]
[l288;a17;*1 $$11,11#70004532496200323422659154056402:requirement]
[i417;b42;a42;O9;~~~.416;2 $$12,12#10566046415157235020018451313112:tparam]
[b167;C2 $$13,13#92430459443460461911108080531343:item1]
[i288;a42;O9;C2 $$14,14#77422149456609303542238260500223:item2]
[*@2$(0.128.128)2 $$15,15#34511555403152284025741354420178:NewsDate]
[l321;*C$7;2 $$16,16#03451589433145915344929335295360:result]
[l321;b83;a83;*C$7;2 $$17,17#07531550463529505371228428965313:result`-line]
[l160;*C+117 $$18,5#88603949442205825958800053222425:package`-title]
[2 $$19,0#53580023442335529039900623488521:gap]
[C2 $$20,20#70211524482531209251820423858195:class`-nested]
[b50;2 $$21,21#03324558446220344731010354752573:Par]
[b83;* $$22,22#78EB85B566C7E078B53494D0FD53D992:subtitle]
[2 $$23,23#1BDAF836AD481778AD91861722C13AB2:toc]
[2 $$0,0#00000000000000000000000000000000:Default]
[{_}%EN-US 
[s2;= Tutorial: Method Calling&]
[s5; While modern C`+`+ applications rely heavily on local, in`-memory 
object method invocations, interacting with the underlying Linux 
desktop environment or system services requires Inter`-Process 
Communication (IPC). With the U`+`+ DBus package, you can query, 
control, and communicate with external daemons entirely natively, 
eliminating the need for cumbersome system shell calls or external 
bindings.&]
[s5; In this tutorial, we will explore the core architecture of the 
D`-Bus message broker, examine how service addressing works, 
and assemble a complete U`+`+ console application that queries 
the message bus to list all actively registered application names.&]
[s3; Table of Contents&]
[s23; &]
[s0; [^topic`:`/`/DBus`/srcdoc`/Tutorial1`_en`-us`#1^ 1. DBus Architecture]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial1`_en`-us`#1`.1^ 1.1. Inter`-Process 
Communication (IPC) Broker]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial1`_en`-us`#1`.2^ 1.2. The 
System and Session Buses]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial1`_en`-us`#1`.3^ 1.3. Service 
Addressing (The 4`-Part Route)]&]
[s23; [^topic`:`/`/DBus`/srcdoc`/Tutorial1`_en`-us`#2^ 2. Core Concepts 
and Usage Principles]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial1`_en`-us`#2`.1^ 2.1. Connection 
Management]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial1`_en`-us`#2`.2^ 2.2. Synchronous 
Method Calls]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial1`_en`-us`#2`.3^ 2.3. Message 
Validation and Payload Parsing]&]
[s23; [^topic`:`/`/DBus`/srcdoc`/Tutorial1`_en`-us`#3^ 3. Assembling 
the Demo]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial1`_en`-us`#3`.1^ 3.1. Initialize 
the Application and Connect]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial1`_en`-us`#3`.2^ 3.2. Formulate 
and Dispatch the Method Call]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial1`_en`-us`#3`.3^ 3.3. Retrieve 
and Validate the Response]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial1`_en`-us`#3`.4^ 3.4. Extract 
the Payload]&]
[s23;    [^topic`:`/`/DBus`/srcdoc`/Tutorial1`_en`-us`#3`.5^  3.5. Handle 
Connection Errors]&]
[s23; &]
[s3;:1:%- 1. DBus Architecture&]
[s5;%- Before diving into code, it is important to understand how 
D`-Bus operates compared to traditional function calls.&]
[s22;:1`.1:%- 1.1. Inter`-Process Communication (IPC) Broker&]
[s5;%- Unlike linking against a shared library where your application 
executes code directly, D`-Bus operates through a background 
message broker daemon ([C dbus`-daemon]). When your application 
wants to trigger an action in another program (like a media player 
or a desktop notification), it serializes a binary message and 
sends it over a local socket to the broker. The broker then routes 
that message to the target application and routes the reply back 
to you.&]
[s22;:1`.2:%- 1.2. The System and Session Buses&]
[s5;%- The D`-Bus architecture is divided into two primary networks. 
The [* System Bus] is a single, heavily restricted bus used by 
the OS for system`-wide events (e.g., hardware detection, network 
changes). The [* Session Bus] is unique to your current user login 
session; it is where desktop applications, UI services, and user 
daemons communicate. In this tutorial, we will be interacting 
with the Session Bus.&]
[s22;:1`.3:%- 1.3. Service Addressing (The 4`-Part Route)&]
[s5;%- To successfully route a message across the bus, the broker 
requires a specific 4`-part address:&]
[s5;i150;O0;%- [* Destination]: The registered name of the target application 
(e.g.,[C@5  org.freedesktop.DBus]).&]
[s5;i150;O0;%- [* Object Path]: The specific virtual object within that 
application handling the request (e.g., [C@5 /org/freedesktop/DBus]).&]
[s5;i150;O0;%- [* Interface]: The namespace of the method being called 
(e.g., [C@5 org.freedesktop.DBus]).&]
[s5;i150;O0;%- [* Member]: The actual function name to execute (e.g., 
[C@5 ListNames]).&]
[s0;%- &]
[s3;:2:%- 2. Core Concepts and Usage Principles&]
[s5;%- Building a functional D`-Bus request relies on three primary 
data management steps within U`+`+.&]
[s22;:2`.1:%- 2.1. Connection Management&]
[s5;%- The [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection^ D
BusConnection] class acts as the bridge between your application 
and the OS socket. It handles authentication, socket initialization, 
and binary stream framing automatically.&]
[s22;:2`.2:%- 2.2. Synchronous Method Calls&]
[s5;%- By default, the [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:MethodCall`(const String`&`,const String`&`,const String`&`,const String`&`,const DBusValueArray`&`)^ M
ethodCall()] method operates synchronously. When invoked, it 
serializes your parameters, dispatches the request to the daemon, 
and pauses execution until the destination service replies (or 
a timeout occurs). This makes writing linear, procedural IPC 
logic straightforward.&]
[s22;:2`.3:%- 2.3. Message Validation and Payload Parsing&]
[s5;%- When the broker returns the target`'s response, it is wrapped 
in a [C^topic`:`/`/DBus`/src`/Upp`_DBusMessage`_en`-us`#Upp`:`:DBusMessage^ DBusMes
sage] object. Because D`-Bus is strictly typed via signatures 
(e.g., [C as] for an array of strings), the engine provides specialized 
parsers like [C^topic`:`/`/DBus`/src`/Upp`_DBusMessage`_en`-us`#Upp`:`:DBusMessage`:`:ParseStringArray`(`)const^ P
arseStringArray()] to safely unpack the binary payload directly 
into native U`+`+ containers like [C@5 Vector<String>].&]
[s0;%- &]
[s3;:3:%- 3. Assembling the Demo&]
[s5;%- With the architectural principles established, let`'s assemble 
the console application step`-by`-step.&]
[s22;:3`.1:%- 3.1. Initialize the Application and Connect&]
[s5;%- We begin by setting up standard U`+`+ logging and instantiating 
our connection object. We then instruct the object to attach 
to the user`'s Session Bus.&]
[s7;%- #include[@0  ][@(0.0.255) <][@0 DBus][@(0.0.255) /][@0 DBus][@(0.0.255) .][@0 h][@(0.0.255) >]&]
[s7;%- &]
[s7;%- using[@0  ]namespace[@0  Upp;]&]
[s7;%- &]
[s7;%- CONSOLE`_APP`_MAIN&]
[s7;%- `{&]
[s7;%-     StdLogSetup(LOG`_COUT [@(0.0.255) `|][@0  LOG`_FILE);]&]
[s7;%- &]
[s7;%-     DBusConnection dbus;&]
[s7;%- &]
[s7;%-     [@4 RLOG][@0 (][@3 `"Connecting to D`-Bus...`"][@0 );]&]
[s7;%-     [/@4 // Attempt to establish a socket connection to the 
local Session Bus broker]&]
[s7;%-     [*@(0.0.255) if][@0 (dbus][@(0.0.255) .][@0 ConnectSession()) 
`{]&]
[s7;%-         [@4 RLOG]([@3 `"Requesting active bus names...`"]);&]
[s22;:3`.2:%- 3.2. Formulate and Dispatch the Method Call&]
[s5;%- Once connected, we dispatch our method call. We are asking 
the D`-Bus daemon itself ([C@5 org.freedesktop.DBus]) to execute 
its built`-in [C@5 ListNames]&]
[s7;%- [/@4 // Dispatch the 4`-part routing address to the broker]&]
[s7;%- [*@(0.0.255) if](dbus[@(0.0.255) .]MethodCall([@3 `"org.freedesktop.DBus`"], 
[@3 `"/org/freedesktop/DBus`"], [@3 `"org.freedesktop.DBus`"], [@3 `"ListNames`"])) 
`{&]
[s22;:3`.3:%- 3.3. Retrieve and Validate the Response&]
[s5;%- Because [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:MethodCall`(const String`&`,const String`&`,const String`&`,const String`&`,const DBusValueArray`&`)^ M
ethodCall()] blocks until the transaction is complete, we can 
immediately retrieve the reply using [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:GetMessage`(`)const^ G
etMessage()]. We must verify that the message is valid ([C^topic`:`/`/DBus`/src`/Upp`_DBusMessage`_en`-us`#Upp`:`:DBusMessage`:`:IsOK`(`)const^ I
sOK()][C )] and not an error packet returned by the daemon.&]
[s7;%- // Capture the reply from the broker and ensure no IPC transmission 
errors occurred&]
[s7;%- if[@0 (]const[@0  DBusMessage]`&[@0  msg ]`=[@0  dbus].[@0 GetMessage(); 
msg].[@0 IsOK()) `{]&]
[s22;:3`.4:%- 3.4. Extract the Payload&]
[s5;%- Knowing that the [C@5 ListNames][@5  ]method returns a D`-Bus 
string array ([C as]), we use the engine`'s built`-in [C^topic`:`/`/DBus`/src`/Upp`_DBusMessage`_en`-us`#Upp`:`:DBusMessage`:`:ParseStringArray`(`)const^ P
arseStringArray()] helper. This cleanly extracts the payload 
into a  [C@5 Vector<String>]&]
[s0;%- [C0                 ][/C@4;0 // Unpack the binary payload directly 
into a native U`+`+ string vector]&]
[s7;%-                 Vector[@(0.0.255) <]String[@(0.0.255) >] names 
[@(0.0.255) `=] msg[@(0.0.255) .]ParseStringArray();&]
[s7;%-                 &]
[s7;%-                 [@4 RLOG][@0 (][@3 `"Active names: `"][@0  ][@(0.0.255) <<][@0  
names][@(0.0.255) .][@0 GetCount());]&]
[s7;%-                 &]
[s7;%-                 [/@4 // Iterate through the vector and log each 
registered application on the bus]&]
[s7;%-                 [*@(0.0.255) for][@0 (][*@(0.0.255) const][@0  String][@(0.0.255) `&][@0  
s ][@(0.0.255) :][@0  names)]&]
[s7;%-                     [@4 RLOG][@0 (s);]&]
[s7;%-             `}&]
[s7;%-             [*@(0.0.255) else]&]
[s7;%-                 [@4 RLOG][@0 (][@3 `"Method Call Error: `"][@0  ][@(0.0.255) <<][@0  
msg][@(0.0.255) .][@0 GetErrorName());]&]
[s7;%-         `}&]
[s7;%-         [*_@(128.0.255) return][@0 ;]&]
[s7;%-     `}&]
[s22;:3`.5:%- 3.5. Handle Connection Errors&]
[s5;%- Finally, we provide a fallback in case the initial socket 
connection to the Session Bus fails (which could happen if the 
application is run in an environment without a desktop session, 
like a headless server.&]
[s7;%-     [/@4 // Log the underlying socket or authentication error 
if connection failed]&]
[s7;%-     [@4 RLOG][@0 (][@3 `"D`-Bus operation failed: `"][@0  ][@(0.0.255) <<][@0  
dbus][@(0.0.255) .][@0 GetErrorDesc());]&]
[s7;%- `}&]
[s5;%- ]]