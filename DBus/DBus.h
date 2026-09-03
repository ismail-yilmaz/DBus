#ifndef _Upp_DBus_h
#define _Upp_DBus_h

#include <Core/Core.h>

namespace Upp {

// Forward declarations
struct DBusValue;

struct DBusValueMap : public VectorMap<String, DBusValue> {
    using VectorMap<String, DBusValue>::VectorMap;
    DBusValueMap(const DBusValueMap& v) : VectorMap<String, DBusValue>(v, 1) {}
};

struct DBusValueArray : public Vector<DBusValue> {
    using Vector<DBusValue>::Vector;
    DBusValueArray(const DBusValueArray& v) : Vector<DBusValue>(v, 1) {}
};

struct DBusValueStruct : public Vector<DBusValue> {
    using Vector<DBusValue>::Vector;
    DBusValueStruct(const DBusValueStruct& v) : Vector<DBusValue>(v, 1) {}
};

class DBusValue : public Moveable<DBusValue>  {
public:
    DBusValue()                                               {}
    DBusValue(byte v) : value(RawToValue(v))                  {}
    DBusValue(int16 v) : value(RawToValue(v))                 {}
    DBusValue(uint16 v) : value(RawToValue(v))                {}
    DBusValue(int32 v) : value(v)                             {}
    DBusValue(uint32 v) : value(RawToValue(v))                {}
    DBusValue(int64 v) : value(v)                             {}
    DBusValue(uint64 v) : value(RawToValue(v))                {}
    DBusValue(bool v) : value(v)                              {}
    DBusValue(double v) : value(v)                            {}
    DBusValue(const String& s) : value(s)                     {}
    DBusValue(const char* s) : value(String(s))               {}
    DBusValue(const Nuller& v) : value(v)                     {}
    DBusValue(const DBusValueMap& v) : value(RawToValue(v))   {}
    DBusValue(const DBusValueArray& v) : value(RawToValue(v)) {}
    DBusValue(const DBusValueStruct& v) : value(RawToValue(v)){}
    
    bool IsNullInstance() const                               { return value.IsNull(); }
    template<typename T>  bool     Is() const                 { return value.Is<T>(); }
    template<typename T>  const T& To() const                 { return value.To<T>(); }
    template<typename T>  operator T() const                  { return value.To<T>(); }

private:
	Value value;
};

struct DBusError : Exc {
    int code;
    DBusError() : Exc(Null), code(-1)                               {}
    DBusError(const String& reason) : Exc(reason), code(-1)         {}
    DBusError(int rc, const String& reason) : Exc(reason), code(rc) {}
};
    
#include "Parser.h"
#include "Message.h"
#include "Connection.h"

}
#endif