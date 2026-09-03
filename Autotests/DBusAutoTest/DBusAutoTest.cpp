#include <DBus/DBus.h>

using namespace Upp;

int passed_tests = 0;
int failed_tests = 0;

#define TEST_CHECK(expr) \
    do { \
        if(expr) { passed_tests++; } \
        else { \
            failed_tests++; \
            RLOG("[FAILED] " << #expr << " at line " << __LINE__); \
        } \
    } while(0)

#define TEST_CHECK_EQ(a, b) \
    do { \
        auto _a = (a); auto _b = (b); \
        if(_a == _b) { passed_tests++; } \
        else { \
            failed_tests++; \
            RLOG("[FAILED] " << #a << " == " << #b << " (" << _a << " != " << _b << ") at line " << __LINE__); \
        } \
    } while(0)


void TestDBusValueTypes()
{
    RLOG("Running DBusValue Type Retention Tests...");

    DBusValue v_byte((byte) 255);
    TEST_CHECK(v_byte.Is<byte>());
    TEST_CHECK_EQ(v_byte.To<byte>(), (byte)255);

    DBusValue v_int16((int16) -32000);
    TEST_CHECK(v_int16.Is<int16>());
    TEST_CHECK_EQ(v_int16.To<int16>(), (int16)-32000);

    DBusValue v_word((word) 65000);
    TEST_CHECK(v_word.Is<word>());
    TEST_CHECK_EQ(v_word.To<word>(), (word)65000);

    DBusValue v_dword((dword) 123456789);
    TEST_CHECK(v_dword.Is<dword>());
    TEST_CHECK_EQ(v_dword.To<dword>(), (dword)123456789);

    DBusValue v_u64((uint64) 0x1234567890ABCDEF);
    TEST_CHECK(v_u64.Is<uint64>());
    TEST_CHECK_EQ(v_u64.To<uint64>(), (uint64)0x1234567890ABCDEF);

    DBusValue v_str("U++ Framework");
    TEST_CHECK(v_str.Is<String>());
    TEST_CHECK_EQ(v_str.To<String>(), "U++ Framework");
    
    DBusValue v_null;
    TEST_CHECK(IsNull(v_null));
}

void TestMessagePrimitives()
{
    RLOG("Running DBusMessage Primitive Round-Trip Tests...");

    DBusValueArray args = {
        DBusValue("Terminal"),
        DBusValue((byte) 200),
        DBusValue((dword) 42),
        DBusValue((double) 3.14159),
        DBusValue(true)
    };

    DBusMessage msg = DBusMessage::CreateMethodCall(100, "com.bobcat", "/core", "com.bobcat.App", "Launch", args);

    TEST_CHECK(msg.IsOK());
    TEST_CHECK(msg.IsMethodCall());
    TEST_CHECK_EQ(msg.GetSerial(), (dword)100);

    DBusMessage parsed(msg.GetRawData());
    
    TEST_CHECK(parsed.IsMethodCall());
    TEST_CHECK_EQ(parsed.GetSerial(), (dword)100);

    DBusMessage::FieldData fields = parsed.ParseFields();
    TEST_CHECK_EQ(fields.destination, "com.bobcat");
    TEST_CHECK_EQ(fields.path, "/core");
    TEST_CHECK_EQ(fields.interface, "com.bobcat.App");
    TEST_CHECK_EQ(fields.member, "Launch");
    TEST_CHECK_EQ(fields.signature, "syudb");

    DBusValueArray parsed_args = parsed.ParseBody();
    TEST_CHECK_EQ(parsed_args.GetCount(), 5);
    
    TEST_CHECK_EQ(parsed_args[0].To<String>(), "Terminal");
    TEST_CHECK_EQ(parsed_args[1].To<byte>(), (byte)200);
    TEST_CHECK_EQ(parsed_args[2].To<dword>(), (dword)42);
    TEST_CHECK_EQ(parsed_args[3].To<double>(), 3.14159);
    TEST_CHECK_EQ(parsed_args[4].To<bool>(), true);
}

void TestComplexContainers()
{
    RLOG("Running DBusMessage Complex Container (Map/Array) Tests...");

    DBusValueMap map;
    map.Add("WindowName", DBusValue("MainTerm"));
    map.Add("PID", DBusValue((dword) 9876));
    
    DBusValueArray arr;
    arr.Add(DBusValue((int64) -123456789));
    arr.Add(DBusValue((int64) 987654321));
    
    DBusValueArray args = { DBusValue(map), DBusValue(arr) };
    
    DBusMessage msg = DBusMessage::CreateSignal(101, "/events", "com.System", "StatusChanged", args);
    
    DBusMessage parsed(msg.GetRawData());
    DBusValueArray parsed_args = parsed.ParseBody();
    
    TEST_CHECK_EQ(parsed_args.GetCount(), 2);
    
    // Validate Map (Dictionary)
    TEST_CHECK(parsed_args[0].Is<DBusValueMap>());
    DBusValueMap pmap = parsed_args[0];
    TEST_CHECK_EQ(pmap.GetCount(), 2);
    TEST_CHECK_EQ(pmap.Get("WindowName").To<String>(), "MainTerm");
    TEST_CHECK_EQ(pmap.Get("PID").To<dword>(), (dword)9876);
    
    // Validate Array
    TEST_CHECK(parsed_args[1].Is<DBusValueArray>());
    DBusValueArray parr = parsed_args[1];
    TEST_CHECK_EQ(parr.GetCount(), 2);
    TEST_CHECK_EQ(parr[0].To<int64>(), (int64)-123456789);
    TEST_CHECK_EQ(parr[1].To<int64>(), (int64)987654321);
}

void TestCornerCases()
{
    RLOG("Running Corner Case & Helper Tests...");

    DBusMessage msg_empty = DBusMessage::CreateMethodCall(200, "d", "/p", "i", "m");
    DBusMessage p_empty(msg_empty.GetRawData());
    TEST_CHECK_EQ(p_empty.ParseBody().GetCount(), 0);
    TEST_CHECK(p_empty.ParseFields().signature.IsEmpty());

    DBusMessage msg_estr = DBusMessage::CreateSignal(201, "/p", "i", "s", { DBusValue("") });
    DBusMessage p_estr(msg_estr.GetRawData());
    TEST_CHECK_EQ(p_estr.ParseString(), "");

    DBusValueArray emptyArr;
    DBusMessage msg_earr = DBusMessage::CreateSignal(202, "/p", "i", "s", { DBusValue(emptyArr) });
    DBusMessage p_earr(msg_earr.GetRawData());
    DBusValueArray body_earr = p_earr.ParseBody();
    DBusValueArray p_earr_val = body_earr.GetCount() > 0 ? (DBusValueArray)body_earr[0] : DBusValueArray();
    TEST_CHECK_EQ(p_earr_val.GetCount(), 0);

    DBusValueMap emptyMap;
    DBusMessage msg_emap = DBusMessage::CreateSignal(203, "/p", "i", "s", { DBusValue(emptyMap) });
    DBusMessage p_emap(msg_emap.GetRawData());
    DBusValueArray body_emap = p_emap.ParseBody();
    DBusValueMap p_emap_val = body_emap.GetCount() > 0 ? (DBusValueMap)body_emap[0] : DBusValueMap();
    TEST_CHECK_EQ(p_emap_val.GetCount(), 0);

    DBusValueArray sarr;
    sarr.Add(DBusValue("One"));
    sarr.Add(DBusValue("Two"));
    DBusMessage msg_sarr = DBusMessage::CreateMethodReturn(204, 100, "d", { DBusValue(sarr) });
    DBusMessage p_sarr(msg_sarr.GetRawData());
    Vector<String> parsed_sarr = p_sarr.ParseStringArray();
    TEST_CHECK_EQ(parsed_sarr.GetCount(), 2);
    if(parsed_sarr.GetCount() >= 2) {
        TEST_CHECK_EQ(parsed_sarr[0], "One");
        TEST_CHECK_EQ(parsed_sarr[1], "Two");
    }

    TEST_CHECK(p_emap.MatchRule("type='signal',interface='i'"));
    TEST_CHECK(p_emap.MatchRule("path='/p'"));
    TEST_CHECK(!p_emap.MatchRule("type='method_call'"));
    TEST_CHECK(!p_emap.MatchRule("interface='wrong'"));

    DBusMessage msg_err = DBusMessage::CreateError(205, 100, "d", "org.Test.Error", "File not found");
    DBusMessage p_err(msg_err.GetRawData());
    TEST_CHECK(p_err.IsError());
    TEST_CHECK_EQ(p_err.GetErrorName(), "org.Test.Error");
    TEST_CHECK_EQ(p_err.GetErrorDesc(), "File not found");
}

void TestStructsAndExtremeNesting()
{
    RLOG("Running DBusMessage Struct and Extreme Nesting Tests...");

    // Simple Struct: (is)
    DBusValueStruct s1;
    s1.Add((int32) 1024);
    s1.Add("SimpleStruct");
    
    DBusMessage msg_s1 = DBusMessage::CreateSignal(300, "/struct", "com.test", "Simple", { DBusValue(s1) });
    DBusMessage p_s1(msg_s1.GetRawData());
    
    TEST_CHECK_EQ(p_s1.ParseFields().signature, "(is)");
    DBusValueArray b_s1 = p_s1.ParseBody();
    TEST_CHECK_EQ(b_s1.GetCount(), 1);
    TEST_CHECK(b_s1[0].Is<DBusValueStruct>());
    
    DBusValueStruct ps1 = b_s1[0];
    TEST_CHECK_EQ(ps1.GetCount(), 2);
    TEST_CHECK_EQ(ps1[0].To<int32>(), (int32)1024);
    TEST_CHECK_EQ(ps1[1].To<String>(), "SimpleStruct");

    // The FileChooser Nightmare: a(sa(us))
    DBusValueStruct ext1;
    ext1.Add((uint32) 0);
    ext1.Add("*.obj");

    DBusValueStruct ext2;
    ext2.Add((uint32) 0);
    ext2.Add("*.gltf");

    DBusValueArray extarr;
    extarr.Add(ext1);
    extarr.Add(ext2);

    DBusValueStruct filter;
    filter.Add("3D Models");
    filter.Add(extarr);

    DBusValueArray filters;
    filters.Add(filter);

    DBusMessage msg_f = DBusMessage::CreateMethodCall(301, "org.fd.portal", "/desktop", "org.fd.portal.FileChooser", "OpenFile", { DBusValue(filters) });
    DBusMessage p_f(msg_f.GetRawData());

    TEST_CHECK_EQ(p_f.ParseFields().signature, "a(sa(us))");
    DBusValueArray b_f = p_f.ParseBody();
    TEST_CHECK_EQ(b_f.GetCount(), 1);
    TEST_CHECK(b_f[0].Is<DBusValueArray>());
    
    DBusValueArray pfilters = b_f[0];
    TEST_CHECK_EQ(pfilters.GetCount(), 1);
    TEST_CHECK(pfilters[0].Is<DBusValueStruct>());
    
    DBusValueStruct pfilter = pfilters[0];
    TEST_CHECK_EQ(pfilter.GetCount(), 2);
    TEST_CHECK_EQ(pfilter[0].To<String>(), "3D Models");
    TEST_CHECK(pfilter[1].Is<DBusValueArray>());
    
    DBusValueArray pextarr = pfilter[1];
    TEST_CHECK_EQ(pextarr.GetCount(), 2);
    
    DBusValueStruct pext2 = pextarr[1];
    TEST_CHECK_EQ(pext2[0].To<uint32>(), (uint32)0);
    TEST_CHECK_EQ(pext2[1].To<String>(), "*.gltf");

    // Variant containing a struct inside a map: a{sv} where v is (db)
    DBusValueStruct vstruct;
    vstruct.Add((double) 3.1415);
    vstruct.Add(true);

    DBusValueMap map;
    map.Add("Bounds", DBusValue(vstruct));
    
    DBusMessage msg_m = DBusMessage::CreateMethodReturn(302, 100, "d", { DBusValue(map) });
    DBusMessage p_m(msg_m.GetRawData());
    
    TEST_CHECK_EQ(p_m.ParseFields().signature, "a{sv}");
    DBusValueArray b_m = p_m.ParseBody();
    TEST_CHECK_EQ(b_m.GetCount(), 1);
    TEST_CHECK(b_m[0].Is<DBusValueMap>());
    
    DBusValueMap pmap = b_m[0];
    TEST_CHECK_EQ(pmap.GetCount(), 1);
    
    DBusValue pvar = pmap.Get("Bounds");
    TEST_CHECK(pvar.Is<DBusValueStruct>());
    
    DBusValueStruct pvstruct = pvar;
    TEST_CHECK_EQ(pvstruct.GetCount(), 2);
    TEST_CHECK_EQ(pvstruct[0].To<double>(), (double)3.1415);
    TEST_CHECK_EQ(pvstruct[1].To<bool>(), true);
}

void TestArrayOfArraysNesting()
{
    RLOG("Running Array-of-Arrays Nesting Tests...");

    // aai: array of array of int32 ("matrix"); two rows of different lengths,
    // so a row-boundary offset error can't hide behind symmetric sizes.
    DBusValueArray row1;
    row1.Add((int32) 1);
    row1.Add((int32) 2);
    row1.Add((int32) 3);

    DBusValueArray row2;
    row2.Add((int32) 4);
    row2.Add((int32) 5);

    DBusValueArray matrix;
    matrix.Add(row1);
    matrix.Add(row2);

    DBusMessage msg_mtx = DBusMessage::CreateSignal(310, "/matrix", "com.test", "Grid", { DBusValue(matrix) });
    DBusMessage p_mtx(msg_mtx.GetRawData());

    TEST_CHECK_EQ(p_mtx.ParseFields().signature, "aai");

    DBusValueArray b_mtx = p_mtx.ParseBody();
    TEST_CHECK_EQ(b_mtx.GetCount(), 1);
    TEST_CHECK(b_mtx[0].Is<DBusValueArray>());

    DBusValueArray pmatrix = b_mtx[0];
    TEST_CHECK_EQ(pmatrix.GetCount(), 2);
    TEST_CHECK(pmatrix[0].Is<DBusValueArray>());
    TEST_CHECK(pmatrix[1].Is<DBusValueArray>());

    DBusValueArray prow1 = pmatrix[0];
    TEST_CHECK_EQ(prow1.GetCount(), 3);
    TEST_CHECK_EQ(prow1[0].To<int32>(), (int32)1);
    TEST_CHECK_EQ(prow1[1].To<int32>(), (int32)2);
    TEST_CHECK_EQ(prow1[2].To<int32>(), (int32)3);

    DBusValueArray prow2 = pmatrix[1];
    TEST_CHECK_EQ(prow2.GetCount(), 2);
    TEST_CHECK_EQ(prow2[0].To<int32>(), (int32)4);
    TEST_CHECK_EQ(prow2[1].To<int32>(), (int32)5);

    // aaax: one level deeper still, and switched to int64 leaves so the leaf
    // array's own data also has to re-align to 8 after its length prefix. This
    // makes sure the recursion keeps its offset bookkeeping straight past two
    // levels of array nesting.
    DBusValueArray leaf1;
    leaf1.Add((int64) 10);
    leaf1.Add((int64) 20);

    DBusValueArray leaf2;
    leaf2.Add((int64) 30);

    DBusValueArray mid1;
    mid1.Add(leaf1);
    mid1.Add(leaf2);

    DBusValueArray leaf3;
    leaf3.Add((int64) 40);
    leaf3.Add((int64) 50);
    leaf3.Add((int64) 60);

    DBusValueArray mid2;
    mid2.Add(leaf3);

    DBusValueArray cube;
    cube.Add(mid1);
    cube.Add(mid2);

    DBusMessage msg_cube = DBusMessage::CreateSignal(311, "/matrix", "com.test", "Cube", { DBusValue(cube) });
    DBusMessage p_cube(msg_cube.GetRawData());

    TEST_CHECK_EQ(p_cube.ParseFields().signature, "aaax");

    DBusValueArray b_cube = p_cube.ParseBody();
    DBusValueArray pcube = b_cube[0];
    TEST_CHECK_EQ(pcube.GetCount(), 2);

    DBusValueArray pmid1 = pcube[0];
    TEST_CHECK_EQ(pmid1.GetCount(), 2);
    DBusValueArray pleaf1 = pmid1[0];
    TEST_CHECK_EQ(pleaf1.GetCount(), 2);
    TEST_CHECK_EQ(pleaf1[0].To<int64>(), (int64)10);
    TEST_CHECK_EQ(pleaf1[1].To<int64>(), (int64)20);
    DBusValueArray pleaf2 = pmid1[1];
    TEST_CHECK_EQ(pleaf2.GetCount(), 1);
    TEST_CHECK_EQ(pleaf2[0].To<int64>(), (int64)30);

    DBusValueArray pmid2 = pcube[1];
    TEST_CHECK_EQ(pmid2.GetCount(), 1);
    DBusValueArray pleaf3 = pmid2[0];
    TEST_CHECK_EQ(pleaf3.GetCount(), 3);
    TEST_CHECK_EQ(pleaf3[0].To<int64>(), (int64)40);
    TEST_CHECK_EQ(pleaf3[1].To<int64>(), (int64)50);
    TEST_CHECK_EQ(pleaf3[2].To<int64>(), (int64)60);
}

void TestDeepStructNesting()
{
    RLOG("Running Deep Struct Nesting Tests...");

    // Three structs deep, deliberately mixing field sizes at each level: a
    // struct always starts on an 8-byte boundary regardless of what came right
    // before it, so a byte immediately followed by a nested struct is exactly
    // the kind of spot a missed re-alignment would show up.
    DBusValueStruct inner;
    inner.Add((int32) 7);
    inner.Add("core");

    DBusValueStruct middle;
    middle.Add((byte) 1);
    middle.Add(inner);

    DBusValueStruct outer;
    outer.Add((int64) 99);
    outer.Add(middle);

    DBusMessage msg = DBusMessage::CreateMethodCall(320, "com.test", "/nest", "com.test.Deep", "Probe", { DBusValue(outer) });
    DBusMessage parsed(msg.GetRawData());

    TEST_CHECK_EQ(parsed.ParseFields().signature, "(x(y(is)))");

    DBusValueArray body = parsed.ParseBody();
    TEST_CHECK_EQ(body.GetCount(), 1);
    TEST_CHECK(body[0].Is<DBusValueStruct>());

    DBusValueStruct pouter = body[0];
    TEST_CHECK_EQ(pouter.GetCount(), 2);
    TEST_CHECK_EQ(pouter[0].To<int64>(), (int64)99);
    TEST_CHECK(pouter[1].Is<DBusValueStruct>());

    DBusValueStruct pmiddle = pouter[1];
    TEST_CHECK_EQ(pmiddle.GetCount(), 2);
    TEST_CHECK_EQ(pmiddle[0].To<byte>(), (byte)1);
    TEST_CHECK(pmiddle[1].Is<DBusValueStruct>());

    DBusValueStruct pinner = pmiddle[1];
    TEST_CHECK_EQ(pinner.GetCount(), 2);
    TEST_CHECK_EQ(pinner[0].To<int32>(), (int32)7);
    TEST_CHECK_EQ(pinner[1].To<String>(), "core");
}

void TestArrayOfStructsAlignment()
{
    RLOG("Running Array-of-Structs Alignment Tests...");

    // a(yxy): each struct is byte + int64 + byte, a 17-byte payload that does NOT
    // land on an 8-byte boundary on its own - the marshaller has to re-pad to
    // 8 before every element, not just before the first one, or element 2 and
    // 3 come out shifted.
    DBusValueStruct rec1;
    rec1.Add((byte) 11);
    rec1.Add((int64) 111111);
    rec1.Add((byte) 21);

    DBusValueStruct rec2;
    rec2.Add((byte) 12);
    rec2.Add((int64) 222222);
    rec2.Add((byte) 22);

    DBusValueStruct rec3;
    rec3.Add((byte) 13);
    rec3.Add((int64) 333333);
    rec3.Add((byte) 23);

    DBusValueArray recs;
    recs.Add(rec1);
    recs.Add(rec2);
    recs.Add(rec3);

    DBusMessage msg = DBusMessage::CreateSignal(321, "/recs", "com.test", "Batch", { DBusValue(recs) });
    DBusMessage parsed(msg.GetRawData());

    TEST_CHECK_EQ(parsed.ParseFields().signature, "a(yxy)");

    DBusValueArray body = parsed.ParseBody();
    TEST_CHECK(body[0].Is<DBusValueArray>());

    DBusValueArray precs = body[0];
    TEST_CHECK_EQ(precs.GetCount(), 3);

    DBusValueStruct prec1 = precs[0];
    TEST_CHECK_EQ(prec1[0].To<byte>(), (byte)11);
    TEST_CHECK_EQ(prec1[1].To<int64>(), (int64)111111);
    TEST_CHECK_EQ(prec1[2].To<byte>(), (byte)21);

    DBusValueStruct prec2 = precs[1];
    TEST_CHECK_EQ(prec2[0].To<byte>(), (byte)12);
    TEST_CHECK_EQ(prec2[1].To<int64>(), (int64)222222);
    TEST_CHECK_EQ(prec2[2].To<byte>(), (byte)22);

    DBusValueStruct prec3 = precs[2];
    TEST_CHECK_EQ(prec3[0].To<byte>(), (byte)13);
    TEST_CHECK_EQ(prec3[1].To<int64>(), (int64)333333);
    TEST_CHECK_EQ(prec3[2].To<byte>(), (byte)23);
}

void TestStructWithArrayAndMap()
{
    RLOG("Running Struct-Containing-Array-and-Map Tests...");

    // (saua{sv}): a struct field can itself be an array or a dict. This is a very
    // common real-world shape (a named record plus an id list plus a properties bag).
    DBusValueArray ids;
    ids.Add((uint32) 100);
    ids.Add((uint32) 200);
    ids.Add((uint32) 300);

    DBusValueMap meta;
    meta.Add("Active", DBusValue(true));
    meta.Add("Owner", DBusValue("root"));

    DBusValueStruct rec;
    rec.Add("Session");
    rec.Add(ids);
    rec.Add(meta);

    DBusMessage msg = DBusMessage::CreateSignal(322, "/rec", "com.test", "Record", { DBusValue(rec) });
    DBusMessage parsed(msg.GetRawData());

    TEST_CHECK_EQ(parsed.ParseFields().signature, "(saua{sv})");

    DBusValueArray body = parsed.ParseBody();
    TEST_CHECK(body[0].Is<DBusValueStruct>());

    DBusValueStruct prec = body[0];
    TEST_CHECK_EQ(prec.GetCount(), 3);
    TEST_CHECK_EQ(prec[0].To<String>(), "Session");

    TEST_CHECK(prec[1].Is<DBusValueArray>());
    DBusValueArray pids = prec[1];
    TEST_CHECK_EQ(pids.GetCount(), 3);
    TEST_CHECK_EQ(pids[0].To<uint32>(), (uint32)100);
    TEST_CHECK_EQ(pids[1].To<uint32>(), (uint32)200);
    TEST_CHECK_EQ(pids[2].To<uint32>(), (uint32)300);

    TEST_CHECK(prec[2].Is<DBusValueMap>());
    DBusValueMap pmeta = prec[2];
    TEST_CHECK_EQ(pmeta.GetCount(), 2);
    TEST_CHECK_EQ(pmeta.Get("Active").To<bool>(), true);
    TEST_CHECK_EQ(pmeta.Get("Owner").To<String>(), "root");

    // Same shape again, but with the array member left empty. An empty
    // DBusValueArray has no element to infer a type from, so we don't assert
    // an exact signature here (same reasoning as the empty-array case in
    // TestCornerCases). What actually matters is that a zero-length member
    // sitting in the *middle* of a struct doesn't eat bytes from, or misalign,
    // the map field that comes right after it.
    DBusValueArray no_ids;

    DBusValueStruct rec_empty;
    rec_empty.Add("Ghost");
    rec_empty.Add(no_ids);
    rec_empty.Add(meta);

    DBusMessage msg2 = DBusMessage::CreateSignal(323, "/rec", "com.test", "Record", { DBusValue(rec_empty) });
    DBusMessage parsed2(msg2.GetRawData());

    DBusValueArray body2 = parsed2.ParseBody();
    DBusValueStruct prec2 = body2[0];
    TEST_CHECK_EQ(prec2[0].To<String>(), "Ghost");

    DBusValueArray pids2 = prec2[1];
    TEST_CHECK_EQ(pids2.GetCount(), 0);

    DBusValueMap pmeta2 = prec2[2];
    TEST_CHECK_EQ(pmeta2.GetCount(), 2);
    TEST_CHECK_EQ(pmeta2.Get("Active").To<bool>(), true);
    TEST_CHECK_EQ(pmeta2.Get("Owner").To<String>(), "root");
}

void TestDeepVariantMapNesting()
{
    RLOG("Running Deep Variant/Map Nesting Tests (ObjectManager-style)...");

    // a{sv} of a{sv} of a{sv} This is the classic BlueZ/UDisks2/NetworkManager shape
    // (GetManagedObjects returns essentially this, one level deeper still with
    // object-path keys). Every level here is declared "a{sv}" on the wire;
    // the nested dict rides inside the outer dict's variant payload, which is
    // exactly the path that needs a full nested signature string rather than
    // a single type char.
    DBusValueMap props;
    props.Add("Powered", DBusValue(true));
    props.Add("Alias", DBusValue("MyDevice"));
    props.Add("Class", DBusValue((uint32) 0x240404));

    DBusValueMap ifaces;
    ifaces.Add("org.bluez.Device1", DBusValue(props));

    DBusValueMap objects;
    objects.Add("/org/bluez/hci0/dev_AA_BB_CC", DBusValue(ifaces));

    DBusMessage msg = DBusMessage::CreateMethodReturn(330, 100, "com.test", { DBusValue(objects) });
    DBusMessage parsed(msg.GetRawData());

    TEST_CHECK_EQ(parsed.ParseFields().signature, "a{sv}");

    DBusValueArray body = parsed.ParseBody();
    TEST_CHECK_EQ(body.GetCount(), 1);
    TEST_CHECK(body[0].Is<DBusValueMap>());

    DBusValueMap pobjects = body[0];
    TEST_CHECK_EQ(pobjects.GetCount(), 1);

    DBusValue piface_v = pobjects.Get("/org/bluez/hci0/dev_AA_BB_CC");
    TEST_CHECK(piface_v.Is<DBusValueMap>());

    DBusValueMap pifaces = piface_v;
    TEST_CHECK_EQ(pifaces.GetCount(), 1);

    DBusValue pprops_v = pifaces.Get("org.bluez.Device1");
    TEST_CHECK(pprops_v.Is<DBusValueMap>());

    DBusValueMap pprops = pprops_v;
    TEST_CHECK_EQ(pprops.GetCount(), 3);
    TEST_CHECK_EQ(pprops.Get("Powered").To<bool>(), true);
    TEST_CHECK_EQ(pprops.Get("Alias").To<String>(), "MyDevice");
    TEST_CHECK_EQ(pprops.Get("Class").To<uint32>(), (uint32)0x240404);
}

void TestMixedTopLevelAlignment()
{
    RLOG("Running Mixed Top-Level Argument Alignment Tests...");

    // A handful of differently-sized top-level args back to back, so that
    // getting one right in isolation but wrong once something odd-sized
    // precedes it (the classic bug) would actually show up: byte, then a
    // struct, then an int16 array, then a double, then another struct.
    DBusValueStruct s1;
    s1.Add((byte) 5);
    s1.Add((int64) 987654321);

    DBusValueArray a1;
    a1.Add((int16) -7);
    a1.Add((int16) 7);
    a1.Add((int16) 1000);

    DBusValueStruct s2;
    s2.Add("tail");
    s2.Add((byte) 9);

    DBusValueArray args = {
        DBusValue((byte) 1),
        DBusValue(s1),
        DBusValue(a1),
        DBusValue((double) 2.71828),
        DBusValue(s2)
    };

    DBusMessage msg = DBusMessage::CreateMethodCall(331, "com.test", "/mix", "com.test.Mix", "Go", args);
    DBusMessage parsed(msg.GetRawData());

    TEST_CHECK_EQ(parsed.ParseFields().signature, "y(yx)and(sy)");

    DBusValueArray body = parsed.ParseBody();
    TEST_CHECK_EQ(body.GetCount(), 5);

    TEST_CHECK_EQ(body[0].To<byte>(), (byte)1);

    TEST_CHECK(body[1].Is<DBusValueStruct>());
    DBusValueStruct ps1 = body[1];
    TEST_CHECK_EQ(ps1[0].To<byte>(), (byte)5);
    TEST_CHECK_EQ(ps1[1].To<int64>(), (int64)987654321);

    TEST_CHECK(body[2].Is<DBusValueArray>());
    DBusValueArray pa1 = body[2];
    TEST_CHECK_EQ(pa1.GetCount(), 3);
    TEST_CHECK_EQ(pa1[0].To<int16>(), (int16)-7);
    TEST_CHECK_EQ(pa1[1].To<int16>(), (int16)7);
    TEST_CHECK_EQ(pa1[2].To<int16>(), (int16)1000);

    TEST_CHECK_EQ(body[3].To<double>(), (double)2.71828);

    TEST_CHECK(body[4].Is<DBusValueStruct>());
    DBusValueStruct ps2 = body[4];
    TEST_CHECK_EQ(ps2[0].To<String>(), "tail");
    TEST_CHECK_EQ(ps2[1].To<byte>(), (byte)9);
}

void TestRoundtripReMarshal()
{
    RLOG("Running Re-Marshal Idempotency Tests...");

    // Build a nested value (map -> variant -> array -> struct), marshal it,
    // parse it back, then marshal the *parsed* value again as-is. If anything
    // came back mistyped from ParseType() (e.g. a struct silently turning
    // into a plain array), this second pass produces a different signature
    // or a different byte stream than the first pass did - a value-equality
    // check alone would never catch that, only a second marshal does.
    DBusValueStruct inner;
    inner.Add((uint32) 4096);
    inner.Add("payload");

    DBusValueArray items;
    items.Add(inner);

    DBusValueMap wrapper;
    wrapper.Add("Items", DBusValue(items));

    DBusMessage msg1 = DBusMessage::CreateSignal(340, "/rt", "com.test", "First", { DBusValue(wrapper) });
    DBusMessage p1(msg1.GetRawData());

    DBusValueArray body1 = p1.ParseBody();
    TEST_CHECK_EQ(body1.GetCount(), 1);

    // Feed the parsed value straight back in as the args for a second message.
    DBusMessage msg2 = DBusMessage::CreateSignal(341, "/rt", "com.test", "Second", body1);
    DBusMessage p2(msg2.GetRawData());

    TEST_CHECK_EQ(p2.ParseFields().signature, p1.ParseFields().signature);
    TEST_CHECK_EQ(p2.ParseFields().signature, "a{sv}");

    DBusValueArray body2 = p2.ParseBody();
    TEST_CHECK_EQ(body2.GetCount(), 1);
    TEST_CHECK(body2[0].Is<DBusValueMap>());

    DBusValueMap pwrapper = body2[0];
    DBusValue pitems_v = pwrapper.Get("Items");
    TEST_CHECK(pitems_v.Is<DBusValueArray>());

    DBusValueArray pitems = pitems_v;
    TEST_CHECK_EQ(pitems.GetCount(), 1);
    TEST_CHECK(pitems[0].Is<DBusValueStruct>());

    DBusValueStruct pinner = pitems[0];
    TEST_CHECK_EQ(pinner.GetCount(), 2);
    TEST_CHECK_EQ(pinner[0].To<uint32>(), (uint32)4096);
    TEST_CHECK_EQ(pinner[1].To<String>(), "payload");

    // The two messages differ in serial and member name (which only affects
    // the header fields section), but carry the same logical payload, so
    // the body bytes, sliced out the same way ParseBody() finds them, should
    // be byte-for-byte identical.
    const String& raw1 = msg1.GetRawData();
    const String& raw2 = msg2.GetRawData();

    int bodypos1 = 16 + msg1.GetHeader().fieldslen;
    while(bodypos1 % 8 != 0)
        bodypos1++;

    int bodypos2 = 16 + msg2.GetHeader().fieldslen;
    while(bodypos2 % 8 != 0)
        bodypos2++;

    String bodybytes1 = raw1.Mid(bodypos1);
    String bodybytes2 = raw2.Mid(bodypos2);

    TEST_CHECK_EQ(bodybytes1, bodybytes2);
}

void TestSignatureLengthGuard()
{
    RLOG("Running Signature Length Guard Tests...");

    // DBusMessage's Null-object pattern, directly: a Null-constructed
    // message must report null, and must not be confused with a
    // legitimately empty (but valid) message.
    DBusMessage nullmsg = Null;
    TEST_CHECK(IsNull(nullmsg));

    DBusMessage emptyargs = DBusMessage::CreateMethodCall(500, "d", "/p", "i", "m");
    TEST_CHECK(!IsNull(emptyargs));

    // Exactly 255 'u' signature characters (right at the D-Bus spec's
    // signature length cap) must still succeed and round-trip cleanly.
    DBusValueArray exactargs;
    for(int i = 0; i < 255; i++)
        exactargs.Add((uint32) i);

    DBusMessage justfits = DBusMessage::CreateMethodCall(501, "com.test", "/big", "com.test.Big", "JustFits", exactargs);
    TEST_CHECK(!IsNull(justfits));

    DBusMessage p_fits(justfits.GetRawData());
    TEST_CHECK_EQ(p_fits.ParseFields().signature.GetLength(), 255);
    TEST_CHECK_EQ(p_fits.ParseBody().GetCount(), 255);

    // One 'u' over the limit: Create() must refuse rather than silently
    // truncating the length byte and desyncing the wire format.
    DBusValueArray toomanyargs;
    for(int i = 0; i < 256; i++)
        toomanyargs.Add((uint32) i);

    DBusMessage toolong = DBusMessage::CreateMethodCall(502, "com.test", "/big", "com.test.Big", "TooMany", toomanyargs);
    TEST_CHECK(IsNull(toolong));
}

void TestRecursionDepthGuard()
{
    RLOG("Running Recursion Depth Guard Tests...");

	// Allowed max: 32
    DBusValue deep = (int32) 42;
    for(int i = 0; i < 40; i++) {
        DBusValueStruct s;
        s.Add(deep);
        deep = s;
    }

    DBusMessage msg = DBusMessage::CreateSignal(503, "/deep", "com.test", "Nested", { deep });
    TEST_CHECK(!IsNull(msg));

    DBusMessage parsed(msg.GetRawData());

    TEST_CHECK(parsed.ParseBody().GetCount() == 0);
}

CONSOLE_APP_MAIN
{
    StdLogSetup(LOG_FILE | LOG_COUT);

    RLOG("=========================================");
    RLOG("         DBus Auto-Test Suite            ");
    RLOG("=========================================");

    TestDBusValueTypes();
    TestMessagePrimitives();
    TestComplexContainers();
    TestCornerCases();
    TestStructsAndExtremeNesting();
    TestArrayOfArraysNesting();
    TestDeepStructNesting();
    TestArrayOfStructsAlignment();
    TestStructWithArrayAndMap();
    TestDeepVariantMapNesting();
    TestMixedTopLevelAlignment();
    TestRoundtripReMarshal();
    TestSignatureLengthGuard();
    TestRecursionDepthGuard();

    RLOG("=========================================");
    RLOG("Tests Passed: " << passed_tests);
    RLOG("Tests Failed: " << failed_tests);
    RLOG("=========================================");

    if(failed_tests > 0)
        SetExitCode(1);
}
