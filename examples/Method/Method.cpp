#include <DBus/DBus.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT | LOG_FILE);

	DBusConnection dbus;

	RLOG("Connecting to D-Bus...");
	if(dbus.ConnectSession()) {
		RLOG("Requesting active bus names...");
		if(dbus.MethodCall("org.freedesktop.DBus", "/org/freedesktop/DBus", "org.freedesktop.DBus",	"ListNames")) {
			if(const DBusMessage& msg = dbus.GetMessage(); msg.IsOK()) {
				Vector<String> names = msg.ParseStringArray();
				RLOG("Active names: " << names.GetCount());
				for(const String& s : names)
					RLOG(s);
			}
			else
				RLOG("Method Call Error: " << msg.GetErrorName());
		}
		return;
	}
	RLOG("D-Bus operation failed: " << dbus.GetErrorDesc());
}