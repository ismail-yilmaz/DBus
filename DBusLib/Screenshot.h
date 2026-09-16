#ifndef _DBusLib_Screenshot_h_
#define _DBusLib_Screenshot_h_

#include <DBus/DBus.h>

namespace Upp {
    
class DBusScreenshot {
public:
    DBusScreenshot(DBusConnection& session);
    virtual ~DBusScreenshot()                                             {}
    
    DBusScreenshot&        Interactive(bool b = true)                     { interactive = b; return *this; }
    DBusScreenshot&        Modal(bool b = true)                           { modal = b; return *this; }
    DBusScreenshot&        RestoreToken(const String& token)              { restoretoken = token; return *this; }
    
    bool                   Execute(const String& parentwindow = Null);
    
    String                 GetPath() const                                { return uri; }
    String                 GetRestoreToken() const                        { return restoretoken; }
    
private:
    DBusConnection&        dbus;
    bool                   interactive:1;
    bool                   modal:1;
    String                 uri;
    String                 restoretoken;
};

String TakeScreenshot(DBusConnection& dbus);

inline constexpr const char *StdDBusScreenshotName      = "org.freedesktop.portal.Desktop";
inline constexpr const char *StdDBusScreenshotPath      = "/org/freedesktop/portal/desktop";
inline constexpr const char *StdDBusScreenshotInterface = "org.freedesktop.portal.Screenshot";

}

#endif