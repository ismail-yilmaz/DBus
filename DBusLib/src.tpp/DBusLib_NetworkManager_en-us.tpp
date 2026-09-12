topic "DBusNetworkManager";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 DBusNetworkManager]]}}&]
[s3; &]
[s1;:Upp`:`:DBusNetworkManager: [@(0.0.255)3 class][3  ][*3 DBusNetworkManager]&]
[s2;%% This class provides a high`-level interface for querying network 
states, active connections, and hardware toggles through the 
system`-wide [C@5 org.freedesktop.NetworkManager] D`-Bus daemon. 
Like other DBusLib utility classes, it separates network requests 
from data retrieval. Client  code should first initiate a command 
(which returns true if the D`-Bus call succeeded), and then retrieve 
the parsed result using the corresponding Get method. Can be 
use both in blocking and non`-blocking mode.&]
[s2;%% &]
[s2;%% [/ Important]: NetworkManager operates on the System Bus. Therefore 
[C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection^ DBusCo
nnection][C  ]must be pre`-intialized using [C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection`:`:ConnectSystem`(`)^ C
onnectSystem()] method before passing it to this class. Modifying 
states (via [C@5 Enable`*], [C@5 Disable`*], or [C@5 Sleep ]methods) 
typically requires active PolicyKit (polkit) privileges.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor detail]]}}&]
[s3; &]
[s5;:Upp`:`:DBusNetworkManager`:`:DBusNetworkManager`(DBusConnection`&`): [* DBusNetwor
kManager](DBusConnection[@(0.0.255) `&] [*@3 dbus])&]
[s2;%% Initializes the manager to operate over the specified [%-*@3 dbus] 
connection. The connection must already be established and the 
[C^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection^ DBusCo
nnection][C  ]object must outlive the manager.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Upp`:`:DBusNetworkManager`:`:FetchVersion`(`): [@(0.0.255) bool] 
[* FetchVersion]()&]
[s2;%% Requests the NetworkManager daemon version string. Returns 
true on success. .&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusNetworkManager`:`:GetVersion`(`)const: String [* GetVersion]() 
[@(0.0.255) const]&]
[s2;%% Returns the version string (e.g., `"[C@5 1.44.2]`") retrieved 
by the last successful [C^topic`:`/`/DBusLib`/src`/DBusLib`_NetworkManager`_en`-us`#Upp`:`:DBusNetworkManager`:`:FetchVersion`(`)^ F
etchVersion()] call.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusNetworkManager`:`:FetchState`(`): [@(0.0.255) bool] 
[* FetchState]()&]
[s2;%% Requests the overall NetworkManager daemon state. Returns 
true on success.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusNetworkManager`:`:GetState`(`)const: State [* GetState]() 
[@(0.0.255) const]&]
[s2;%% Returns the state retrieved by the last successful [C^topic`:`/`/DBusLib`/src`/DBusLib`_NetworkManager`_en`-us`#Upp`:`:DBusNetworkManager`:`:FetchState`(`)^ F
etchState()] call. .&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusNetworkManager`:`:FetchConnectivity`(`): [@(0.0.255) bool] 
[* FetchConnectivity]()&]
[s2;%% Requests the daemon`'s current internet connectivity assessment. 
Returns true on success.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusNetworkManager`:`:GetConnectivity`(`)const: Connectivity 
[* GetConnectivity]() [@(0.0.255) const]&]
[s2;%% Returns the connectivity status retrieved by the last successful 
[C^topic`:`/`/DBusLib`/src`/DBusLib`_NetworkManager`_en`-us`#Upp`:`:DBusNetworkManager`:`:FetchConnectivity`(`)^ F
etchConnectivity()] call. .&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusNetworkManager`:`:FetchDevices`(`): [@(0.0.255) bool] 
[* FetchDevices]()&]
[s2;%% Requests the list of all network devices recognized by the 
daemon. Returns true on success.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusNetworkManager`:`:GetDevices`(`)const: Vector<String> 
[* GetDevices]() [@(0.0.255) const]&]
[s2;%% Returns an array of D`-Bus object paths representing the system`'s 
network devices fetched by the latest [C^topic`:`/`/DBusLib`/src`/DBusLib`_NetworkManager`_en`-us`#Upp`:`:DBusNetworkManager`:`:FetchDevices`(`)^ F
etchDevices()] call.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusNetworkManager`:`:FetchActiveConnections`(`): [@(0.0.255) bool] 
[* FetchActiveConnections]()&]
[s2;%% Requests the list of currently active network connections. 
Returns true on success.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusNetworkManager`:`:GetActiveConnections`(`)const: Vector<String> 
[* GetActiveConnections]() [@(0.0.255) const]&]
[s2;%% Returns an array of D`-Bus object paths pointing to the currently 
active connection profiles, fetched  by the latest [C^topic`:`/`/DBusLib`/src`/DBusLib`_NetworkManager`_en`-us`#Upp`:`:DBusNetworkManager`:`:FetchActiveConnections`(`)^ F
etchActiveConnections()] call.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusNetworkManager`:`:FetchPrimaryConnection`(`): [@(0.0.255) bool] 
[* FetchPrimaryConnection]()&]
[s2;%% Requests the object path of the connection currently providing 
the default route (the primary internet link). Returns true on 
success.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusNetworkManager`:`:GetPrimaryConnection`(`)const: String 
[* GetPrimaryConnection]() [@(0.0.255) const]&]
[s2;%% Returns the object path of the primary connection fetched 
by the latest [C^topic`:`/`/DBusLib`/src`/DBusLib`_NetworkManager`_en`-us`#Upp`:`:DBusNetworkManager`:`:FetchPrimaryConnection`(`)^ F
etchPrimaryConnection()] call.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusNetworkManager`:`:FetchPrimaryConnectionType`(`): [@(0.0.255) bool] 
[* FetchPrimaryConnectionType]()&]
[s2;%% Requests the hardware type string of the primary connection. 
Returns true on success.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusNetworkManager`:`:GetPrimaryConnectionType`(`)const: String 
[* GetPrimaryConnectionType]() [@(0.0.255) const]&]
[s2;%% Returns a descriptive string of the primary connection type 
fetched by the latest [C^topic`:`/`/DBusLib`/src`/DBusLib`_NetworkManager`_en`-us`#Upp`:`:DBusNetworkManager`:`:FetchPrimaryConnectionType`(`)^ F
etchPrimaryConnectionType()] call (e.g., `"[C@5 802`-3`-ethernet]`", 
`"[C@5 802`-11`-wireless]`", `"[C@5 vpn]`").&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusNetworkManager`:`:FetchNetworkingStatus`(`): [@(0.0.255) bool] 
[* FetchNetworkingStatus]()&]
[s2;%% Requests the global networking enablement state. Returns true 
on success.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusNetworkManager`:`:IsNetworkingEnabled`(`)const: [@(0.0.255) bool] 
[* IsNetworkingEnabled]() [@(0.0.255) const]&]
[s2;%% Returns true if global networking is enabled, based on the 
last [C^topic`:`/`/DBusLib`/src`/DBusLib`_NetworkManager`_en`-us`#Upp`:`:DBusNetworkManager`:`:FetchNetworkingStatus`(`)^ F
etchNetworkingStatus()] call.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusNetworkManager`:`:EnableNetworking`(bool`): [@(0.0.255) bool] 
[* EnableNetworking]([@(0.0.255) bool] [*@3 b] [@(0.0.255) `=] [@(0.0.255) true])&]
[s2;%% Globally enables or disables all networking interfaces managed 
by the daemon.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusNetworkManager`:`:DisableNetworking`(`): [@(0.0.255) bool] 
[* DisableNetworking]()&]
[s2;%% Globally disables all networking interfaces managed by the 
daemon.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusNetworkManager`:`:FetchWirelessStatus`(`): [@(0.0.255) bool] 
[* FetchWirelessStatus]()&]
[s2;%% Requests the Wi`-Fi subsystem state. Returns true on success.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusNetworkManager`:`:IsWirelessEnabled`(`)const: [@(0.0.255) bool] 
[* IsWirelessEnabled]() [@(0.0.255) const]&]
[s2;%% Returns true if Wi`-Fi is enabled (not in airplane mode).&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusNetworkManager`:`:EnableWireless`(bool`): [@(0.0.255) bool] 
[* EnableWireless]([@(0.0.255) bool] [*@3 b] [@(0.0.255) `=] [@(0.0.255) true])&]
[s2;%% Toggles the wireless (Wi`-Fi) hardware subsystem.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusNetworkManager`:`:DisableWireless`(`): [@(0.0.255) bool] 
[* DisableWireless]()&]
[s2;%% Disables the wireless (Wi`-Fi) hardware subsystem.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusNetworkManager`:`:FetchWwanStatus`(`): [@(0.0.255) bool] 
[* FetchWwanStatus]()&]
[s2;%% Requests the Mobile Broadband (WWAN) subsystem state. Returns 
true on success.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusNetworkManager`:`:IsWwanEnabled`(`)const: [@(0.0.255) bool] 
[* IsWwanEnabled]() [@(0.0.255) const]&]
[s2;%% Returns true if cellular modems/WWAN are enabled.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusNetworkManager`:`:EnableWwan`(bool`): [@(0.0.255) bool] 
[* EnableWwan]([@(0.0.255) bool] [*@3 b] [@(0.0.255) `=] [@(0.0.255) true])&]
[s2;%% Toggles the cellular/WWAN hardware subsystem.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusNetworkManager`:`:DisableWwan`(`): [@(0.0.255) bool] 
[* DisableWwan]()&]
[s2;%% Disables the cellular/WWAN hardware subsystem.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusNetworkManager`:`:Sleep`(bool`): [@(0.0.255) bool] 
[* Sleep]([@(0.0.255) bool] [*@3 b ][@(0.0.255) `=] [@(0.0.255) true])&]
[s2;%% Instructs NetworkManager to put all network interfaces to 
sleep (true) or wake them up (false). Typically used when the 
system is suspending or resuming from disk/RAM.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusNetworkManager`:`:WakeUp`(`): [@(0.0.255) bool] [* WakeUp]()&]
[s2;%% Instructs NetworkManager to wake all network interfaces. Typically 
used when the system is resuming from disk/RAM.&]
[s3; &]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Enumerations]]}}&]
[s3; &]
[s5;:Upp`:`:DBusNetworkManager`:`:State: [@(0.0.255) enum] [* DBusNetworkManager`::State] 
[@(0.0.255) :] [@(0.128.128) uint32]&]
[s2;%% Enumerators representing the overall daemon state.&]
[s0;*%% &]
[ {{2147:1006:6847h1; [s0;%% [* Value]]
:: [s0;%% [* Code]]
:: [s0;%% [* Description]]
:: [s0;%% [C@5 Unknown]]
:: [s0;=%% [C@5 0]]
:: [s0;%% Networking state is unknown or uninitialized.]
:: [s0;%% [C@5 Asleep]]
:: [s0;=%% [C@5 10]]
:: [s0;%% Networking is inactive, or the system is suspended.]
:: [s0;%% [C@5 Disconnected]]
:: [s0;=%% [C@5 20]]
:: [s0;%% No active connections are established.]
:: [s0;%% [C@5 Disconnecting]]
:: [s0;=%% [C@5 30]]
:: [s0;%% Network connections are currently being torn down.]
:: [s0;%% [C@5 Connecting]]
:: [s0;=%% [C@5 40]]
:: [s0;%% Actively negotiating a network connection.]
:: [s0;%% [C@5 ConnectedLocal]]
:: [s0;=%% [C@5 50]]
:: [s0;%% Connected to a local network (loopback/link`-local) but no 
external routing.]
:: [s0;%% [C@5 ConnectedSite]]
:: [s0;=%% [C@5 60]]
:: [s0;%% Connected to a network with local routing, but no internet 
access.]
:: [s0;%% [C@5 ConnectedGlobal]]
:: [s0;=%% [C@5 70]]
:: [s0;%% Fully connected to the internet.]}}&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:DBusNetworkManager`:`:Connectivity: [@(0.0.255) enum] [* DBusNetworkManager`:
:Connectivity] [@(0.0.255) :] [@(0.128.128) uint32]&]
[s2;%%  Enumerators representing the internet connectivity status 
based on NetworkManager`'s active probing.&]
[s2;%% &]
[ {{2147:1006:6847h1; [s0;%% [* Value]]
:: [s0;%% [* Code]]
:: [s0;%% [* Description]]
:: [s0;%% [C@5 Unknown]]
:: [s0;=%% [C@5 0]]
:: [s0;%% Connectivity status is unknown.]
:: [s0;%% [C@5 None]]
:: [s0;=%% [C@5 1]]
:: [s0;%% The host is not connected to any network.]
:: [s0;%% [C@5 Portal]]
:: [s0;=%% [C@5 2]]
:: [s0;%% Traffic is intercepted by a captive portal (e.g., hotel/airport 
Wi`-Fi).]
:: [s0;%% [C@5 Limited]]
:: [s0;=%% [C@5 3]]
:: [s0;%% Connected, but full internet access is unavailable.]
:: [s0;%% [C@5 Full]]
:: [s0;=%% [C@5 4]]
:: [s0;%% Fully connected with unrestricted internet access.]}}&]
[s3;%% &]
[s4;%% &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 D`-Bus NetworkManager Constants]]}}&]
[s3; &]
[s2; This group of constants defines the fundamental strings specified 
by the NetworkManager standard ([C@5 org.freedesktop.NetworkManager]). 
They provide the required routing for querying global network 
states, enumerating hardware devices, determining internet connectivity, 
and toggling network subsystems like Wi`-Fi and WWAN.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:StdDBusNetworkManagerName: [@(0.0.255) constexpr] [@(0.0.255) const] 
[@(0.0.255) char] [@(0.0.255) `*][* StdDBusNetworkManagerName ]`= [C `"][C@5 org.freedeskto
p.NetworkManager][C `"]&]
[s2;%% The well`-known bus name of the NetworkManager system daemon.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:StdDBusNetworkManagerPath: [@(0.0.255) constexpr] [@(0.0.255) const] 
[@(0.0.255) char] [@(0.0.255) `*][* StdDBusNetworkManagerPath ]`= [C `"/][C@5 org/freedeskt
op/NetworkManager][C `"]&]
[s2;%% The root object path implemented by NetworkManager.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:StdDBusNetworkManagerInterface: [@(0.0.255) constexpr] 
[@(0.0.255) const] [@(0.0.255) char] [@(0.0.255) `*][* StdDBusNetworkManagerInterface 
]`= [C `"][C@5 org.freedesktop.NetworkManager][C `"]&]
[s2;%% The primary interface handling global states and device management.]]