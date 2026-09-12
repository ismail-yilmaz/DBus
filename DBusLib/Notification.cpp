#include "Notification.h"

namespace Upp {

DBusNotification::DBusNotification(DBusConnection& session)
: dbus(session)
, replaces(0)
, timeout(-1)
{
}
	
DBusNotification& DBusNotification::Action(const String& id, const String& txt)
{
	actions.Add(id);
	actions.Add(txt);
	return *this;
}

DBusNotification& DBusNotification::Hint(const String& key, const DBusValue& v)
{
	hints.Add(key, v.Is<DBusValue>() ? v : AsVariant(v));
	return *this;
}

bool DBusNotification::Notify()
{
	return dbus.MethodCall(
		StdDBusNotificationName,
		StdDBusNotificationPath,
		StdDBusNotificationInterface,
		"Notify", {	app, replaces, icon, summary, body, actions, hints, timeout }
	);
}

uint32 DBusNotification::GetNotificationId() const
{
	return GetFirstArg<uint32>(dbus.GetMessage(), 0);
}


}