#ifndef _DBusLib_Inspector_h_
#define _DBusLib_Inspector_h_

#include <DBus/DBus.h>

namespace Upp {

class DBusInspector {
public:
    DBusInspector(DBusConnection& dbus);
    virtual ~DBusInspector()                     {}
    
    bool            Enumerate();
    bool            EnumerateActivatable();
    Vector<String>  GetServiceNames() const      { return dbus.GetMessage().ParseStringArray(); }

    bool            Introspect(const String& service, const String& path);
    String          GetResult() const            { return dbus.GetMessage().ParseString(); }
    
    bool            Ping(const String& service, const String& path = "/");
    bool            IsAlive() const              { return dbus.GetMessage().IsOK(); }
    
    bool            FetchMachineId(const String& service, const String& path = "/");
    String          GetMachineId() const         { return dbus.GetMessage().ParseString(); }

private:
    DBusConnection& dbus;
};

}
#endif
