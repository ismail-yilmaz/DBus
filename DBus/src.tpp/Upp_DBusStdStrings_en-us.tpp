topic "DBus Constants";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 D`-Bus Standard Constants]]}}&]
[s3; &]
[s2; This group of constants defines the fundamental strings specified 
by the freedesktop.org D`-Bus standard. They provide the necessary 
routing for essential IPC operations, including querying the 
bus daemon, dynamically introspecting objects, and accessing 
properties across all standard D`-Bus services.&]
[s0; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constant List]]}}&]
[s3; &]
[s5;:Upp`:`:StdDBusName: [@(0.0.255) constexpr] [@(0.0.255) const] [@(0.0.255) char] 
[@(0.0.255) `*][* StdDBusName ]`= [C `"][C@5 org.freedesktop.DBus][C `"]&]
[s2;%% The well`-known bus name of the D`-Bus daemon itself. Used 
as the destination when sending messages directly to the message 
bus (e.g., listing services). .&]
[s3; &]
[s4; &]
[s5;:Upp`:`:StdDBusPath: [@(0.0.255) constexpr] [@(0.0.255) const] [@(0.0.255) char] 
[@(0.0.255) `*][* StdDBusPath ]`=[*  ][C `"][C@5 /org/freedesktop/DBus][C `"]&]
[s2;%% The standard object path for the D`-Bus daemon.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:StdDBusInterface: [@(0.0.255) constexpr] [@(0.0.255) const] 
[@(0.0.255) char] [@(0.0.255) `*][* StdDBusInterface ]`= [C `"][C@5 org.freedesktop.DBus][C `"
]&]
[s2;%% The core interface implemented by the D`-Bus daemon. Used 
to invoke standard bus methods like [C@5 ListNames] or [C@5 RequestName]. 
.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:StdDBusProperties: [@(0.0.255) constexpr] [@(0.0.255) const] 
[@(0.0.255) char] [@(0.0.255) `*][* StdDBusProperties ]`= [C `"][C@5 org.freedesktop.DBus.P
roperties`"]&]
[s2;%% The standard interface for property access. Used with [C@5 Get], 
[C@5 Set], and [C@5 GetAll ]methods to interact with object properties. 
.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:StdDBusIntrospectable: [@(0.0.255) constexpr] [@(0.0.255) const] 
[@(0.0.255) char] [@(0.0.255) `*][* StdDBusIntrospectable] `= [C `"][C@5 org.freedesktop.DB
us.Introspectable][C `"]&]
[s2;%% The standard interface for XML introspection. Used to invoke 
the [C@5 Introspect ]method to dynamically discover an object`'s 
methods, signals, and properties..&]
[s3; &]
[s4; &]
[s5;:Upp`:`:StdDBusPeer: [@(0.0.255) constexpr] [@(0.0.255) const] [@(0.0.255) char] 
[@(0.0.255) `*][* StdDBusPeer] `= [C `"][C@5 org.freedesktop.DBus.Peer][C `"]&]
[s2;%% The standard interface for peer`-to`-peer connection management. 
Used to invoke [C@5 Ping ]to check if a service is alive.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:StdDBusObjectManager: [@(0.0.255) constexpr] [@(0.0.255) const] 
[@(0.0.255) char] [@(0.0.255) `*][* StdDBusObjectManager] `= [C `"][C@5 org.freedesktop.DBu
s.ObjectManager][C `"]&]
[s2;%% The standard interface for enumerating and tracking objects. 
Used to invoke [C@5 GetManagedObjects ]to receive a complete tree 
of objects and properties in a single call.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:StdDBusLocal: [@(0.0.255) constexpr] [@(0.0.255) const] [@(0.0.255) char] 
[@(0.0.255) `*][* StdDBusLocal] `= [C `"][C@5 org.freedesktop.DBus.Local][C `"]&]
[s2;%% The internal routing interface used by the D`-Bus daemon for 
out`-of`-band messages.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:StdDBusLocalPath: [@(0.0.255) constexpr] [@(0.0.255) const] 
[@(0.0.255) char] [@(0.0.255) `*][* StdDBusLocalPath] `= [C `"/][C@5 org/freedesktop/DBus/L
ocal][C `"]&]
[s2;%%  The object path corresponding to the local routing interface.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:StdDBusPrefix: [@(0.0.255) constexpr] [@(0.0.255) const] 
[@(0.0.255) char] [@(0.0.255) `*][* StdDBusPrefix `=  ]`= [C `"][C@5 org.freedesktop.DBus.][C `"
]&]
[s2;%% The standard namespace prefix for core D`-Bus services, errors, 
and interfaces. Useful for string matching to determine if an 
interface or error name belongs to the base D`-Bus specification 
(e.g., checking if an error string starts with `"[C@5 org.freedesktop.DBus.Error]`"
).&]
[s3;%% ]]