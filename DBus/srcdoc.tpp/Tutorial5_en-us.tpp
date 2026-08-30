topic "Tutorial 5: Building a Server Service";
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
[s2;= Tutorial: Building a Server Service&]
[s5; While previous tutorials focused on client`-side operations 
such as sending notifications, querying active system names, 
or multiplexing broadcast signals advanced desktop integration 
often requires your application to act as a service provider. 
Whether you are building a terminal emulator  that needs to respond 
to external CLI scripts, or a background hardware daemon, D`-Bus 
allows your application to securely expose native functions to 
the rest of the operating system.&]
[s5; In this tutorial, we will explore the architecture of D`-Bus 
server registration, examine how well`-known names are claimed 
on the bus, and assemble a complete U`+`+ server application 
that intercepts remote method calls, unpacks typed arguments, 
and transmits structured replies.&]
[s0; &]
[s3; Table of Contents&]
[s23; &]
[s0; [^topic`:`/`/DBus`/srcdoc`/Tutorial5`_en`-us`#1^ 1. Server`-Side 
D`-Bus Architecture]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial5`_en`-us`#1`.1^ 1.1. Service 
Provision vs. Client Invocation]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial5`_en`-us`#1`.2^ 1.2. Claiming 
Well`-Known Names (RequestName)]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial5`_en`-us`#1`.3^ 1.3. The 
Message Routing and Lifecycle]&]
[s23; [^topic`:`/`/DBus`/srcdoc`/Tutorial5`_en`-us`#2^ 2. Core Concepts 
and Usage Principles]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial5`_en`-us`#2`.1^ 2.1. Intercepting 
Inbound Requests (WhenMethodCall)]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial5`_en`-us`#2`.2^ 2.2. Interface 
and Member Routing Inspection]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial5`_en`-us`#2`.3^ 2.3. Crafting 
and Returning Synchronous Replies (SendReply)]&]
[s23; [^topic`:`/`/DBus`/srcdoc`/Tutorial5`_en`-us`#3^ 3. Assembling 
the Demo]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial5`_en`-us`#3`.1^ 3.1. Establishing 
the Session Connection]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial5`_en`-us`#3`.2^ 3.2. Registering 
the Service Name]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial5`_en`-us`#3`.3^ 3.3. Configuring 
the Method Dispatch Hook]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial5`_en`-us`#3`.4^ 3.4. Filtering 
and Parsing Incoming Payloads]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial5`_en`-us`#3`.5^ 3.5. Executing 
Local Logic and Replying]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial5`_en`-us`#3`.6^ 3.6. Initiating 
the Server Event Listener]&]
[s23; &]
[s3;:1:%- 1. Server`-Side D`-Bus Architecture&]
[s5;%- Before diving into code, it is important to understand how 
server`-side hosting works within the D`-Bus message broker ecosystem.&]
[s22;:1`.1:%- 1.1. Service Provision vs. Client Invocation&]
[s5;%- In client mode, your application initiates connections and 
sends out packets. In server mode, your application transforms 
into an IPC endpoint. It sits idly on the message bus socket, 
waiting for external processes to target its unique address space.&]
[s22;:1`.2:%- 1.2. Claiming Well`-Known Names ([C RequestName])&]
[s5;%- When a D`-Bus application starts, the system assigns it a 
temporary unique connection name (e.g., [C@5 :1.48]). Because client 
applications cannot predict this dynamic identifier, services 
claim a human`-readable [* Well`-Known Name] (such as [C@5 com.bobcat.Terminal]). 
When an external script or app invokes a method using that well`-known 
name, the central [C dbus`-daemon] resolves it and routes the packet 
straight to your application`'s socket.&]
[s22;:1`.3:%- 1.3. The Message Routing and Lifecycle&]
[s5;%- When an inbound message arrives at the server socket, the 
D`-Bus engine parses its routing headers (destination, path, 
interface, and member). Instead of handling raw byte streams, 
the connection object exposes high`-level callback hooks that 
allow your application to inspect the request, execute internal 
business logic, and dispatch a structured reply back to the caller`'s 
unique serial number.&]
[s0;%- &]
[s3;:2:%- 2. Core Concepts and Usage Principles&]
[s5;%- Building a responsive D`-Bus server relies on three primary 
data management mechanics within the U`+`+ framework.&]
[s22;:2`.1:%- 2.1. Intercepting Inbound Requests ([C WhenMethodCall])&]
[s5;%- The server connection exposes a functional hook named [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:WhenMethodCall^ W
henMethodCall]. Whenever an external client transmits a method 
call matching your service ownership, the engine automatically 
catches the frame and triggers this lambda or callback, passing 
the raw [C^topic`:`/`/DBus`/src`/Upp`_DBusMessage`_en`-us`#Upp`:`:DBusMessage^ DBus
Message] for inspection.&]
[s22;:2`.2:%- 2.2. Interface and Member Routing Inspection&]
[s5;%- Because a single service can host multiple objects and interfaces, 
the server must filter incoming requests. Using [^topic`:`/`/DBus`/src`/Upp`_DBusMessage`_en`-us`#Upp`:`:DBusMessage`:`:ParseFields`(`)const^ P
][C^topic`:`/`/DBus`/src`/Upp`_DBusMessage`_en`-us`#Upp`:`:DBusMessage`:`:ParseFields`(`)const^ a
rseFields()] method, the application retrieves structural metadata, 
specifically verifying that the [C^topic`:`/`/DBus`/src`/Upp`_DBusMessage`_en`-us`#Upp`:`:DBusMessage`:`:FieldData`:`:interface^@5 i
nterface][^topic`:`/`/DBus`/src`/Upp`_DBusMessage`_en`-us`#Upp`:`:DBusMessage`:`:FieldData`:`:interface^@5  
]matches your expected component and the [C^topic`:`/`/DBus`/src`/Upp`_DBusMessage`_en`-us`#Upp`:`:DBusMessage`:`:FieldData`:`:member^ m
ember][^topic`:`/`/DBus`/src`/Upp`_DBusMessage`_en`-us`#Upp`:`:DBusMessage`:`:FieldData`:`:member^  
]matches the requested function name (e.g., [C OpenTab]).&]
[s22;:2`.3:%- 2.3. Crafting and Returning Synchronous Replies ([C SendReply])&]
[s5;%- D`-Bus method calls expect closure. Once the server executes 
its internal logic, it must signal completion to the waiting 
client. Using [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:SendReply`(const DBusMessage`&`,const DBusValueArray`&`)^ S
endReply()], the server bundles return parameters (such as unique 
IDs or status strings) and links them directly to the original 
message`'s transaction ID.&]
[s0;%- &]
[s3;:3:%- 3. Assembling the Demo&]
[s5;%- With the architectural principles established, let`'s assemble 
the D`-Bus server application step`-by`-step.&]
[s22;:3`.1:%- 3.1. Initializing and Connecting to the Bus&]
[s5;%- We begin by setting up standard logging and establishing a 
connection to the user`'s Session Bus.&]
[s7;%- #include[@0  ][@(0.0.255) <][@0 DBus][@(0.0.255) /][@0 DBus][@(0.0.255) .][@0 h][@(0.0.255) >]&]
[s7;%- &]
[s7;%- using[@0  ]namespace[@0  Upp;]&]
[s7;%- &]
[s7;%- CONSOLE`_APP`_MAIN&]
[s7;%- `{&]
[s7;%-     StdLogSetup(LOG`_COUT [@(0.0.255) `|][@0  LOG`_FILE);]&]
[s7;%-     DBusConnection dbus;&]
[s7;%- &]
[s7;%-     [@4 RLOG][@0 (][@3 `"Bobcat D`-Bus Daemon`"][@0 );]&]
[s7;%- &]
[s7;%-     [/@4 // Establish socket connection to the local session 
bus]&]
[s7;%-     [*@(0.0.255) if][@0 (][@(0.0.255) !][@0 dbus][@(0.0.255) .][@0 ConnectSession())]&]
[s7;%-         [*_@(128.0.255) return];&]
[s22;:3`.2:%- 3.2. Claiming the Well`-Known Service Name&]
[s5;%- Before our application can receive remote traffic, it must 
claim its identity on the network. By calling [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:RequestName`(const String`&`)^ R
equestName()] we tell the central daemon that our application 
now exclusively owns this namespace.&]
[s7;%- // Claim our well`-known service name on the bus&]
[s7;%- if[@0 (]![@0 dbus].[@0 RequestName(][@3 `"com.bobcat.Terminal`"][@0 ))]&]
[s7;%-     [*_@(128.0.255) return][@0 ;]&]
[s7;%- &]
[s7;%- RLOG[@0 (][@3 `"Service com.bobcat.Terminal registered and listening!`"][@0 );]&]
[s22;:3`.3:%- 3.3. Configuring the Method Dispatch Hook&]
[s5;%- We bind a lambda function to [^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:WhenMethodCall^ W
henMethodCall]. This handler acts as the central router for all 
incoming remote procedure calls directed at our service.&]
[s7;%- // Handle incoming remote method calls asynchronously via 
callback dispatch&]
[s7;%- dbus[@(0.0.255) .][@0 WhenMethodCall ][@(0.0.255) `=][@0  ][@(0.0.255) `[`&`]][@0 (][*@(0.0.255) c
onst][@0  DBusMessage][@(0.0.255) `&][@0  msg) `{]&]
[s7;%-     DBusMessage[@(0.0.255) `::]FieldData fd [@(0.0.255) `=] msg[@(0.0.255) .]ParseFie
lds();&]
[s22;:3`.4:%- 3.4. Filtering and Parsing Incoming Payloads&]
[s5;%- Inside the dispatch handler, we inspect the interface and 
member fields. If a client invokes the OpenTab method under our 
control interface, we parse the incoming message body to extract 
arguments (such as a working directory path).&]
[s7;%- if[@0 (fd].[@0 interface ]`=`=[@0  ][@3 `"com.bobcat.Terminal.Control`"][@0  
]`&`&[@0  fd].[@0 member ]`=`=[@0  ][@3 `"OpenTab`"][@0 ) `{]&]
[s7;%-             [/@4 // Extract argument payload from the inbound 
message body]&]
[s7;%-             DBusValueArray args [@(0.0.255) `=][@0  msg][@(0.0.255) .][@0 ParseBody();
]&]
[s7;%-             String path [@(0.0.255) `=][@0  args][@(0.0.255) .][@0 GetCount() 
][@(0.0.255) >][@0  ][@3 0][@0  ][@(0.0.255) ?][@0  args][@(0.0.255) `[][@3 0][@(0.0.255) `].][@0 ToS
tring() ][@(0.0.255) :][@0  ][@3 `"`~/`"][@0 ;]&]
[s7;%- &]
[s7;%-             [@4 RLOG]([@3 `"Incoming request: Opening new tab at 
path: `"] [@(0.0.255) <<] path);&]
[s22;:3`.5:%- 3.5. Executing Local Logic and Replying&]
[s5;%- We execute our application`-level logic (simulating the creation 
of a new terminal tab which yields an ID of 42) and use [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:SendReply`(const DBusMessage`&`,const DBusValueArray`&`)^ S
endReply()][C  ]to transmit the success parameters back to the 
calling client.&]
[s7;%-         [/@4 // Execute the actual U`+`+ application logic...]&]
[s7;%-         [*@(0.0.255) int][@0  new`_tab`_id ][@(0.0.255) `=][@0  ][@3 42][@0 ;]&]
[s7;%- &]
[s7;%-         [/@4 // Transmit the structured result back to the original 
caller]&]
[s7;%-         dbus[@(0.0.255) .][@0 SendReply(msg, `{ new`_tab`_id, 
][@3 `"Tab successfully created`"][@0  `});]&]
[s7;%-     `}&]
[s7;%-     [*@(0.0.255) else][@0  `{]&]
[s7;%-         [@4 RLOG][@0 (][@3 `"Unhandled method call: `"][@0  ][@(0.0.255) <<][@0  
fd][@(0.0.255) .][@0 member);]&]
[s7;%-         [/@4 // In a production environment, you would invoke 
a D`-Bus Error reply here]&]
[s7;%-     `}&]
[s7;%- `};&]
[s22;:3`.6:%- 3.6. Initiating the Server Event Listener&]
[s5;%- Finally, we put the connection into listening mode. By calling 
[^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:Listen`(`)^ L
isten()], the application enters a blocking event loop, keeping 
the daemon alive and responsive to incoming socket activity.&]
[s7;%- // Keep the daemon alive and processing inbound socket events 
with a 10`-second check cycle&]
[s7;%-     dbus[@(0.0.255) .][@0 Timeout(][@3 10000][@0 )][@(0.0.255) .][@0 Listen();]&]
[s7;%- `}]]