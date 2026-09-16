topic "DBusFileSelector";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 DBusFileSelector]]}}&]
[s3; &]
[s1;:Upp`:`:DBusFileSelector: [@(0.0.255)3 class][3  ][*3 DBusFileSelector]&]
[s2;%% This class provides a native, file and directory selection 
dialog by interfacing directly with the XDG Desktop Portal ([C@5 org.freedesktop.po
rtal.FileChooser]) over D`-Bus.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor detail]]}}&]
[s3; &]
[s5;:Upp`:`:DBusFileSelector`:`:DBusFileSelector`(DBusConnection`&`): [* DBusFileSelect
or](DBusConnection[@(0.0.255) `&] [*@3 dbus])&]
[s2;%% Initializes the file selector to operate over the specified 
[%-*@3 dbus] connection. The connection must already be established 
and the [^topic`:`/`/DBus`/src`/Upp`_DBusConnection`_en`-us`#Upp`:`:DBusConnection^ D
BusConnection] object must outlive the file selector and it must 
be in blocking mode.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Upp`:`:DBusFileSelector`:`:Type`(const String`&`,const String`&`): DBusFileSelec
tor[@(0.0.255) `&] [* Type]([@(0.0.255) const] String[@(0.0.255) `&] 
[*@3 desc], [@(0.0.255) const] String[@(0.0.255) `&] [*@3 pattern])&]
[s2;%% Adds a file extension filter to the dialog. [%-*@3 desc] is 
the description shown to the user (e.g., `"[C@5 Images]`"), and 
[%-*@3 pattern] is a space`-separated list of wildcard patterns 
(e.g., `"[C@5 `*.png `*.jpg]`"). Returns `*this for method chaining.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusFileSelector`:`:AllFileTypes`(`): DBusFileSelector[@(0.0.255) `&] 
[* AllFileTypes]()&]
[s2;%% Adds a default `"All Files`" filter with the [C@5 `*] wildcard. 
Returns `*this for method chaining.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusFileSelector`:`:ClearTypes`(`): DBusFileSelector[@(0.0.255) `&] 
[* ClearTypes]()&]
[s2;%% Clears all previously registered file type filters. Returns 
`*this for method chaining.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusFileSelector`:`:ActiveDir`(const String`&`): DBusFileSelector[@(0.0.255) `&
] [* ActiveDir]([@(0.0.255) const] String[@(0.0.255) `&] [*@3 dir])&]
[s2;%% Sets the initial directory [%-*@3 dir] that the dialog will 
display when opened. Returns `*this for method chaining.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusFileSelector`:`:ActiveFile`(const String`&`): DBusFileSelector[@(0.0.255) `&
] [* ActiveFile]([@(0.0.255) const] String[@(0.0.255) `&] [*@3 name])&]
[s2;%% Sets the initial default file [%-*@3 name] populated in the 
dialog`'s input field. Returns `*this for method chaining.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusFileSelector`:`:Multi`(bool`): DBusFileSelector[@(0.0.255) `&] 
[* Multi]([@(0.0.255) bool] [*@3 b] [@(0.0.255) `=] [@(0.0.255) true])&]
[s2;%% Enables or disable selecting multiple files in the dialog. 
Returns `*this for method chaining.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusFileSelector`:`:ExecuteOpen`(const String`&`): [@(0.0.255) bool] 
[* ExecuteOpen]([@(0.0.255) const] String[@(0.0.255) `&] [*@3 title] 
[@(0.0.255) `=] [@(0.128.128) Null])&]
[s6;%% Requires an open D`-Bus connection in blocking mode.&]
[s2;%% Executes the dialog in `"[C@5 Open]`" mode. [%-*@3 title] optionally 
overrides the default dialog window title. Returns true if the 
user successfully selected one or more files, or false if the 
dialog was cancelled or an error occurred.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusFileSelector`:`:ExecuteSaveAs`(const String`&`): [@(0.0.255) bool] 
[* ExecuteSaveAs]([@(0.0.255) const] String[@(0.0.255) `&] [*@3 title] 
[@(0.0.255) `=] [@(0.128.128) Null])&]
[s6;%% Requires an open D`-Bus connection in blocking mode.&]
[s2;%% Executes the dialog in `"[C@5 Save As]`" mode. [%-*@3 title] optionally 
overrides the default dialog window title. Returns true if the 
user successfully selected destination path, or false if the 
dialog was cancelled or an error occurred.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusFileSelector`:`:ExecuteSelectDir`(const String`&`): [@(0.0.255) bool] 
[* ExecuteSelectDir]([@(0.0.255) const] String[@(0.0.255) `&] [*@3 title] 
[@(0.0.255) `=] [@(0.128.128) Null])&]
[s6;%% Requires an open D`-Bus connection in blocking mode.&]
[s2;%% Executes the dialog in `"Directory Selection`" mode, allowing 
the user to pick a folder instead of a file. [%-*@3 title] optionally 
overrides the default dialog window title. Returns true if a 
folder was selected.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusFileSelector`:`:Get`(`)const: String [* Get]() [@(0.0.255) const]&]
[s2;%% Returns the absolute path of the first selected file or directory. 
Returns Null if no selection was made or the dialog was cancelled.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusFileSelector`:`:GetFiles`(`)const: [@(0.0.255) const] 
Vector<String>[@(0.0.255) `&] [* GetFiles]() [@(0.0.255) const]&]
[s2;%% Returns a vector containing the absolute paths of all selected 
files. Useful when [C^topic`:`/`/DBusLib`/src`/DBusLib`_FileSelector`_en`-us`#Upp`:`:DBusFileSelector`:`:Multi`(bool`)^ M
ulti()] is enabled.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Function List]]}}&]
[s3;%% &]
[s0;l288;%% The following convenience functions provide a quick, 
one`-line API for the most common file selection tasks, closely 
mirroring the standard CtrlLib file selector globals.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:SelectOpenFile`(DBusConnection`&`,const String`&`): String 
[* SelectOpenFile](DBusConnection[@(0.0.255) `&] [*@3 dbus], [@(0.0.255) const] 
String[@(0.0.255) `&] [*@3 types] [@(0.0.255) `=] [@(0.128.128) Null])&]
[s6;%% Requires an open D`-Bus connection in blocking mode.&]
[s2;%% Prompts the user to select a single file to open. Returns 
the selected file path, or Null if cancelled. The [%-*@3 types][%-  
]string format expects descriptions and patterns separated by 
a tab ([C@5 `\t]), and multiple filters separated by a newline 
([C@5 `\n]). Example: `"[C@5 Images][C@6 `\t][C@5 `*.png `*.jpg][C@6 `\n][C@5 Text 
Files][C@6 `\t][C@5 `*.txt]`"&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SelectOpenFiles`(DBusConnection`&`,const String`&`): Vector<String> 
[* SelectOpenFiles](DBusConnection[@(0.0.255) `&] [*@3 dbus], [@(0.0.255) const] 
String[@(0.0.255) `&] [*@3 types] [@(0.0.255) `=] Null)&]
[s6;%% Requires an open D`-Bus connection in blocking mode.&]
[s2;%% Prompts the user to select multiple files to open. Returns 
a vector of strings containing all selected paths on success. 
The [%-*@3 types][%-  ]string format expects descriptions and patterns 
separated by a tab ([C@5 `\t]), and multiple filters separated 
by a newline ([C@5 `\n]). Example: `"[C@5 Images][C@6 `\t][C@5 `*.png 
`*.jpg][C@6 `\n][C@5 Text Files][C@6 `\t][C@5 `*.txt]`"&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SelectLoadFile`(DBusConnection`&`,const String`&`): String 
[* SelectLoadFile](DBusConnection[@(0.0.255) `&] [*@3 dbus], [@(0.0.255) const] 
String[@(0.0.255) `&] [*@3 types] [@(0.0.255) `=] [@(0.128.128) Null])&]
[s6;%% Requires an open D`-Bus connection in blocking mode.&]
[s2;%% Prompts the user to select a file, reads the entire file into 
memory, and returns its contents as a String. Returns Null if 
the dialog was cancelled or the file could not be read. The [%-*@3 types][%-  
]string format expects descriptions and patterns separated by 
a tab ([C@5 `\t]), and multiple filters separated by a newline 
([C@5 `\n]). Example: `"[C@5 Images][C@6 `\t][C@5 `*.png `*.jpg][C@6 `\n][C@5 Text 
Files][C@6 `\t][C@5 `*.txt]`"&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SelectSaveFile`(DBusConnection`&`,const String`&`,const String`&`): [@(0.0.255) b
ool] [* SelectSaveFile](DBusConnection[@(0.0.255) `&] [*@3 dbus], [@(0.0.255) const] 
String[@(0.0.255) `&] [*@3 data], [@(0.0.255) const] String[@(0.0.255) `&] 
[*@3 types] [@(0.0.255) `=] [@(0.128.128) Null])&]
[s6;%% Requires an open D`-Bus connection in blocking mode.&]
[s2;%% Prompts the user for a destination path and saves data to 
that location. Returns true if the file was successfully saved, 
or false if cancelled or a write error occurred. The [%-*@3 types][%-  
]string format expects descriptions and patterns separated by 
a tab ([C@5 `\t]), and multiple filters separated by a newline 
([C@5 `\n]). Example: `"[C@5 Images][C@6 `\t][C@5 `*.png `*.jpg][C@6 `\n][C@5 Text 
Files][C@6 `\t][C@5 `*.txt]`"&]
[s3; &]
[s4; &]
[s5;:Upp`:`:SelectDirectory`(DBusConnection`&`): String [* SelectDirectory](DBusConnect
ion[@(0.0.255) `&] [*@3 dbus])&]
[s6;%% Requires an open D`-Bus connection in blocking mode.&]
[s2;%% Prompts the user to select a directory/folder. Returns the 
selected directory path, or Null if cancelled.&]
[s3; &]
[s0; &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 D`-Bus FileChooser (Desktop Portal) Constants]]}}&]
[s3;%% &]
[s2;%% This group of constants defines the fundamental strings specified 
by the XDG Desktop Portal standard ([C@5 org.freedesktop.portal.Desktop]) 
for file dialogs. They provide the required routing for invoking 
native, sandbox`-safe file and directory selection dialogs across 
modern Linux desktop environments (such as GNOME and KDE).&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:StdDBusFileChooserName: [@(0.0.255) constexpr] [@(0.0.255) const] 
[@(0.0.255) char] [@(0.0.255) `*][* StdDBusFileChooserName ]`= [C `"][C@5 org.freedesktop.p
ortal.Desktop][C `"]&]
[s2;%% The well`-known bus name of the XDG Desktop Portal service. 
Used as the destination service name when routing native UI requests 
on the session bus.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:StdDBusFileChooserPath: [@(0.0.255) constexpr] [@(0.0.255) const] 
[@(0.0.255) char] [@(0.0.255) `*][* StdDBusFileChooserPath ]`= [C `"/][C@5 org/freedesktop/
portal/desktop][C `"]&]
[s2;%% The standard object path implemented by the portal daemon 
where desktop interaction interfaces are exposed.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:StdDBusFileChooserInterface: [@(0.0.255) constexpr] [@(0.0.255) const] 
[@(0.0.255) char] [@(0.0.255) `*][* StdDBusFileChooserInterface ]`= 
[C `"][C@5 org.freedesktop.portal.Desktop][C `"]&]
[s2;%% The core interface implemented by the portal daemon for file 
selection operations. Used to invoke standard file chooser methods, 
including [C@5 OpenFile ]and [C@5 SaveFile].&]
[s3; ]]