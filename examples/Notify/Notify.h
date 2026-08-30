#ifndef _DBusShellNotification_h_
#define _DBusShellNotification_h_

#include <DBus/DBus.h>

namespace Upp {

class DBusNotification {
private:
    String    app_name;
    dword     replaces_id;
    String    icon;
    String    summary;
    String    body;
    int32     timeout;
    DBusValueMap hints;
    DBusValueArray actions;

public:
    DBusNotification& AppName(const String& s) { app_name = s; return *this; }
    DBusNotification& Replaces(dword id)       { replaces_id = id; return *this; }
    DBusNotification& Icon(const String& s)    { icon = s; return *this; }
    DBusNotification& Summary(const String& s) { summary = s; return *this; }
    DBusNotification& Body(const String& s)    { body = s; return *this; }
    DBusNotification& Timeout(int32 ms)        { timeout = ms; return *this; }

    // D-Bus Dictionary (a{sv}) Hint Builders
    DBusNotification& Hint(const String& key, const DBusValue& v) { hints(key, v); return *this; }
    DBusNotification& Urgency(byte level)      { return Hint("urgency", level); } // 0=low, 1=normal, 2=critical
    DBusNotification& Category(const String& s){ return Hint("category", s); }
    DBusNotification& Resident(bool b)         { return Hint("resident", b); }

    // D-Bus Array (as) Action Builder (Requires pairs of strings per FreeDesktop spec)
    DBusNotification& Action(const String& id, const String& label) {
        actions << id << label;
        return *this;
    }

    // Constructor with sensible defaults
    DBusNotification() : app_name("U++ DBus"), replaces_id(0), timeout(-1) {}

    // Transmits the payload and returns the Notification ID (or 0 on failure)
    dword Send(DBusConnection& dbus) {
        bool ok = dbus.MethodCall(
            "org.freedesktop.Notifications",
            "/org/freedesktop/Notifications",
            "org.freedesktop.Notifications",
            "Notify",
            {
                app_name,
                replaces_id,
                icon,
                summary,
                body,
                actions,
                hints,
                timeout
            }
        );

        const DBusMessage& rep = dbus.GetMessage();
        if(ok && rep.IsOK()) {
            DBusValueArray out = rep.ParseBody();
            if(out.GetCount() > 0 && out[0].Is<dword>()) {
                return out[0].To<dword>();
            }
        }
        return 0; // Return 0 to indicate failure
    }
};

}


#endif
