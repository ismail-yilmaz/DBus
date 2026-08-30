#include <DBus/DBus.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT | LOG_FILE);

	DBusConnection dbus;
	if(!dbus.ConnectSession()) {
		RLOG("Connection failed.");
		return;
	}

	if(dbus.AddMatch("type='signal'"))
		RLOG("Global signal subscription successful.");

	dbus.WhenSignal = [](const DBusMessage& msg) {
		DBusMessage::FieldData fd = msg.ParseFields();
		RLOG("[GLOBAL IN] " << fd.interface << "::" << fd.member);
	};

	// Begin async listening mode
	dbus.NonBlocking().Listen();
	RLOG("Listening for events. Open an application or plug in a USB...");

	SocketWaitEvent we;
	dbus.AddTo(we);

	while(dbus.Do())
		we.Wait(1000); // External blocking
}