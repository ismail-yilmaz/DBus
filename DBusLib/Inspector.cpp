#include "Inspector.h"

namespace Upp {

DBusInspector::DBusInspector(DBusConnection& dbc)
: dbus(dbc)
{
}

bool DBusInspector::Enumerate()
{
	return dbus.BusMethodCall("ListNames");
}

bool DBusInspector::EnumerateActivatable()
{
	return dbus.BusMethodCall("ListActivatableNames");
}

bool DBusInspector::Introspect(const String& service, const String& path)
{
	return dbus.MethodCall(service,	path, StdDBusIntrospectable, "Introspect");
}

bool DBusInspector::Ping(const String& service, const String& path)
{
	return dbus.MethodCall(service, path, StdDBusPeer, "Ping");
}

bool DBusInspector::FetchMachineId(const String& service, const String& path)
{
	return dbus.MethodCall(service, path, StdDBusPeer, "GetMachineId");
}

}