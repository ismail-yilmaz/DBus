#include <DBus/DBus.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT | LOG_FILE);

	DBusConnection dbus;
	if(!dbus.ConnectSession()) return;

	DBusValueMap map {
		{ "WindowSystem", "Wayland" },
		{ "Width",  1920 },
		{ "Scaling", 1.25 },
		{ "Accelarated", true },
	};
	
	DBusValueArray arr {
		"EGL",
		"OpenGL",
		"Vulkan"
	};

	RLOG("Sending complex U++ payloads over the wire...");

	dbus.BusMethodCall("NonExistentMethod", { map, arr });   // Should return error

	if(const DBusMessage& msg = dbus.GetMessage(); msg.IsError()) {
		RLOG("Daemon safely parsed the payload boundaries and replied:");
		RLOG(msg.GetErrorName());
	}
}