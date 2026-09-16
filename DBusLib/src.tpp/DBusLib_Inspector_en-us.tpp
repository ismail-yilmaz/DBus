topic "DBusInspector";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 DBusInspector]]}}&]
[s3; &]
[s1;:Upp`:`:DBusInspector: [@(0.0.255)3 class][3  ][*3 DBusInspector]&]
[s0;l288;%% This class provides a set of standard diagnostic and 
discovery tools for interacting with the D`-Bus daemon and other 
connected peers. Can be used in both blocking and non`-blocking 
mode.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor detail]]}}&]
[s3; &]
[s5;:Upp`:`:DBusInspector`:`:DBusInspector`(DBusConnection`&`): [* DBusInspector](DBusC
onnection[@(0.0.255) `&] [*@3 dbus])&]
[s2;%% Initializes the inspector to operate over the specified [%-*@3 dbus] 
connection. The connection must already be established and the 
[^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection^ DBusCon
nection] object must outlive the inspector.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Upp`:`:DBusInspector`:`:Enumerate`(`): [@(0.0.255) bool] [* Enumerate]()&]
[s2;%% Requests a list of all currently active (registered) service 
names on the bus via [C@5 org.freedesktop.DBus.ListNames]. Returns 
true if the request was successful. [C^topic`:`/`/DBusLib`/src`/UppLib`_Inspector`_en`-us`#Upp`:`:DBusInspector`:`:GetServiceNames`(`)const^ G
etServiceNames()] method can be used to query the active service 
names.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusInspector`:`:EnumerateActivatable`(`): [@(0.0.255) bool] 
[* EnumerateActivatable]()&]
[s2;%% Requests a list of all services that are configured to be 
automatically started (activated) by the daemon when called, 
via [C@5 org.freedesktop.DBus.ListActivatableNames]. Returns true 
if the request was successful. [C^topic`:`/`/DBusLib`/src`/UppLib`_Inspector`_en`-us`#Upp`:`:DBusInspector`:`:GetServiceNames`(`)const^ G
etServiceNames()] method can be used to query the activatable 
service names.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusInspector`:`:GetServiceNames`(`)const: Vector<String> 
[* GetServiceNames]() [@(0.0.255) const]&]
[s2;%% Returns the array of service names retrieved by the last successful 
call to [C^topic`:`/`/DBusLib`/src`/UppLib`_Inspector`_en`-us`#Upp`:`:DBusInspector`:`:Enumerate`(`)^ E
numerate()] or [C^topic`:`/`/DBusLib`/src`/UppLib`_Inspector`_en`-us`#Upp`:`:DBusInspector`:`:EnumerateActivatable`(`)^ E
numerateActivatable()].&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusInspector`:`:Introspect`(const String`&`,const String`&`): [@(0.0.255) b
ool] [* Introspect]([@(0.0.255) const] String[@(0.0.255) `&] [*@3 service], 
[@(0.0.255) const] String[@(0.0.255) `&] [*@3 path])&]
[s2;%% Queries the specified [%-*@3 service] at the given object [%-*@3 path] 
for its XML introspection data using the [C@5 org.freedesktop.DBus.Introspectable] 
interface. Returns true if the request was successful. [C^topic`:`/`/DBusLib`/src`/UppLib`_Inspector`_en`-us`#Upp`:`:DBusInspector`:`:GetResult`(`)const^ G
etResult()] method can be used to return the raw XML ouptut.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusInspector`:`:GetResult`(`)const: String [* GetResult]() 
[@(0.0.255) const]&]
[s2;%% Returns the raw XML introspection string fetched by the last 
successful call to [C^topic`:`/`/DBusLib`/src`/UppLib`_Inspector`_en`-us`#Upp`:`:DBusInspector`:`:Introspect`(const String`&`,const String`&`)^ I
ntrospect()].&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusInspector`:`:Ping`(const String`&`,const String`&`): [@(0.0.255) bool] 
[* Ping]([@(0.0.255) const] String[@(0.0.255) `&] [*@3 service], [@(0.0.255) const] 
String[@(0.0.255) `&] [*@3 path] [@(0.0.255) `=] `"[@(0.0.255) /]`")&]
[s2;%% Sends a low`-level ping command to the specified [%-*@3 service] 
and [%-*@3 path] using the [C@5 org.freedesktop.DBus.Peer] interface. 
This is used to verify that a specific application is responsive. 
Returns true if the network call was successfully executed. [C^topic`:`/`/DBusLib`/src`/UppLib`_Inspector`_en`-us`#Upp`:`:DBusInspector`:`:IsAlive`(`)const^ I
sAlive()] method can be used to check if the specific application 
was responsive.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusInspector`:`:IsAlive`(`)const: [@(0.0.255) bool] [* IsAlive]() 
[@(0.0.255) const]&]
[s2;%% Returns true if the last [C^topic`:`/`/DBusLib`/src`/UppLib`_Inspector`_en`-us`#Upp`:`:DBusInspector`:`:Ping`(const String`&`,const String`&`)^ P
ing()] call received a valid, non`-error acknowledgment from 
the target service.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusInspector`:`:FetchMachineId`(const String`&`,const String`&`): [@(0.0.255) b
ool] [* FetchMachineId]([@(0.0.255) const] String[@(0.0.255) `&] [*@3 service], 
[@(0.0.255) const] String[@(0.0.255) `&] [*@3 path] [@(0.0.255) `=] `"[@(0.0.255) /]`")&]
[s2;%% Requests the machine`'s UUID from the specified [%-*@3 service] 
and [%-*@3 path] via the [C@5 org.freedesktop.DBus.Peer.GetMachineId] 
method. Returns true if the request was successful. [C^topic`:`/`/DBusLib`/src`/UppLib`_Inspector`_en`-us`#Upp`:`:DBusInspector`:`:GetMachineId`(`)const^ G
etMachineId()] method can be used to read the machine id.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusInspector`:`:GetMachineId`(`)const: String [* GetMachineId]() 
[@(0.0.255) const]&]
[s2;%% Returns the machine UUID string retrieved by the last successful 
call to [C^topic`:`/`/DBusLib`/src`/UppLib`_Inspector`_en`-us`#Upp`:`:DBusInspector`:`:FetchMachineId`(const String`&`,const String`&`)^ F
etchMachineId()][C .]&]
[s0;%% ]]