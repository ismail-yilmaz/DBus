topic "DBusValue";
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
[ {{10000@(113.42.0) [s0;%% [*@7;4 DBusValue]]}}&]
[s3; &]
[s1;:Upp`:`:DBusValue: [@(0.0.255)3 class][3  ][*3 DBusValue][3  ][@(0.0.255)3 :][3  
][@(0.0.255)3 public][3  Moveable<][*3 DBusValue][3 >]&]
[s2;%% This class is a  type`-safe wrapper around the standard [^topic`:`/`/Core`/src`/Value`_en`-us`#Value`:`:class^ V
alue] container. It provides a strict typing bridge for D`-Bus 
payload marshaling. It explicitly intercepts sized integer types 
([@(0.128.128) byte], [@(0.128.128) int16], [@(0.128.128) uint16], 
[@(0.128.128) uint32], [@(0.128.128) uint64]) and forces them into 
[^topic`:`/`/Core`/src`/Value`_en`-us`#RawToValue`(const T`&`)^ raw 
value format], preventing them from decaying into standard integers 
during implicit conversions. This ensures exact mapping to D`-Bus 
binary signatures (e.g., `'y`', `'n`', `'q`', `'u`', `'t`').&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Constructor detail]]}}&]
[s4; &]
[s5;:Upp`:`:DBusValue`:`:DBusValue`(`): [* DBusValue]()&]
[s2;%% Default constructor.&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusValue`:`:DBusValue`(byte`): [* DBusValue]([@(0.128.128) byte 
][*@3 v])&]
[s5;:Upp`:`:DBusValue`:`:DBusValue`(int16`): [* DBusValue]([@(0.128.128) int16 
][*@3 v])&]
[s5;:Upp`:`:DBusValue`:`:DBusValue`(uint16`): [* DBusValue]([@(0.128.128) uint16 
][*@3 v])&]
[s5;:Upp`:`:DBusValue`:`:DBusValue`(int32`): [* DBusValue]([@(0.128.128) int32 
][*@3 v])&]
[s5;:Upp`:`:DBusValue`:`:DBusValue`(uint32`): [* DBusValue]([@(0.128.128) uint32 
][*@3 v])&]
[s5;:Upp`:`:DBusValue`:`:DBusValue`(int64`): [* DBusValue]([@(0.128.128) int64 
][*@3 v])&]
[s5;:Upp`:`:DBusValue`:`:DBusValue`(uint64`): [* DBusValue]([@(0.128.128) uint64 
][*@3 v])&]
[s0;:Upp`:`:DBusValue`:`:DBusValue`(bool`): [* DBusValue]([@(0.0.255) bool] 
[*@3 v])&]
[s5;:Upp`:`:DBusValue`:`:DBusValue`(double`): [* DBusValue]([@(0.0.255) double] 
[*@3 v])&]
[s5;:Upp`:`:DBusValue`:`:DBusValue`(const String`&`): [* DBusValue]([@(0.0.255) const] 
String[@(0.0.255) `&] [*@3 v])&]
[s5;:Upp`:`:DBusValue`:`:DBusValue`(const char`*`): [* DBusValue]([@(0.0.255) const] 
[@(0.0.255) char] [@(0.0.255) `*][*@3 s])&]
[s5;:Upp`:`:DBusValue`:`:DBusValue`(const Nuller`&`): [* DBusValue]([@(0.0.255) const] 
Nuller[@(0.0.255) `&] [*@3 v])&]
[s5;:Upp`:`:DBusValue`:`:DBusValue`(const DBusValueMap`&`): [* DBusValue]([@(0.0.255) con
st] DBusValueMap[@(0.0.255) `&] [*@3 v])&]
[s5;:Upp`:`:DBusValue`:`:DBusValue`(const DBusValueArray`&`): [* DBusValue]([@(0.0.255) c
onst] DBusValueArray[@(0.0.255) `&] [*@3 v])&]
[s5;:Upp`:`:DBusValue`:`:DBusValue`(const DBusValueStruct`&`): [* DBusValue]([@(0.0.255) c
onst] DBusValueStruct[@(0.0.255) `&] [*@3 v])&]
[s2; Constructs a DBusValue of given type.&]
[s3; &]
[ {{10000F(128)G(128)@1 [s0;%% [* Public Method List]]}}&]
[s3; &]
[s5;:Upp`:`:DBusValue`:`:IsNullInstance`(`)const: [@(0.0.255) bool] 
[* IsNullInstance]() [@(0.0.255) const]&]
[s2;%% Returns true if the underlying Value is Null.&]
[s3;%% &]
[s4; &]
[s5;:Upp`:`:DBusValue`:`:Is`(`)const: [@(0.0.255) template] <[@(0.0.255) typename] 
[%%*@(0.128.128) T]> [@(0.0.255) bool] [* Is]() [@(0.0.255) const]&]
[s2;%% Checks if the underlying boxed value matches the requested 
type [*@(0.128.128) T].&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusValue`:`:To`(`)const: [@(0.0.255) template] <[@(0.0.255) typename] 
[%%*@(0.128.128) T]> [@(0.0.255) const] [%%*@(0.128.128) T][@(0.0.255) `&] 
[* To]() [@(0.0.255) const]&]
[s2;%% Extracts the underlying value as the specified type [*@(0.128.128) T]. 
If the requested type [*@(0.128.128) T] does not exactly match 
the boxed type, this will trigger a run`-time assertion failure. 
&]
[s3; &]
[s4; &]
[s5;:Upp`:`:DBusValue`:`:operator`(`)const: [@(0.0.255) template] <[@(0.0.255) typename] 
[*@(0.128.128) T]> operator [*@(0.128.128) T]() [@(0.0.255) const]&]
[s2;%% Implicit conversion operator. Safely extracts the underlying 
value[^topic`:`/`/Core`/src`/Value`_en`-us`#Value`:`:class^  ]into 
the target type [*@(0.128.128) T ]using [^topic`:`/`/Core`/src`/Value`_en`-us`#Value`:`:To`(`)const^ V
alue`::To<T>()] method. If the requested type [*@(0.128.128) T] 
does not exactly match the boxed type, this will trigger a runtime 
assertion failure. &]
[s3;%% &]
[s0;%% &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 DBusValueArray]]}}&]
[s3; &]
[s1;:Upp`:`:DBusValueArray: [@(0.0.255)3 struct][3  ][*3 DBusValueArray][3  
][@(0.0.255)3 :][3  ][@(0.0.255)3 public][3  Vector<DBusValue>]&]
[s2;%% A type`-safe [^topic`:`/`/DBus`/src`/Upp`_DBusValue`_en`-us`#Upp`:`:DBusValue^ D
BusValue ]array container for D`-Bus lists (e.g., as, ai, av).&]
[s3; &]
[s0;%% &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 DBusValueMap]]}}&]
[s3;%% &]
[s1;:Upp`:`:DBusValueMap: [@(0.0.255)3 struct][3  ][*3 DBusValueMap][3  ][@(0.0.255)3 :][3  
][@(0.0.255)3 public][3  VectorMap<String, DBusValue>]&]
[s2;%% A type`-safe [^topic`:`/`/DBus`/src`/Upp`_DBusValue`_en`-us`#Upp`:`:DBusValue^ D
BusValue ]dictionary container for D`-Bus variant maps (typically 
a`{sv`}).&]
[s3; &]
[s0;%% &]
[ {{10000@(113.42.0) [s0;%% [*@7;4 DBusValueStruct]]}}&]
[s3; &]
[s1;:Upp`:`:DBusValueStruct: [@(0.0.255)3 struct][3  ][*3 DBusValueStruct][3  
][@(0.0.255)3 :][3  ][@(0.0.255)3 public][3  Vector<DBusValue>]&]
[s2;%% A type sage [^topic`:`/`/DBus`/src`/Upp`_DBusValue`_en`-us`#Upp`:`:DBusValue^ DB
usValue ]struct container D`-Bus structures (is).&]
[s3; &]
[s0;%% ]]