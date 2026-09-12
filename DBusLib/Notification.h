#ifndef _DBusLib_Notification_h_
#define _DBusLib_Notification_h_

#include <DBus/DBus.h>

namespace Upp {

class DBusNotification {
public:
    DBusNotification(DBusConnection& session);

    DBusNotification& App(const String& s)                        { app = s; return *this; }
    DBusNotification& Replaces(uint32 id)                         { replaces = id; return *this; }
    DBusNotification& Icon(const String& s)                       { icon = s; return *this; }
    DBusNotification& Summary(const String& s)                    { summary = s; return *this; }
    DBusNotification& Body(const String& s)                       { body = s; return *this; }
    DBusNotification& Timeout(int32 ms)                           { timeout = ms; return *this; }

    DBusNotification& Action(const String& id, const String& txt);
    DBusNotification& Hint(const String& key, const DBusValue& v);
    
    DBusNotification& Urgency(byte level)                         { return Hint("urgency", level); }

    bool              Notify();
    uint32            GetNotificationId() const;

private:
	DBusConnection& dbus;
    String          app;
    uint32          replaces;
    int32           timeout;
    String          icon;
    String          summary;
    String          body;
    DBusValueArray  actions;
    DBusValueMap    hints;
};

inline constexpr const char *StdDBusNotificationName      = "org.freedesktop.Notifications";
inline constexpr const char *StdDBusNotificationPath      = "/org/freedesktop/Notifications";
inline constexpr const char *StdDBusNotificationInterface = "org.freedesktop.Notifications";
}

#endif