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

    // 1. Simple Struct: (is)
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

    // 2. The FileChooser Nightmare: a(sa(us))
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

    // 3. Variant containing a struct inside a map: a{sv} where v is (db)
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

    RLOG("=========================================");
    RLOG("Tests Passed: " << passed_tests);
    RLOG("Tests Failed: " << failed_tests);
    RLOG("=========================================");

    if(failed_tests > 0)
        SetExitCode(1);
}