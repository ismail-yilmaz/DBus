#include <DBusLib/DBusLib.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);
	
	DBusConnection dbus;
	if(!dbus.ConnectSession()) {
		RLOG("Failed to connect to D-Bus session bus: " << dbus.GetErrorDesc());
		Exit(1);
	}

	// Take a screenshot in "interactive mode"
	RLOG("Screenshot path: " << TakeScreenshot(dbus));

}