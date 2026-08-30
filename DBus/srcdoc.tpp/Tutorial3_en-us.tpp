topic "Tutorial 3: Global Multiplexing and Async Event Loops";
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
[s2;= Tutorial: Global Multiplexing and Async Event Loops&]
[s5; While synchronous method calls are ideal for linear initialization 
and straightforward queries, modern desktop applications cannot 
afford to block their execution threads waiting for remote services 
to reply. True desktop integration requires a reactive, event`-driven 
architecture capable of listening to asynchronous system broadcasts, 
such as hardware hot`-plugging, power state changes, or application 
focus notifications, without freezing the UI or application logic.&]
[s5; With the U`+`+ DBus package, you can configure global signal 
subscriptions, bind custom event callbacks, and integrate native 
socket polling directly into an asynchronous event loop.&]
[s5; &]
[s3; Table of Contents&]
[s23; &]
[s0; [^topic`:`/`/DBus`/srcdoc`/Tutorial3`_en`-us`#1^ 1. Asynchronous 
D`-Bus Architecture]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial3`_en`-us`#1`.1^ 1.1. Synchronous 
Blocking vs. Event`-Driven Multiplexing]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial3`_en`-us`#1`.2^ 1.2. Server`-Side 
Filtering via Match Rules]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial3`_en`-us`#1`.3^ 1.3. Socket 
Polling and Event Integration (SocketWaitEvent)]&]
[s23; [^topic`:`/`/DBus`/srcdoc`/Tutorial3`_en`-us`#2^ 2. Core Concepts 
and Usage Principles]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial3`_en`-us`#2`.1^ 2.1. Dynamic 
Subscription Management (AddMatch)]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial3`_en`-us`#2`.2^ 2.2. Event 
Routing via Callbacks (WhenSignal)]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial3`_en`-us`#2`.3^ 2.3. Non`-Blocking 
Pump Control (Do)]&]
[s23; [^topic`:`/`/DBus`/srcdoc`/Tutorial3`_en`-us`#3^ 3. Assembling 
the Demo]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial3`_en`-us`#3`.1^ 3.1. Establishing 
the Session Connection]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial3`_en`-us`#3`.2^ 3.2. Registering 
the Global Match Rule]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial3`_en`-us`#3`.3^ 3.3. Binding 
the Incoming Signal Callback]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial3`_en`-us`#3`.4^ 3.4. Configuring 
Non`-Blocking Listener Mode]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial3`_en`-us`#3`.5^ 3.5. Integrating 
with U`+`+ Socket Multiplexing]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial3`_en`-us`#3`.6^ 3.6. Executing 
the Real`-Time Event Loop]&]
[s23; &]
[s3;:1:%- 1. Asynchronous D`-Bus Architecture&]
[s5;%- Before diving into code, it is important to understand how 
asynchronous socket multiplexing differs from standard request`-response 
calls.&]
[s22;:1`.1:%- 1.1. Synchronous Blocking vs. Event`-Driven Multiplexing&]
[s5;%- In a synchronous design, when your application sends a method 
call, execution halts until the transport layer receives a reply 
packet. In contrast, an asynchronous event`-driven architecture 
keeps communication channels open. The application registers 
interest in specific event types and enters a low`-overhead polling 
state, waking up only when raw bytes arrive on the underlying 
socket descriptor.&]
[s22;:1`.2:%- 1.2. Server`-Side Filtering via Match Rules&]
[s5;%- The D`-Bus message broker handles an immense volume of broadcast 
traffic across the system or session bus. If every listening 
application processed every message, CPU overhead would spike 
instantly. To prevent this, the D`-Bus protocol uses [* Match Rules]. 
By transmitting a rule string (e.g., [C@5 type`=`'signal`']), the 
client instructs the broker`'s server`-side router to filter 
messages, forwarding only those that match the specified criteria 
to the client`'s socket.&]
[s22;:1`.3:%- 1.3. Socket Polling and Event Integration ([C SocketWaitEvent])&]
[s5;%- U`+`+ provides robust socket abstraction utilities. [C^topic`:`/`/Core`/src`/SocketWaitEvent`_en`-us`#SocketWaitEvent`:`:class^ S
ocketWaitEvent] interfaces directly with OS`-level polling mechanisms 
(such as [C@5 poll][@5  ]or [C@5 select]), allowing the application 
to sleep efficiently until network activity occurs. By binding 
the [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection^ DB
usConnection] instance to this wait event, the application achieves 
zero`-CPU`-usage idling while remaining instantly reactive to 
incoming IPC packets.&]
[s0;%- &]
[s3;:2:%- 2. Core Concepts and Usage Principles&]
[s5;%- Building a real`-time event listener relies on three primary 
data management mechanics within the U`+`+ D`-Bus engine.&]
[s22;:2`.1:%- 2.1. Dynamic Subscription Management ([C AddMatch])&]
[s5;%- Subscriptions are initialized at runtime by calling [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:AddMatch`(const String`&`,Event`)^ A
ddMatch()]. This sends a control message to the central D`-Bus 
daemon asking it to unlock routing paths for specific signal 
types, interfaces, or object paths.&]
[s22;:2`.2:%- 2.2. Event Routing via Callbacks ([C WhenSignal])&]
[s5;%- To handle incoming asynchronous broadcasts without cluttering 
the main thread, the connection object exposes a [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:WhenSignal^ W
henSignal] functional hook. Whenever a matching signal crosses 
the wire, the engine automatically deserializes the packet and 
invokes this callback, passing the parsed [C^topic`:`/`/DBus`/src`/Upp`_DBusMessage`_en`-us`#Upp`:`:DBusMessage^ D
BusMessage] for inspection.&]
[s22;:2`.3:%- 2.3. Non`-Blocking Pump Control ([C Do])&]
[s5;%- Switching the connection into non`-blocking mode via [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:NonBlocking`(bool`)^ .
NonBlocking()][C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:Listen`(`)^ .
Listen()] alters how internal packet buffers are flushed. Combined 
with the event loop pump [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:Do`(`)^ D
o()] method, the engine processes incoming queues chunk`-by`-chunk, 
returning control back to the application loop between cycles.&]
[s0;%- &]
[s3;:3:%- 3. Assembling the Demo&]
[s5;%- With the architectural principles established, let`'s assemble 
the console application step`-by`-step.&]
[s22;:3`.1:%- 3.1. Establishing the Session Connection&]
[s5;%- We begin by initializing standard logging and establishing 
a connection to the user`'s local Session Bus.&]
[s7;%- using[@0  ]namespace[@0  Upp;]&]
[s7;%- &]
[s7;%- CONSOLE`_APP`_MAIN&]
[s7;%- `{&]
[s7;%-     StdLogSetup(LOG`_COUT [@(0.0.255) `|][@0  LOG`_FILE);]&]
[s7;%- &]
[s7;%-     DBusConnection dbus;&]
[s7;%- &]
[s7;%-     [@4 RLOG][@0 (][@3 `"Connecting to D`-Bus...`"][@0 );]&]
[s7;%-     [*@(0.0.255) if][@0 (][@(0.0.255) !][@0 dbus][@(0.0.255) .][@0 ConnectSession()) 
`{]&]
[s7;%-         [@4 RLOG][@0 (][@3 `"Connection failed.`"][@0 );]&]
[s7;%-         [*_@(128.0.255) return][@0 ;]&]
[s7;%-     `}&]
[s22;:3`.2:%- 3.2. Registering the Global Match Rule&]
[s5;%- Once connected, we transmit a match rule to the daemon. By 
specifying [C@5 type`=`'signal`'][C  ]we instruct the message broker 
to stream every broadcast signal occurring across the session 
bus directly to our application socket.&]
[s7;%- // Transmit the rule over the wire to subscribe to all broadcast 
signals&]
[s7;%- if[@0 (dbus].[@0 AddMatch(][@3 `"type`=`'signal`'`"][@0 ))]&]
[s7;%-     [@4 RLOG]([@3 `"Global signal subscription successful.`"]);&]
[s22;:3`.3:%- 3.3. Binding the Incoming Signal Callback&]
[s5;%- We assign a lambda function to [C dbus.][C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:WhenSignal^ W
henSignal][C .]This callback serves as our global routing funnel. 
Whenever any application on the session bus fires a signal, our 
lambda executes, parses the header fields (interface and member 
name), and prints them to the console.&]
[s7;%- // Attach the fallback callback to handle generic incoming 
signals asynchronously&]
[s7;%-  dbus[@(0.0.255) .][@0 WhenSignal ][@(0.0.255) `=][@0  ][@(0.0.255) `[`]][@0 (][*@(0.0.255) c
onst][@0  DBusMessage][@(0.0.255) `&][@0  msg) `{]&]
[s7;%-      DBusMessage[@(0.0.255) `::][@0 FieldData fd ][@(0.0.255) `=][@0  
msg][@(0.0.255) .][@0 ParseFields();]&]
[s7;%-      [@4 RLOG][@0 (][@3 `"`[GLOBAL IN`] `"][@0  ][@(0.0.255) <<][@0  fd][@(0.0.255) .][@0 inte
rface ][@(0.0.255) <<][@0  ][@3 `"`::`"][@0  ][@(0.0.255) <<][@0  fd][@(0.0.255) .][@0 member);]&]
[s7;%-  `};&]
[s22;:3`.4:%- 3.4. Configuring Non`-Blocking Listener Mode&]
[s5;%- We transition the connection into asynchronous listener mode. 
This prepares the underlying socket file descriptors for non`-blocking 
operations.&]
[s7;%- // Begin async non`-blocking listening mode&]
[s7;%- dbus[@(0.0.255) .][@0 NonBlocking()][@(0.0.255) .][@0 Listen();]&]
[s7;%- RLOG([@3 `"Listening for events. Open an application or plug 
in a USB...`"][@0 );]&]
[s22;:3`.5:%- 3.5. Integrating with U`+`+ Socket Multiplexing&]
[s5;%- To prevent our console application from spinning at 100% CPU 
usage while waiting for events, we instantiate a [C^topic`:`/`/Core`/src`/SocketWaitEvent`_en`-us`#SocketWaitEvent`:`:class^ S
ocketWaitEvent] and register our D`-Bus connection with it.&]
[s7;%- // Create a socket polling container for efficient multiplexing&]
[s7;%- SocketWaitEvent we;&]
[s7;%- dbus[@(0.0.255) .]AddTo(we);&]
[s22;:3`.6:%- 3.6. Executing the Real`-Time Event Loop&]
[s5;%- Finally, we construct our main processing loop. The loop evaluates 
[C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:Do`(`)^ D
o()] to drain active packet queues, while we.[C^topic`:`/`/Core`/src`/SocketWaitEvent`_en`-us`#SocketWaitEvent`:`:Wait`(int`)^ W
ait(1000)] puts the thread to sleep for up to one second if no 
network events occur, ensuring an ultra`-lightweight execution 
footprint.&]
[s7;%-     [/@4 // Run the event loop, polling for socket activity 
with a 1`-second timeout]&]
[s7;%-     [*@(0.0.255) while][@0 (dbus][@(0.0.255) .][@0 Do())]&]
[s7;%-         we[@(0.0.255) .][@0 Wait(][@3 1000][@0 ); ]&]
[s7;%- `}]]