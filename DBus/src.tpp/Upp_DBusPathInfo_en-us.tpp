topic "DBusPathInfo";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 DBusPathInfo]]}}&]
[s3; &]
[s1;:Upp`:`:DBusPathInfo: [@(0.0.255)3 struct][3  ][*3 DBusPathInfo]&]
[s2; A helper structure designed to parse and store the individual 
components of a standard D`-Bus address string. It separates 
the transport method (such as [C@5 unix], [C@5 tcp], or [C@5 nonce`-tcp]) 
from its associated configuration keys (like [C@5 host], [C@5 port], 
[C@5 path], or [C@5 noncefile])&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Member Variables]]}}&]
[s3; &]
[s5;:Upp`:`:DBusPathInfo`:`:path: String [* path]&]
[s2;%% Stores the original, unparsed D`-Bus address string.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusPathInfo`:`:method: String [* method]&]
[s2;%% Stores the transport method extracted from the address. Common 
values include `"[%-C@5 unix]`" (for local domain sockets), `"[%-C@5 tcp]`" 
(for standard network sockets), and `"[%-C@5 nonce`-tcp]`" (for 
authenticated Windows connections). .&]
[s3;*@3 &]
[s4; &]
[s5;:Upp`:`:DBusPathInfo`:`:parameters: VectorMap<String, String> 
[* parameters]&]
[s2;%% A key`-value map containing the connection parameters parsed 
from the address. For example, a nonce`-tcp address will populate 
this map with `"[C@5 host]`", `"[C@5 port]`", and `"[C@5 noncefile]`" 
keys.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor detail]]}}&]
[s3; &]
[s5;:Upp`:`:DBusPathInfo`:`:DBusPathInfo`(`): [* DBusPathInfo]()&]
[s2;%% Default constructor.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusPathInfo`:`:DBusPathInfo`(const String`&`): [* DBusPathInfo]([@(0.0.255) c
onst] String[@(0.0.255) `&] [*@3 addr])&]
[s2;%% Initializes the structure and immediately calls Parse() on 
the provided D`-Bus address string [%-*@3 addr].&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Method List]]}}&]
[s3; &]
[s5;:Upp`:`:DBusPathInfo`:`:Parse`(const String`&`): [@(0.0.255) void] 
[* Parse]([@(0.0.255) const] String[@(0.0.255) `&] [*@3 addr])&]
[s2;%% Parses the provided D`-Bus address string. It extracts the 
transport method (the portion before the colon) and populates 
the parameters map with the comma`-separated key`-value pairs 
that follow.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusPathInfo`:`:operator`[`]`(const char`*`)const: String 
operator[@(0.0.255) `[`]]([@(0.0.255) const] [@(0.0.255) char] [@(0.0.255) `*][*@3 id]) 
[@(0.0.255) const]&]
[s2;%% A convenience operator to retrieve the value of a specific 
connection parameter by its key (e.g., [C@5 info`[`"port`"`]] or 
[C@5 info`[`"noncefile`"`]]). Returns a void string if the requested 
key does not exist in the address.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusPathInfo`:`:Clear`(`): [@(0.0.255) void] [* Clear]()&]
[s2;%% Clears the stored [C^topic`:`/`/DBus`/src`/Upp`_DBusPathInfo`_en`-us`#Upp`:`:DBusPathInfo`:`:path^ p
ath], [C^topic`:`/`/DBus`/src`/Upp`_DBusPathInfo`_en`-us`#Upp`:`:DBusPathInfo`:`:method^ m
ethod], and all [C^topic`:`/`/DBus`/src`/Upp`_DBusPathInfo`_en`-us`#Upp`:`:DBusPathInfo`:`:parameters^ p
arameters], resetting the structure to its default, empty state. 
&]
[s0;%% ]]