#ifndef _DBusLib_NetworkManager_h_
#define _DBusLib_NetworkManager_h_

#include <DBus/DBus.h>

namespace Upp {

class DBusNetworkManager {
public:
    enum class State : uint32 {
        Unknown         = 0,
        Asleep          = 10,
        Disconnected    = 20,
        Disconnecting   = 30,
        Connecting      = 40,
        ConnectedLocal  = 50,
        ConnectedSite   = 60,
        ConnectedGlobal = 70
    };

    enum class Connectivity : uint32 {
        Unknown = 0,
        None    = 1,
        Portal  = 2,
        Limited = 3,
        Full    = 4
    };
    
    DBusNetworkManager(DBusConnection& dbus);
    virtual ~DBusNetworkManager() {}

    bool             FetchVersion();
    String           GetVersion() const                 { return GetFirstArg<String>(dbus.GetMessage(), Null); }

    bool             FetchState();
    State            GetState() const                   { return static_cast<State>(GetFirstArg<uint32>(dbus.GetMessage(), 0)); }

    bool             FetchConnectivity();
    Connectivity     GetConnectivity() const            { return static_cast<Connectivity>(GetFirstArg<uint32>(dbus.GetMessage(), 0)); }

    bool             FetchDevices();
    Vector<String>   GetDevices() const                 { return dbus.GetMessage().ParseStringArray(); }

    bool             FetchActiveConnections();
    Vector<String>   GetActiveConnections() const;

    bool             FetchPrimaryConnection();
    String           GetPrimaryConnection() const       { return GetFirstArg<String>(dbus.GetMessage(), Null); }

    bool             FetchPrimaryConnectionType();
    String           GetPrimaryConnectionType() const   { return GetFirstArg<String>(dbus.GetMessage(), Null); }

    bool             FetchNetworkingStatus();
    bool             IsNetworkingEnabled() const        { return GetFirstArg<bool>(dbus.GetMessage(), false); }
    bool             EnableNetworking(bool b = true);
    bool             DisableNetworking()                { return EnableNetworking(false); }

    bool             FetchWirelessStatus();
    bool             IsWirelessEnabled() const          { return GetFirstArg<bool>(dbus.GetMessage(), false); }
    bool             EnableWireless(bool b = true);
    bool             DisableWireless()                  { return EnableWireless(false); }

    bool             FetchWwanStatus();
    bool             IsWwanEnabled() const              { return GetFirstArg<bool>(dbus.GetMessage(), false); }
    bool             EnableWwan(bool b = true);
    bool             DisableWwan()                      { return EnableWwan(false); }

    bool             Sleep(bool b = true);              // true puts NM to sleep, false wakes it
    bool             WakeUp()                           { return Sleep(false); }

private:
    DBusConnection&  dbus;
};

inline constexpr const char *StdDBusNetworkManagerName      = "org.freedesktop.NetworkManager";
inline constexpr const char *StdDBusNetworkManagerPath      = "/org/freedesktop/NetworkManager";
inline constexpr const char *StdDBusNetworkManagerInterface = "org.freedesktop.NetworkManager";

}

#endif