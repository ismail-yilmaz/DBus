#include <DBus/DBus.h>

using namespace Upp;

bool FetchEnvironment(DBusConnection& dbus)
{
	return dbus.FetchProperty(
				"org.freedesktop.systemd1",				// Destination
				"/org/freedesktop/systemd1",			// Path
				"org.freedesktop.systemd1.Manager",		// Interface
				"Environment");							// Property
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT | LOG_FILE);

	DBusConnection dbus;
	if(dbus.ConnectSession()) {
		RLOG("Querying systemd for its environment array...");
		if(FetchEnvironment(dbus)) {
			if(const DBusMessage& msg = dbus.GetMessage(); !msg.IsError()) {
				DBusValueArray v = msg.ParseBody();
				if(v.GetCount() > 0 && v[0].Is<DBusValueArray>()) {
					const auto& q =v[0].To<DBusValueArray>();
					RLOG("Environment variable count: " << q.GetCount());
					for(const String& e : q)
						RLOG(e);
				}
			}
			else
				RLOG(Format("Daemon rejected the request. %s (%s)", msg.GetErrorName(), msg.GetErrorDesc()));
		}
		return;
	}
	RLOG("D-Bus operation failed: " << dbus.GetErrorDesc());

}