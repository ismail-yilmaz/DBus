topic "Tutorial 2: Complex Variant Marshalling";
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
[s2;= Tutorial: Complex Variant Marshaling&]
[s5;%- While simple D`-Bus method calls exchange primitive data types 
like strings and integers, real`-world Inter`-Process Communication 
requires transmitting rich, composite structures such as configuration 
property bags, hardware profiles, and nested lists. With the 
U`+`+ DBus package, complex structures are safely managed using 
specialized container types ([C^topic`:`/`/DBus`/src`/Upp`_DBusValue`_en`-us`#Upp`:`:DBusValueMap^ D
BusValueMap] and [C^topic`:`/`/DBus`/src`/Upp`_DBusValue`_en`-us`#Upp`:`:DBusValueArray^ D
BusValueArray]) that map directly to D`-Bus variant signature 
types like [C@5 a`{sv`}] (dictionary of string to variant) and 
generic arrays.&]
[s5;%- In this tutorial, we will explore the architecture of variant 
serialization, examine how composite data boundaries are enforced 
across the wire, and assemble a complete U`+`+ application that 
transmits mixed`-type payloads and gracefully handles daemon`-side 
error responses.&]
[s3; Table of Contents&]
[s23; &]
[s0; [^topic`:`/`/DBus`/srcdoc`/Tutorial2`_en`-us`#1^ 1. D`-Bus Variant 
Architecture]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial2`_en`-us`#1`.1^ 1.1. The 
Variant Type System (v and a`{sv`})]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial2`_en`-us`#1`.2^ 1.2. Binary 
Serialization Boundaries]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial2`_en`-us`#1`.3^ 1.3. Type`-Safety 
Enforcement in U`+`+]&]
[s23; [^topic`:`/`/DBus`/srcdoc`/Tutorial2`_en`-us`#2^ 2. Core Concepts 
and Usage Principles]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial2`_en`-us`#2`.1^ 2.1. Composite 
Container Initialization]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial2`_en`-us`#2`.2^ 2.2. Explicit 
Type Retention in Mixed Maps]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial2`_en`-us`#2`.3^ 2.3. Introspecting 
Daemon Error Responses]&]
[s23; [^topic`:`/`/DBus`/srcdoc`/Tutorial2`_en`-us`#3^ 3. Assembling 
the Demo]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial2`_en`-us`#3`.1^ 3.1. Establishing 
Session Connection]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial2`_en`-us`#3`.2^ 3.2. Initializing 
the Variant Dictionary (DBusValueMap)]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial2`_en`-us`#3`.3^ 3.3. Initializing 
the Variant Array (DBusValueArray)]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial2`_en`-us`#3`.4^ 3.4. Dispatching 
Composite Payloads via Method Call]&]
[s23;     [^topic`:`/`/DBus`/srcdoc`/Tutorial2`_en`-us`#3`.5^ 3.5. Capturing 
and Analyzing Error Packets]&]
[s23; &]
[s3;:1:%- 1. D`-Bus Variant Architecture&]
[s5;%- Before diving into code, it is important to understand how 
complex containers are structured and transmitted over the D`-Bus 
wire protocol.&]
[s22;:1`.1:%- 1.1. The Variant Type System ([C v] and [C a`{sv`}])&]
[s5;%- Unlike static type systems where a container must hold elements 
of a single data type, D`-Bus provides a variant type ([C@5 v]). 
A variant acts as a container envelope that wraps any primitive 
or container type alongside its exact type signature. The most 
common pattern in desktop Linux architecture is the [C@5 a`{sv`}] 
signature (an array of dictionary entries mapping string keys 
to variant values). This is used everywhere from window manager 
states to device configurations.&]
[s22;:1`.2:%- 1.2. Binary Serialization Boundaries&]
[s5;%- When sending composite structures, the U`+`+ IPC engine must 
walk the entire object tree, calculating alignment padding, serializing 
type strings, and packing values into a contiguous binary stream. 
If a container`'s boundaries or type signatures are malformed, 
the receiving daemon will reject the packet at the socket level.&]
[s22;:1`.3:%- 1.3. Type`-Safety Enforcement in U`+`+&]
[s5;%- To prevent runtime type mismatches or crashes caused by native 
U`+`+ generic containers, the package relies on dedicated wrapper 
types ([C^topic`:`/`/DBus`/src`/Upp`_DBusValue`_en`-us`#Upp`:`:DBusValueMap^ DBusVa
lueMap] and [C^topic`:`/`/DBus`/src`/Upp`_DBusValue`_en`-us`#Upp`:`:DBusValueArray^ D
BusValueArray]). These wrappers guarantee that every inserted 
element is explicitly validated and correctly bound to a [C^topic`:`/`/DBus`/src`/Upp`_DBusValue`_en`-us`#Upp`:`:DBusValue^ D
BusValue][^topic`:`/`/DBus`/src`/Upp`_DBusValue`_en`-us`#Upp`:`:DBusValue^  
]instance before serialization.&]
[s0;%- &]
[s3;:2:%- 2. Core Concepts and Usage Principles&]
[s5;%- Building a robust composite D`-Bus call relies on three primary 
data management principles.&]
[s22;:2`.1:%- 2.1. Composite Container Initialization&]
[s5;%- U`+`+ supports clean C`+`+11 initializer lists for initializing 
maps and arrays. This allows developers to declare hierarchical 
payloads inline with high readability, mirroring how configuration 
trees are structured in markup languages.&]
[s22;:2`.2:%- 2.2. Explicit Type Retention in Mixed Maps&]
[s5;%- Because C`+`+ integer literals default to standard [C@5 int][@5  
]or [C@5 int32], mixed`-type maps often require explicit casting 
(e.g., [C@(0.128.128) uint64] or [C@5 double]) to ensure the D`-Bus 
marshaller encodes the exact wire signature expected by the target 
service.&]
[s22;:2`.3:%- 2.3. Introspecting Daemon Error Responses&]
[s5;%- When testing payloads against restricted or non`-existent 
methods, the D`-Bus broker does not crash the client application. 
Instead, it returns a structured error packet. Client code can 
inspect these packets using [^topic`:`/`/DBus`/src`/Upp`_DBusMessage`_en`-us`#Upp`:`:DBusMessage`:`:IsError`(`)const^ D
BusMessage`::][C^topic`:`/`/DBus`/src`/Upp`_DBusMessage`_en`-us`#Upp`:`:DBusMessage`:`:IsError`(`)const^ I
sError()] to retrieve diagnostic information directly from the 
system bus.&]
[s0;%- &]
[s3;:3:%- 3. Assembling the Demo&]
[s5;%- With the architectural principles established, let`'s assemble 
the U`+`+ console application step`-by`-step.&]
[s22;:3`.1:%- 3.1. Establishing Session Connection&]
[s5;%- We begin by setting up standard logging and instantiating 
our connection object, binding securely to the local user session 
bus.&]
[s7;%- #include[@0  ][@(0.0.255) <][@0 DBus][@(0.0.255) /][@0 DBus][@(0.0.255) .][@0 h][@(0.0.255) >]&]
[s7;%- &]
[s7;%- using[@0  ]namespace[@0  Upp;]&]
[s7;%- &]
[s7;%- CONSOLE`_APP`_MAIN&]
[s7;%- `{&]
[s7;%-     StdLogSetup(LOG`_COUT [@(0.0.255) `|][@0  LOG`_FILE);]&]
[s7;%- &]
[s7;%-     DBusConnection dbus;&]
[s7;%-     &]
[s7;%-     [/@4 // Connect to the local session bus socket]&]
[s7;%-     [*@(0.0.255) if][@0 (][@(0.0.255) !][@0 dbus][@(0.0.255) .][@0 ConnectSession()) 
]&]
[s7;%-         [*_@(128.0.255) return];&]
[s22;:3`.2:%- 3.2. Initializing the Variant Dictionary ([C DBusValueMap])&]
[s5;%- Next, we construct a complex configuration map ([C@5 a`{sv`}]) 
containing mixed data types: a string, an explicitly cast 64`-bit 
integer, a floating`-point number, and a boolean flag[C .]&]
[s7;%- // Construct a type`-safe variant dictionary representing 
a hardware/display profile&]
[s7;%- DBusValueMap map `{&]
[s7;%-     `{ [@3 `"WindowSystem`"][@0 , ][@3 `"Wayland`"][@0  `},]&]
[s7;%-     `{ [@3 `"Width`"][@0 ,  (][@(0.128.128) uint64][@0 ) ][@3 1920][@0  
`},]&]
[s7;%-     `{ [@3 `"Scaling`"][@0 , ][@(128.0.255) 1.25][@0  `},]&]
[s7;%-     `{ [@3 `"Accelarated`"][@0 , ][*@(0.0.255) true][@0  `},]&]
[s7;%- `};&]
[s22;:3`.3:%- 3.3. Initializing the Variant Array ([C DBusValueArray])&]
[s5;%- We then define a supporting array payload representing a list 
of supported graphics rendering backends.&]
[s7;%- // Construct a type`-safe variant array for list data&]
[s7;%- DBusValueArray arr `{&]
[s7;%-     [@3 `"EGL`"][@0 ,]&]
[s7;%-     [@3 `"OpenGL`"][@0 ,]&]
[s7;%-     [@3 `"Vulkan`"]&]
[s7;%- `};&]
[s7;%- &]
[s7;%- RLOG([@3 `"Sending complex U`+`+ payloads over the wire...`"][@0 );]&]
[s22;:3`.4:%- 3.4. Dispatching Composite Payloads via Method Call&]
[s5;%- We bundle our custom [C map] and [C arr] containers directly into 
the argument initializer list of [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:MethodCall`(const String`&`,const String`&`,const String`&`,const String`&`,const DBusValueArray`&`)^ M
ethodCall()]. In this example, we target a non`-existent method 
([C@5 NonExistentMethod][C )] on the D`-Bus daemon to demonstrate 
how composite boundaries are validated even when the target call 
fails.&]
[s7;%- // Dispatch the method call, packaging the custom containers 
as variant arguments&]
[s7;%- dbus[@(0.0.255) .][@0 MethodCall(]&]
[s7;%-     [@3 `"org.freedesktop.DBus`"][@0 ,]&]
[s7;%-     [@3 `"/org/freedesktop/DBus`"][@0 ,]&]
[s7;%-     [@3 `"org.freedesktop.DBus`"][@0 ,]&]
[s7;%-     [@3 `"NonExistentMethod`"][@0 ,            ][/@4 // Intentionally 
invalid method to trigger an error reply]&]
[s7;%-     `{ map, arr `}&]
[s7;%- );&]
[s22;:3`.5:%- 3.5. Capturing and Analyzing Error Packets&]
[s5;%- [+93 Finally, we retrieve the response message. Instead of checking 
for a standard success code (][C^topic`:`/`/DBus`/src`/Upp`_DBusMessage`_en`-us`#Upp`:`:DBusMessage`:`:IsOK`(`)const^+93 D
BusMessage`::IsOK()][+93 ), we inspect whether the daemon returned 
an error (][C^topic`:`/`/DBus`/src`/Upp`_DBusMessage`_en`-us`#Upp`:`:DBusMessage`:`:IsError`(`)const^+93 D
BusMessage`::IsError()][+93 ), proving that the complex payload 
boundaries were successfully parsed and delivered across the 
socket before the method lookup failed.]&]
[s7;%-     [/@4 // Intercept the response message from the broker]&]
[s7;%-     [*@(0.0.255) if][@0 (][*@(0.0.255) const][@0  DBusMessage][@(0.0.255) `&][@0  
msg ][@(0.0.255) `=][@0  dbus][@(0.0.255) .][@0 GetMessage(); msg][@(0.0.255) .][@0 IsError()
) `{]&]
[s7;%-         [@4 RLOG][@0 (][@3 `"Daemon safely parsed the payload boundaries 
and replied:`"][@0 );]&]
[s7;%-         [@4 RLOG][@0 (msg][@(0.0.255) .][@0 GetErrorName());]&]
[s7;%-     `}&]
[s7;%- `}]]