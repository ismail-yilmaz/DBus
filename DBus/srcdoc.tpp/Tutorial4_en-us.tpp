topic "Tutorial 4: Sending Desktop Notifications";
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
[s2;= Tutorial: Sending Desktop Notifications&]
[s5; This tutorial demonstrates how to connect to the D`-Bus Session 
Bus and interact with the standard FreeDesktop Notification daemon. 
We will build a fluent helper class to safely encapsulate complex 
D`-Bus signatures (specifically variant dictionaries and arrays) 
using our native U`+`+ integration.&]
[s3; Table of Contents&]
[s23; &]
[s23; [^topic`:`/`/DBus`/srcdoc`/Tutorial2`_en`-us`#1^ 1. Core Architecture 
and Principles]&]
[s23; [^topic`:`/`/DBus`/srcdoc`/Tutorial2`_en`-us`#2^ 2. Practical 
Usage Patterns]&]
[s23; [^topic`:`/`/DBus`/srcdoc`/Tutorial2`_en`-us`#3^ 3. Step by Step 
Code Assembly]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial2`_en`-us`#3`.1^ 3.1. Defining 
the Class Shell and State Variables]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial2`_en`-us`#3`.2^ 3.2. Building 
the Setters]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial2`_en`-us`#3`.3^ 3.3. Constructing 
the D`-Bus Payload (The Send Method)]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial2`_en`-us`#3`.4^ 3.4. Parsing 
the Daemon`'s Response]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial2`_en`-us`#3`.5^ 3.5. The 
Main Execution Loop]&]
[s23; &]
[s3;:1: 1. Core Architecture and Principles&]
[s5;%- Unlike standard local function calls or rendering a UI control 
locally, D`-Bus relies on Inter`-Process Communication (IPC). 
To show a desktop notification on a Linux desktop environment 
(like GNOME, KDE, or XFCE), our application does not link against 
a GUI library to draw a window. Instead, we serialize a standardized 
message and transmit it over a socket to a background daemon 
([C@5 org.freedesktop.Notifications]).&]
[s5;%- The FreeDesktop specification for notifications requires a 
strict method signature: [C@5 susssasa`{sv`}i]. Let`'s break down 
what the daemon expects to receive over the wire:&]
[s5;i150;O0;%- [@5 s] `- [* Application Name] (String)&]
[s5;i150;O0;%- [@5 u ]`- [* Replaces ID] ([@(0.128.128) uint32]). If 0, 
a new notification bubble is created. If > 0, the daemon updates 
the existing bubble matching that ID.&]
[s5;i150;O0;%- [@5 s ]`- [* Notification Icon] (String). Can be a standard 
theme icon (e.g., `"utilities`-terminal`") or a file path.&]
[s5;i150;O0;%- [@5 s ]`- [* Summary] (String). The title of the notification.&]
[s5;i150;O0;%- [@5 s ]`- [* Body] (String). The main text.&]
[s5;i150;O0;%- [@5 as ]`- [* Actions] (Array of Strings). Interactive 
buttons.&]
[s5;i150;O0;%- [@5 a`{sv`} ]`- [* Hints] (Dictionary `-a map`- of String 
to Variant). Metadata like urgency or categories.&]
[s5;i150;O0;%- [@5 i ]`- [* Timeout] ([@(0.128.128) int32]). How long the 
notification stays on screen in milliseconds.&]
[s5;%- Because D`-Bus relies on heavily nested variants (like[C  ][C@5 a`{sv`}]), 
we cannot just pass generic C`+`+ structures. To prevent runtime 
core type assertions, D`-Bus package provides [C^topic`:`/`/DBus`/src`/Upp`_DBusValue`_en`-us`#Upp`:`:DBusValueMap^@5 D
BusValueMap][R@5  ]and [C^topic`:`/`/DBus`/src`/Upp`_DBusValue`_en`-us`#Upp`:`:DBusValueArray^@5 D
BusValueArray]. These are strictly typed wrappers that guarantee 
memory`-safe marshalling across the D`-Bus wire protocol.&]
[s0;%- &]
[s3;:2: 2. Practical Usage Patterns&]
[s5;%- Instead of forcing the user to manually assemble the [C@5 susssasa`{sv`}i] 
payload every time they want to show a message, we will construct 
a helper class ([C@5 DBusNotification]). Our class will allow the 
developer to chain methods like [C@5 .Summary(`"...`").Icon(`"...`")]. 
Internally, this class abstracts the complexity of [C^topic`:`/`/DBus`/src`/Upp`_DBusValue`_en`-us`#Upp`:`:DBusValueMap^@5 D
BusValueMap][R@5  ]and [C^topic`:`/`/DBus`/src`/Upp`_DBusValue`_en`-us`#Upp`:`:DBusValueArray^@5 D
BusValueArray][C@5  ]allocation. Furthermore, it will handle parsing 
the Daemon`'s response to capture the assigned Notification ID, 
returning it as a simple [C dword] that we can reuse later.&]
[s0;%- &]
[s3;:3: 3. Step`-by`-Step Code Assembly&]
[s5;%- We will build our helper class, [C@5 DBusNotification], progressively, 
examining how each piece maps to the D`-Bus specification, before 
writing our main application loop.&]
[s22;:3`.1:%- 3.1. Defining the Class Shell and State Variables&]
[s5;%- First, we define the member variables that will hold our payload. 
Notice how they map directly to the [C@5 susssasa`{sv`}i] signature.&]
[s7;%- #include[@0  ][@(0.0.255) <][@0 DBus][@(0.0.255) /][@0 DBus][@(0.0.255) .][@0 h][@(0.0.255) >]&]
[s7;%- &]
[s7;%- namespace[@0  Upp `{]&]
[s7;%- &]
[s7;%- class[@0  DBusNotification `{]&]
[s7;%- private:&]
[s7;%-     String    app`_name;&]
[s7;%-     [@(0.128.128) dword][@0      replaces`_id;      ][/@4 // Maps 
to `'u`']&]
[s7;%-     String    icon;&]
[s7;%-     String    summary;&]
[s7;%-     String    body;&]
[s7;%-     [@(0.128.128) int32][@0      timeout;          ][/@4 // Maps 
to `'i`']&]
[s7;%-     &]
[s7;%-     DBusValueMap   hints;       [/@4 // Maps to `'a`{sv`}`']&]
[s7;%-     DBusValueArray actions;     [/@4 // Maps to `'as`']&]
[s7;%- &]
[s7;%- public:&]
[s7;%-     DBusNotification() [@(0.0.255) :] app`_name([@3 `"U`+`+ DBus 
Engine`"]), replaces`_id([@3 0]), timeout([@(0.0.255) `-][@3 1]) `{`}&]
[s5;%- [/ Note: We initialize ][/C@5 replaces`_id][/  to ][/C@5 0 ][/ so the 
daemon knows to spawn a new notification by default, and ][/C@5 timeout 
][/ to ][/C@5 `-1]&]
[s22;:3`.2:%- 3.2. Building the Setters&]
[s5;%- Next, we expose the setter methods. By returning a reference 
to [C@5 `*this], we allow the user to chain calls together. For 
the complex D`-Bus containers, we utilize the fluent operators 
([C@5 operator()] and [C@5 operator<<]) we explicitly designed into 
[C^topic`:`/`/DBus`/src`/Upp`_DBusValue`_en`-us`#Upp`:`:DBusValueMap^@5 DBusValueMa
p][R@5  ]and [C^topic`:`/`/DBus`/src`/Upp`_DBusValue`_en`-us`#Upp`:`:DBusValueArray^@5 D
BusValueArray].&]
[s7;%-     [/@4 // Basic String and Integer setters]&]
[s7;%-     DBusNotification[@(0.0.255) `&][@0  AppName(][*@(0.0.255) const][@0  
String][@(0.0.255) `&][@0  s) `{ app`_name ][@(0.0.255) `=][@0  s; ][*_@(128.0.255) return][@0  
][@(0.0.255) `*][*@(0.0.255) this][@0 ; `}]&]
[s7;%-     DBusNotification[@(0.0.255) `&][@0  Replaces(][@(0.128.128) dword][@0  
id)       `{ replaces`_id ][@(0.0.255) `=][@0  id; ][*_@(128.0.255) return][@0  
][@(0.0.255) `*][*@(0.0.255) this][@0 ; `}]&]
[s7;%-     DBusNotification[@(0.0.255) `&][@0  Icon(][*@(0.0.255) const][@0  
String][@(0.0.255) `&][@0  s)    `{ icon ][@(0.0.255) `=][@0  s; ][*_@(128.0.255) return][@0  
][@(0.0.255) `*][*@(0.0.255) this][@0 ; `}]&]
[s7;%-     DBusNotification[@(0.0.255) `&][@0  Summary(][*@(0.0.255) const][@0  
String][@(0.0.255) `&][@0  s) `{ summary ][@(0.0.255) `=][@0  s; ][*_@(128.0.255) return][@0  
][@(0.0.255) `*][*@(0.0.255) this][@0 ; `}]&]
[s7;%-     DBusNotification[@(0.0.255) `&][@0  Body(][*@(0.0.255) const][@0  
String][@(0.0.255) `&][@0  s)    `{ body ][@(0.0.255) `=][@0  s; ][*_@(128.0.255) return][@0  
][@(0.0.255) `*][*@(0.0.255) this][@0 ; `}]&]
[s7;%-     DBusNotification[@(0.0.255) `&][@0  Timeout(][@(0.128.128) int32][@0  
ms)        `{ timeout ][@(0.0.255) `=][@0  ms; ][*_@(128.0.255) return][@0  
][@(0.0.255) `*][*@(0.0.255) this][@0 ; `}]&]
[s7;%- &]
[s7;%-     [/@4 // D`-Bus Dictionary (a`{sv`}) Hint Builders]&]
[s7;%-     [/@4 // We use the DBusValueMap`::operator()(key, value) 
to append cleanly.]&]
[s7;%-     DBusNotification[@(0.0.255) `&][@0  Hint(][*@(0.0.255) const][@0  
String][@(0.0.255) `&][@0  key, ][*@(0.0.255) const][@0  DBusValue][@(0.0.255) `&][@0  
v) `{ hints(key, v); ][*_@(128.0.255) return][@0  ][@(0.0.255) `*][*@(0.0.255) this][@0 ; 
`}]&]
[s7;%-     &]
[s7;%-     [/@4 // Helper wrappers around Hint() for common FreeDesktop 
specs]&]
[s7;%-     DBusNotification[@(0.0.255) `&][@0  Urgency(][@(0.128.128) byte][@0  
level)      `{ ][*_@(128.0.255) return][@0  Hint(][@3 `"urgency`"][@0 , 
level); `} ][/@4 // 0`=low, 1`=normal, 2`=critical]&]
[s7;%-     DBusNotification[@(0.0.255) `&][@0  Category(][*@(0.0.255) const][@0  
String][@(0.0.255) `&][@0  s)`{ ][*_@(128.0.255) return][@0  Hint(][@3 `"category`"][@0 , 
s); `}]&]
[s7;%- &]
[s7;%-     [/@4 // D`-Bus Array (as) Action Builder ]&]
[s7;%-     [/@4 // The spec dictates that actions are passed as a flat 
array of string pairs: `[identifier, label`]]&]
[s7;%-     DBusNotification[@(0.0.255) `&][@0  Action(][*@(0.0.255) const][@0  
String][@(0.0.255) `&][@0  id, ][*@(0.0.255) const][@0  String][@(0.0.255) `&][@0  
label) `{]&]
[s7;%-         actions [@(0.0.255) <<][@0  id ][@(0.0.255) <<][@0  label; 
][/@4 // Using DBusValueArray`::operator<<]&]
[s7;%-         [*_@(128.0.255) return][@0  ][@(0.0.255) `*][*@(0.0.255) this][@0 ;]&]
[s7;%-     `}&]
[s22;:3`.3:%- 3.3. Constructing the D`-Bus Payload (The Send Method)&]
[s5;%- This is where the serialization happens. We take our accumulated 
state variables and pass them to [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:MethodCall`(const String`&`,const String`&`,const String`&`,const String`&`,const DBusValueArray`&`)^ D
BusConnection`::MethodCall()].&]
[s7;%-     [/@4 // Transmits the payload over the provided connection]&]
[s7;%-     [@(0.128.128) dword][@0  Send(DBusConnection][@(0.0.255) `&][@0  
dbus) `{]&]
[s7;%-         [*@(0.0.255) bool][@0  ok ][@(0.0.255) `=][@0  dbus][@(0.0.255) .][@0 MethodCall(]&]
[s7;%-             [@3 `"org.freedesktop.Notifications`"][@0 ,   ][/@4 // 
Destination service]&]
[s7;%-             [@3 `"/org/freedesktop/Notifications`"][@0 ,  ][/@4 // 
Object path]&]
[s7;%-             [@3 `"org.freedesktop.Notifications`"][@0 ,   ][/@4 // 
Interface]&]
[s7;%-             [@3 `"Notify`"][@0 ,                          ][/@4 // 
Method]&]
[s7;%-             `{&]
[s7;%-                 app`_name,&]
[s7;%-                 replaces`_id,&]
[s7;%-                 icon,&]
[s7;%-                 summary,&]
[s7;%-                 body,&]
[s7;%-                 actions,                      [/@4 // Safe variant 
array serialization]&]
[s7;%-                 hints,                        [/@4 // Safe variant 
dictionary serialization]&]
[s7;%-                 timeout&]
[s7;%-             `}&]
[s7;%-         );&]
[s22;:3`.4:%- 3.4. Parsing the Daemon`'s Response&]
[s5;%- D`-Bus method calls are synchronous by default (unless specifically 
dispatched asynchronously). The FreeDesktop daemon replies with 
a single [C@(0.128.128) uint32][@(0.128.128)  ]representing the ID 
of the notification it just created. We parse the [C^topic`:`/`/DBus`/src`/Upp`_DBusMessage`_en`-us`#Upp`:`:DBusMessage^ D
BusMessage] body to extract this ID, which we return to the caller.&]
[s7;%- const[@0  DBusMessage]`&[@0  rep ]`=[@0  dbus].[@0 GetMessage();]&]
[s7;%-         &]
[s7;%-         [*@(0.0.255) if][@0 (ok ][@(0.0.255) `&`&][@0  rep][@(0.0.255) .][@0 IsOK()) 
`{]&]
[s7;%-             DBusValueArray out [@(0.0.255) `=][@0  rep][@(0.0.255) .][@0 ParseBody();]&]
[s7;%-             &]
[s7;%-             [/@4 // Check that the body contains at least one 
item, and that it is a uint32 (dword)]&]
[s7;%-             [*@(0.0.255) if][@0 (out][@(0.0.255) .][@0 GetCount() 
][@(0.0.255) >][@0  ][@3 0][@0  ][@(0.0.255) `&`&][@0  out][@(0.0.255) `[][@3 0][@(0.0.255) `].][@0 I
s][@(0.0.255) <][@(0.128.128) dword][@(0.0.255) >][@0 ()) `{]&]
[s7;%-                 [*_@(128.0.255) return][@0  out][@(0.0.255) `[][@3 0][@(0.0.255) `].][@0 T
o][@(0.0.255) <][@(0.128.128) dword][@(0.0.255) >][@0 ();]&]
[s7;%-             `}&]
[s7;%-         `}&]
[s7;%-         [*_@(128.0.255) return][@0  ][@3 0][@0 ; ][/@4 // Return 0 to 
indicate failure]&]
[s7;%-     `}&]
[s7;%- `}; [/@4 // End of DBusNotification class]&]
[s7;%- `}  [/@4 // End of namespace Upp]&]
[s22;:3`.5:%- 3.5. The Main Execution Loop&]
[s5;%- Now that our architecture is encapsulated, utilizing it in 
a U`+`+ application is trivial. In this execution loop, we will 
demonstrate a powerful UX pattern: spawning a `"progress`" notification, 
simulating some work, and then dynamically [/ updating] that exact 
same notification bubble (using the Replaces method) so we don`'t 
spam the user`'s desktop with multiple popups.&]
[s7;%- #include[@0  ][@3 `"DBusNotification.h`"]&]
[s7;%- &]
[s7;%- using[@0  ]namespace[@0  Upp;]&]
[s7;%- &]
[s7;%- CONSOLE`_APP`_MAIN&]
[s7;%- `{&]
[s7;%-     StdLogSetup(LOG`_COUT [@(0.0.255) `|][@0  LOG`_FILE);]&]
[s7;%-     &]
[s7;%-     DBusConnection dbus;&]
[s7;%-     [@4 RLOG][@0 (][@3 `"Connecting to D`-Bus Session Bus...`"][@0 );]&]
[s7;%-     [*@(0.0.255) if][@0 (][@(0.0.255) !][@0 dbus][@(0.0.255) .][@0 ConnectSession()) 
`{]&]
[s7;%-         [@4 RLOG][@0 (][@3 `"Connection failed.`"][@0 );]&]
[s7;%-         [*_@(128.0.255) return][@0 ;]&]
[s7;%-     `}&]
[s7;%- &]
[s7;%-     [/@4 // Dispatch the initial notification]&]
[s7;%-     DBusNotification note;&]
[s7;%-     [@(0.128.128) dword][@0  note`_id ][@(0.0.255) `=][@0  note][@(0.0.255) .][@0 AppName(
][@3 `"DBus Tutorial`"][@0 )]&]
[s7;%-                         [@(0.0.255) .][@0 Icon(][@3 `"utilities`-terminal`"][@0 )]&]
[s7;%-                         [@(0.0.255) .][@0 Summary(][@3 `"Compiling 
Package`"][@0 )]&]
[s7;%-                         [@(0.0.255) .][@0 Body(][@3 `"Building DBus 
module... (50][@(0.0.255) %)][@3 `"][@0 )]&]
[s7;%-                         [@(0.0.255) .][@0 Urgency(][@3 1][@0 ) ][/@4 // 
Normal priority]&]
[s7;%-                         [@(0.0.255) .][@0 Timeout(][@3 0][@0 ) ][/@4 // 
0 forces the bubble to stay open]&]
[s7;%-                         [@(0.0.255) .][@0 Action(][@3 `"cancel`"][@0 , 
][@3 `"Cancel Build`"][@0 )]&]
[s7;%-                         [@(0.0.255) .][@0 Send(dbus);]&]
[s7;%- &]
[s7;%-     [*@(0.0.255) if][@0 (note`_id ][@(0.0.255) >][@0  ][@3 0][@0 ) `{]&]
[s7;%-         [@4 RLOG][@0 (][@3 `"Successfully launched notification. 
Assigned ID: `"][@0  ][@(0.0.255) <<][@0  note`_id);]&]
[s7;%-         &]
[s7;%-         [/@4 // Simulate a long`-running process...]&]
[s7;%-         Sleep([@3 2000][@0 ); ]&]
[s7;%-         [@4 RLOG][@0 (][@3 `"Build complete. Updating existing notification 
bubble...`"][@0 );]&]
[s7;%-         &]
[s7;%-         [/@4 // Update the existing notification]&]
[s7;%-         [/@4 // By feeding the previous note`_id into Replaces(), 
the daemon ]&]
[s7;%-         [/@4 // morphs the old bubble instead of creating a 
new one.]&]
[s7;%-         note[@(0.0.255) .][@0 Summary(][@3 `"Compilation Complete`"][@0 )]&]
[s7;%-             [@(0.0.255) .][@0 Body(][@3 `"DBus package built successfully. 
Zero errors.`"][@0 )]&]
[s7;%-             [@(0.0.255) .][@0 Icon(][@3 `"dialog`-information`"][@0 )]&]
[s7;%-             [@(0.0.255) .][@0 Replaces(note`_id) ]&]
[s7;%-             [@(0.0.255) .][@0 Timeout(][@3 3000][@0 ) ][/@4 // Bubble 
will now naturally fade out after 3 seconds]&]
[s7;%-             [@(0.0.255) .][@0 Send(dbus);]&]
[s7;%-             &]
[s7;%-         [@4 RLOG][@0 (][@3 `"Notification gracefully updated on`-screen.`"][@0 );]&]
[s7;%-     `}&]
[s7;%-     [*@(0.0.255) else][@0  `{]&]
[s7;%-         [@4 RLOG][@0 (][@3 `"Failed to send notification. Error: 
`"][@0  ][@(0.0.255) <<][@0  dbus][@(0.0.255) .][@0 GetMessage()][@(0.0.255) .][@0 GetErrorDesc
());]&]
[s7;%-     `}&]
[s7;%- `}]]