#include "NetworkManager.h"

namespace Upp {

DBusNetworkManager::DBusNetworkManager(DBusConnection& dbus)
: dbus(dbus)
{
}

bool DBusNetworkManager::FetchDevices()
{
	return dbus.MethodCall(
		StdDBusNetworkManagerName,
		StdDBusNetworkManagerPath,
		StdDBusNetworkManagerInterface,
		"GetDevices"
	);
}

bool DBusNetworkManager::FetchVersion()
{
	return dbus.MethodCall(
		StdDBusNetworkManagerName,
		StdDBusNetworkManagerPath,
		StdDBusProperties,
		"Get",	{ StdDBusNetworkManagerInterface, "Version" }
	);
}

bool DBusNetworkManager::FetchState()
{
	return dbus.MethodCall(
		StdDBusNetworkManagerName,
		StdDBusNetworkManagerPath,
		StdDBusProperties,
		"Get", { StdDBusNetworkManagerInterface, "State" }
	);
}

bool DBusNetworkManager::FetchConnectivity()
{
	return dbus.MethodCall(
		StdDBusNetworkManagerName,
		StdDBusNetworkManagerPath,
		StdDBusProperties,
		"Get", { StdDBusNetworkManagerInterface, "Connectivity" }
	);
}

bool DBusNetworkManager::FetchActiveConnections()
{
	return dbus.MethodCall(
		StdDBusNetworkManagerName,
		StdDBusNetworkManagerPath,
		StdDBusProperties,
		"Get", { StdDBusNetworkManagerInterface, "ActiveConnections" }
	);
}

Vector<String> DBusNetworkManager::GetActiveConnections() const
{
    Vector<String> paths;
    for(const DBusValue& q : GetFirstArg<DBusValueArray>(dbus.GetMessage(), DBusValueArray())) {
        if(q.Is<String>())
            paths.Add(q);
    }
    return paths;
}

bool DBusNetworkManager::FetchPrimaryConnection()
{
	return dbus.MethodCall(
		StdDBusNetworkManagerName,
		StdDBusNetworkManagerPath,
		StdDBusProperties,
		"Get", { StdDBusNetworkManagerInterface, "PrimaryConnection" }
	);
}

bool DBusNetworkManager::FetchPrimaryConnectionType()
{
	return dbus.MethodCall(
		StdDBusNetworkManagerName,
		StdDBusNetworkManagerPath,
		StdDBusProperties,
		"Get", { StdDBusNetworkManagerInterface, "PrimaryConnectionType" }
	);
}

bool DBusNetworkManager::FetchNetworkingStatus()
{
	return dbus.MethodCall(
		StdDBusNetworkManagerName,
		StdDBusNetworkManagerPath,
		StdDBusProperties,
		"Get", { StdDBusNetworkManagerInterface, "NetworkingEnabled" }
	);
}

bool DBusNetworkManager::FetchWirelessStatus()
{
	return dbus.MethodCall(
		StdDBusNetworkManagerName,
		StdDBusNetworkManagerPath,
		StdDBusProperties,
		"Get", { StdDBusNetworkManagerInterface, "WirelessEnabled" }
	);
}

bool DBusNetworkManager::FetchWwanStatus()
{
	return dbus.MethodCall(
		StdDBusNetworkManagerName,
		StdDBusNetworkManagerPath,
		StdDBusProperties,
		"Get", { StdDBusNetworkManagerInterface, "WwanEnabled" }
	);
}

bool DBusNetworkManager::EnableNetworking(bool b)
{
	return dbus.MethodCall(
		StdDBusNetworkManagerName,
		StdDBusNetworkManagerPath,
		StdDBusNetworkManagerInterface,
		"Enable", { b }
	);
}

bool DBusNetworkManager::EnableWireless(bool b)
{
	return dbus.MethodCall(
		StdDBusNetworkManagerName,
		StdDBusNetworkManagerPath,
		StdDBusProperties,
		"Set", { StdDBusNetworkManagerInterface, "WirelessEnabled", AsVariant(b) }
	);
}

bool DBusNetworkManager::EnableWwan(bool b)
{
	return dbus.MethodCall(
		StdDBusNetworkManagerName,
		StdDBusNetworkManagerPath,
		StdDBusProperties,
		"Set", { StdDBusNetworkManagerInterface, "WwanEnabled", AsVariant(b) }
	);
}

bool DBusNetworkManager::Sleep(bool sleep)
{
	return dbus.MethodCall(
		StdDBusNetworkManagerName,
		StdDBusNetworkManagerPath,
		StdDBusNetworkManagerInterface,
		"Sleep", { sleep }
	);
}

}