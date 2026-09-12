topic "DBusNotification";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 DBusNotification]]}}&]
[s3; &]
[s1;:Upp`:`:DBusNotification: [@(0.0.255)3 class][3  ][*3 DBusNotification]&]
[s0;l288;%% Tis class provides an easy`-to`-use interface for interacting 
with the desktop environment`'s notification daemon via the [C@5 org.freedesktop.No
tifications] standard. Like other DBusLib utility classes, it 
separates network requests from data retrieval. Client  code 
should first initiate a command (which returns true if the D`-Bus 
call succeeded), and then retrieve the parsed result using the 
corresponding Get method. Can be use both in blocking and non`-blocking 
mode.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor detail]]}}&]
[s3; &]
[s5;:Upp`:`:DBusNotification`:`:DBusNotification`(DBusConnection`&`): [* DBusNotificati
on](DBusConnection[@(0.0.255) `&] [*@3 session])&]
[s2;%% Initializes the notification helper to operate over the specified 
[%-*@3 dbus] connection. The connection must already be established 
and the [^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection^ D
BusConnection] object must outlive the notification helper.&]
[s3;%% &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Upp`:`:DBusNotification`:`:App`(const String`&`): DBusNotification[@(0.0.255) `&] 
[* App]([@(0.0.255) const] String[@(0.0.255) `&] [*@3 name])&]
[s2;%% .Sets the application name that will be displayed by the notification 
server. Returns `*this for met hod chaining.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusNotification`:`:Replaces`(uint32`): DBusNotification[@(0.0.255) `&] 
[* Replaces](uint32 [*@3 id])&]
[s2;%% Sets the ID of an existing notification that this new notification 
should replace or update. If [%-*@3 id] is [C@5 0], a new notification 
is always created. Returns `*this for met hod chaining.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusNotification`:`:Icon`(const String`&`): DBusNotification[@(0.0.255) `&] 
[* Icon]([@(0.0.255) const] String[@(0.0.255) `&] [*@3 s])&]
[s2;%% Sets the icon for the notification. This can be an absolute 
file path (e.g., `"[C@5 /usr/share/icons/...]`") or a standard 
freedesktop icon name (e.g., `"[C@5 dialog`-warning]`").  Returns 
`*this for met hod chaining.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusNotification`:`:Summary`(const String`&`): DBusNotification[@(0.0.255) `&
] [* Summary]([@(0.0.255) const] String[@(0.0.255) `&] [*@3 s])&]
[s2;%% Sets the summary text (the title or headline) of the notification. 
Returns `*this for met hod chaining.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusNotification`:`:Body`(const String`&`): DBusNotification[@(0.0.255) `&] 
[* Body]([@(0.0.255) const] String[@(0.0.255) `&] [*@3 s])&]
[s2;%% Sets the main body text of the notification. Most modern desktop 
environments support basic HTML markup here (like [C@5 <b>], [C@5 <i>], 
or [C@5 <br>]). Returns `*this for met hod chaining.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusNotification`:`:Timeout`(int32`): DBusNotification[@(0.0.255) `&] 
[* Timeout]([@(0.128.128) int32 ][*@3 ms])&]
[s2;%% Sets the expiration time of the notification in milliseconds. 
A value of [C@5 `-1] instructs the server to use its own default 
timeout based on urgency, while [C@5 0] means the notification 
should never automatically expire. Returns `*this for met hod 
chaining.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusNotification`:`:Action`(const String`&`,const String`&`): DBusNotific
ation[@(0.0.255) `&] [* Action]([@(0.0.255) const] String[@(0.0.255) `&] 
[*@3 id], [@(0.0.255) const] String[@(0.0.255) `&] [*@3 text])&]
[s2;%% Adds a user`-interactive action (typically rendered as a button). 
The [%-*@3 id ]is the programmatic key returned by the daemon if 
the user clicks it, and [%-*@3 text ]is the human`-readable label 
displayed on the button. Returns `*this for met hod chaining.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusNotification`:`:Hint`(const String`&`,const DBusValue`&`): DBusNotifi
cation[@(0.0.255) `&] [* Hint]([@(0.0.255) const] String[@(0.0.255) `&] 
[*@3 key], [@(0.0.255) const] DBusValue[@(0.0.255) `&] [*@3 value])&]
[s2;%% Adds extra metadata to the notification using the standard 
hints dictionary. The [%-*@3 value] is automatically wrapped into 
a D`-Bus variant ([C@5 `'v`']) for wire serialization. Common hints 
include `"[C@5 sound`-name]`", `"[C@5 category]`", or `"[C@5 image`-path]`". 
Returns `*this for method chaining.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusNotification`:`:Urgency`(byte`): DBusNotification[@(0.0.255) `&] 
[* Urgency]([@(0.128.128) byte ][*@3 level])&]
[s2;%% A convenience helper that adds an `"urgency`" hint to the 
notification. Standard levels are [C@5 0] (Low), [C@5 1] (Normal), 
and [C@5 2] (Critical). Returns `*this for met hod chaining.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusNotification`:`:Notify`(`): [@(0.0.255) bool] [* Notify]()&]
[s2;%% Compiles the builder parameters into the standard wire format 
and dispatches the [C@5 Notify ]method call to the D`-Bus notification 
daemon. Returns true if the network request was successful.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusNotification`:`:GetNotificationId`(`)const: [@(0.128.128) uint32 
][* GetNotificationId]() [@(0.0.255) const]&]
[s2;%% Retrieves the unique notification ID assigned and returned 
by the desktop environment after a successful [C^topic`:`/`/DBusLib`/src`/DBusLib`_Notification`_en`-us`#Upp`:`:DBusNotification`:`:Notify`(`)^ N
otify()] call. This ID can be saved and passed to [C^topic`:`/`/DBusLib`/src`/DBusLib`_Notification`_en`-us`#Upp`:`:DBusNotification`:`:Replaces`(uint32`)^ R
eplaces()] later to update the same popup, or used to close it 
programmatically. Returns[C@5  0] on failure .&]
[s3; &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 D`-Bus Desktop Notification Constants]]}}&]
[s3; &]
[s2; This group of constants defines the fundamental strings specified 
by the Desktop Notifications standard ([C@5 org.freedesktop.Notifications]). 
They provide the required routing for broadcasting popups, closing 
active notifications, and querying the visual capabilities of 
the user`'s current desktop environment.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:StdDBusNotificationName: [@(0.0.255) constexpr] [@(0.0.255) const] 
[@(0.0.255) char] [@(0.0.255) `*][* StdDBusNotificationName] `= [C `"][C@5 org.freedesktop.
Notifications][C `"]&]
[s2;%% The well`-known bus name of the active notification daemon. 
Used as the destination service name when routing notification 
messages on the session bus.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:StdDBusNotificationPath: [@(0.0.255) constexpr] [@(0.0.255) const] 
[@(0.0.255) char] [@(0.0.255) `*][* StdDBusNotificationPath] `= [C `"][C@5 /org/freedesktop
/Notifications][C `"]&]
[s2;%% The standard object path implemented by the notification daemon.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:StdDBusNotificationInterface: [@(0.0.255) constexpr] [@(0.0.255) const] 
[@(0.0.255) char] [@(0.0.255) `*][* StdDBusNotificationInterface] `= 
[C `"][C@5 org.freedesktop.Notifications][C `"]&]
[s2;%% The core interface implemented by the notification daemon. 
Used to invoke standard notification methods, including [C@5 Notify], 
[C@5 CloseNotification], [C@5 GetCapabilities], and [C@5 GetServerInformation].&]
[s3; &]
[s0;%% ]]