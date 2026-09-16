topic "DBusMediaPlayer";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 DBusMediaPlayer]]}}&]
[s3; &]
[s1;:Upp`:`:DBusMediaPlayer: [@(0.0.255)3 class][3  ][*3 DBusMediaPlayer]&]
[s0;l288;%% This class provides an interface for controlling and 
querying media players over D`-Bus using the standard MPRIS (Media 
Player Remote Interfacing Specification) protocol. Can be used 
in both blocking and non`-blocking mode.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor detail]]}}&]
[s3; &]
[s5;:Upp`:`:DBusMediaPlayer`:`:DBusMediaPlayer`(DBusConnection`&`): [* DBusMediaPlayer](
DBusConnection[@(0.0.255) `&] [*@3 session])&]
[s2;%% Initializes the media player to operate over the specified 
[%-*@3 dbus] connection. The connection must already be established 
and the [^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection^ D
BusConnection] object must outlive the media player.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Upp`:`:DBusMediaPlayer`:`:Target`(const String`&`): DBusMediaPlayer[@(0.0.255) `&] 
[* Target]([@(0.0.255) const] String[@(0.0.255) `&] [*@3 player])&]
[s2;%% Sets the D`-Bus destination name of the media [%-*@3 player] 
you want to control (e.g., [C@5 org.mpris.MediaPlayer2.spotify]). 
Returns `*this for method chaining.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMediaPlayer`:`:GetTarget`(`)const: String [* GetTarget]() 
[@(0.0.255) const]&]
[s2;%% Returns the currently targeted media player destination name.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMediaPlayer`:`:FetchPlayers`(`): [@(0.0.255) bool] 
[* FetchPlayers]()&]
[s2;%% Queries the D`-Bus daemon for a list of all currently active 
services that implement the MPRIS interface. Returns true if 
the query succeeded.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMediaPlayer`:`:GetPlayers`(`)const: Vector<String> 
[* GetPlayers]() [@(0.0.255) const]&]
[s2;%% Returns the list of active MPRIS media player destination 
names retrieved by the last successful [C^topic`:`/`/DBusLib`/src`/DBusLib`_MediaPlayer`_en`-us`#Upp`:`:DBusMediaPlayer`:`:FetchPlayers`(`)^ F
etchPlayers()] call.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMediaPlayer`:`:FetchStatus`(`): [@(0.0.255) bool] [* FetchStatus]()&]
[s2;%% Requests the current [C@5 PlaybackStatus ]property from the 
targeted player. Returns true on success. .&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMediaPlayer`:`:GetStatus`(`)const: Status [* GetStatus]() 
[@(0.0.255) const]&]
[s2;%% Returns the playback status retrieved by the last successful 
[C^topic`:`/`/DBusLib`/src`/DBusLib`_MediaPlayer`_en`-us`#Upp`:`:DBusMediaPlayer`:`:FetchStatus`(`)^ F
etchStatus()] call.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMediaPlayer`:`:FetchMetaData`(`): [@(0.0.255) bool] 
[* FetchMetaData]()&]
[s2;%% Requests the [C@5 Metadata ]dictionary from the targeted player. 
Returns true on success.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMediaPlayer`:`:GetMetaData`(`)const: MetaData [* GetMetaData]() 
[@(0.0.255) const]&]
[s2;%% Parses and returns the metadata dictionary retrieved by the 
last successful [C^topic`:`/`/DBusLib`/src`/DBusLib`_MediaPlayer`_en`-us`#Upp`:`:DBusMediaPlayer`:`:FetchMetaData`(`)^ F
etchMetaData()] call as a MetaData struct.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMediaPlayer`:`:GetRawMetaData`(`)const: DBusValueMap 
[* GetRawMetaData]() [@(0.0.255) const]&]
[s2;%% Returns the raw D`-Bus dictionary (a`{sv`}) retrieved by the 
last successful [C^topic`:`/`/DBusLib`/src`/DBusLib`_MediaPlayer`_en`-us`#Upp`:`:DBusMediaPlayer`:`:FetchMetaData`(`)^ F
etchMetaData()] call, useful for extracting non`-standard or 
vendor`-specific metadata fields.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMediaPlayer`:`:FetchPosition`(`): [@(0.0.255) bool] 
[* FetchPosition]()&]
[s2;%% Requests the current playback [C@5 Position ]property. Returns 
true on success. .&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMediaPlayer`:`:GetPosition`(`)const: [@(0.128.128) int64 
][* GetPosition]() [@(0.0.255) const]&]
[s2;%% Returns the playback position (in microseconds) retrieved 
by the last successful [C^topic`:`/`/DBusLib`/src`/DBusLib`_MediaPlayer`_en`-us`#Upp`:`:DBusMediaPlayer`:`:FetchPosition`(`)^ F
etchPosition()] call.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMediaPlayer`:`:FetchVolume`(`): [@(0.0.255) bool] [* FetchVolume]()&]
[s2;%% Requests the current [C@5 Volume ]property. Returns true on 
success. .&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMediaPlayer`:`:GetVolume`(`)const: [@5 double ][* GetVolume]() 
[@(0.0.255) const]&]
[s2;%% Returns the volume level (typically [C@5 0.0] to [C@5 1.0]) retrieved 
by the last successful [^topic`:`/`/DBusLib`/src`/DBusLib`_MediaPlayer`_en`-us`#Upp`:`:DBusMediaPlayer`:`:FetchVolume`(`)^ F
etchVolume()] call.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMediaPlayer`:`:SetVolume`(double`): [@(0.0.255) bool] 
[* SetVolume]([@(0.0.255) double] [*@3 vol])&]
[s2;%% Directly sets the player`'s volume to [%-*@3 vol]. Returns true 
if the property update command was successful.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMediaPlayer`:`:FetchRate`(`): [@(0.0.255) bool] [* FetchRate]()&]
[s2;%% Requests the current playback [C@5 Rate ]property (multiplier, 
where [C@5 1.0] is normal speed). Returns true on success.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMediaPlayer`:`:GetRate`(`)const: [@(0.0.255) double] 
[* GetRate]() [@(0.0.255) const]&]
[s2;%% Returns the playback rate retrieved by the last successful 
[C^topic`:`/`/DBusLib`/src`/DBusLib`_MediaPlayer`_en`-us`#Upp`:`:DBusMediaPlayer`:`:FetchRate`(`)^ F
etchRate()] call.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMediaPlayer`:`:Raise`(`): [@(0.0.255) bool] [* Raise]()&]
[s2;%% Commands the media player`'s graphical user interface to bring 
itself to the foreground. Returns true on success.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMediaPlayer`:`:Quit`(`): [@(0.0.255) bool] [* Quit]()&]
[s2;%% Commands the media player application to gracefully exit. 
Returns true on success.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMediaPlayer`:`:Play`(`): [@(0.0.255) bool] [* Play]()&]
[s2;%% Commands the player to start or resume playback. Return true 
on success.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMediaPlayer`:`:Pause`(`): [@(0.0.255) bool] [* Pause]()&]
[s2;%% Commands the player to pause playback. Return true on success.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMediaPlayer`:`:PlayPause`(`): [@(0.0.255) bool] [* PlayPause]()&]
[s2;%% Commands the player to toggle between playing and paused states. 
Returns true on success.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMediaPlayer`:`:Stop`(`): [@(0.0.255) bool] [* Stop]()&]
[s2;%% Commands the player to stop playback entirely. Return true 
on success.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMediaPlayer`:`:Prev`(`): [@(0.0.255) bool] [* Prev]()&]
[s2;%% Commands the player to skip to the previous track. Returns 
true on success&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMediaPlayer`:`:Next`(`): [@(0.0.255) bool] [* Next]()&]
[s2;%% Commands the player to skip to the next track. Returns true 
on success&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMediaPlayer`:`:Seek`(int64`): [@(0.0.255) bool] [* Seek](int64 
[*@3 offset])&]
[s2;%% Commands the player to seek forward or backward by the specified 
[%-*@3 offset] (in microseconds) relative to the current position. 
Returns true on success.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMediaPlayer`:`:SetPosition`(const String`&`,int64`): [@(0.0.255) bool] 
[* SetPosition]([@(0.0.255) const] String[@(0.0.255) `&] [*@3 trackid], 
int64 [*@3 pos])&]
[s2;%% Commands the player to set the playback position strictly 
to [%-*@3 pos ](in microseconds) for the specific track identified 
by [%-*@3 trackid]. Return true on success.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusMediaPlayer`:`:OpenUri`(const String`&`): [@(0.0.255) bool] 
[* OpenUri]([@(0.0.255) const] String[@(0.0.255) `&] [*@3 uri])&]
[s2;%% Commands the player to open and begin playing the specified 
media [%-*@3 uri]. Returns true on success.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Structs and Enums]]}}&]
[s3; &]
[s5;:Upp`:`:DBusMediaPlayer`:`:Status: [@(0.0.255)3 enum][3  ][*3 DBusMediaPlayer`::Status]&]
[s2; Represents the current playback state of the media player. Values 
are [C@5 Stopped], [C@5 Playing], [C@5 Paused], and [C@5 Unknown].&]
[s3;%% &]
[s4;%% &]
[s1;:Upp`:`:DBusMediaPlayer`:`:MetaData: [@(0.0.255)3 struct][3  ][*3 DBusMediaPlayer`::Meta
Data]&]
[s2; A simple structure that maps standard MPRIS metadata attributes 
into native types. Contains fields for track details ([C@5 title], 
[C@5 album], [C@5 trackid], [C@5 url], [C@5 length]), artist information 
([C@5 artist], [C@5 albumartist], [C@5 composer], [C@5 lyricist]), and 
metrics ([C@5 usecount], [C@5 userrating], [C@5 audiobpm]). Lists are 
represented as [C@5 Vector<String>]&]
[s3; &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 D`-Bus MPRIS Constants]]}}&]
[s3; &]
[s2; This group of constants defines the fundamental strings specified 
by the MPRIS standard. They provide the necessary routing for 
discovering, querying, and controlling media players over D`-Bus, 
standardizing the interfaces across different applications like 
Spotify, VLC, and Rhythmbox.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:StdDBusMprisPath: [@(0.0.255) constexpr] [@(0.0.255) const] 
[@(0.0.255) char] [@(0.0.255) `*][* StdDBusMprisPath] `= [C `"][C@5 /org/mpris/MediaPlayer2
][C `"]&]
[s2;%% The standard object path implemented by all MPRIS`-compatible 
media players. Used as the target path when sending method calls 
or querying properties.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:StdDBusMprisRoot: [@(0.0.255) constexpr] [@(0.0.255) const] 
[@(0.0.255) char] [@(0.0.255) `*][* StdDBusMprisRoot ]`= [C `"][C@5 org.mpris.MediaPlayer2][C `"
]&]
[s2;%% The root interface for MPRIS players. It provides basic application 
identity, capabilities (such as supported URI schemes and MIME 
types), and global application control (like Quit and Raise). 
.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:StdDBusMprisPlayer: [@(0.0.255) constexpr] [@(0.0.255) const] 
[@(0.0.255) char] [@(0.0.255) `*][* StdDBusMprisPlayer ]`= [C `"][C@5 org.mpris.MediaPlayer
2.Player][C `"]&]
[s2;%% The core playback interface. Used to invoke standard media 
control methods (such as Play, Pause, Next, Seek) and to access 
dynamic playback properties (such as Volume, Metadata, Position, 
and PlaybackStatus). .&]
[s3; &]
[s4; &]
[s5;:Upp`:`:StdDBusMprisTrackList: [@(0.0.255) constexpr] [@(0.0.255) const] 
[@(0.0.255) char] [@(0.0.255) `*][* StdDBusMprisTrackList ]`= [C `"][C@5 org.mpris.MediaPla
yer2.TrackList][C `"]&]
[s2;%% The standard interface for managing a player`'s current tracklist 
or play queue. Used to add tracks, remove tracks, or jump to 
specific tracks within the active queue.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:StdDBusMprisPlaylists: [@(0.0.255) constexpr] [@(0.0.255) const] 
[@(0.0.255) char] [@(0.0.255) `*][* StdDBusMprisPlaylists ]`= [C `"][C@5 org.mpris.MediaPla
yer2.Playlists][C `"]&]
[s2;%% The standard interface for playlist management, allowing remote 
clients to query available saved playlists and activate them.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:StdDBusMprisPrefix: [@(0.0.255) constexpr] [@(0.0.255) const] 
[@(0.0.255) char] [@(0.0.255) `*][* StdDBusMprisPrefix ]`= [C `"][C@5 org.mpris.MediaPlayer
2.][C `"]&]
[s2;%% The standard well`-known bus name prefix for all MPRIS media 
players. Essential for service discovery, as media players append 
their application name to this string (e.g., `"[C@5 org.mpris.MediaPlayer2.vlc]`" 
or `"[C@5 org.mpris.MediaPlayer2.spotify]`"). Used to filter active 
D`-Bus services to find controllable players.&]
[s3; &]
[s2;%% ]]