#include <DBus/DBus.h>

// After running this server, open up a terminal and execute the following command:
// dbus-send --print-reply --session --dest=com.bobcat.Terminal /com/bobcat/Terminal com.bobcat.Terminal.Control.OpenTab string:"/var/log"

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT | LOG_FILE);
	DBusConnection dbus;

	RLOG("Bobcat D-Bus Daemon...");

	if(!dbus.ConnectSession())
		return;

	// Claim our well-known name on the bus
	if(!dbus.RequestName("com.bobcat.Terminal"))
		return;

	RLOG("Service com.bobcat.Terminal registered and listening!");

	// Handle incoming remote method calls
	dbus.WhenMethodCall = [&](const DBusMessage& msg) {
		DBusMessage::FieldData fd = msg.ParseFields();

		if(fd.interface == "com.bobcat.Terminal.Control" && fd.member == "OpenTab") {
			DBusValueArray args = msg.ParseBody();
			String path = args.GetCount() > 0 ? args[0] : "~/";

			RLOG("Incoming request: Opening new tab at path: " << path);

			// Execute the actual U++ application logic here...
			int new_tab_id = 42;

			// Send the result back to the caller
			dbus.SendReply(msg, { new_tab_id, "Tab successfully created" });
		}
		else {
			RLOG("Unhandled method call: " << fd.member);
			// In a complete implementation, we'd send an Error (Type 3) reply here
		}
	};

	// Keep the daemon alive and processing events
	dbus.Timeout(10000).Listen();
}