#include <DBusLib/DBusLib.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT | LOG_FILE);
	
	try {
		DBusConnection dbus;
		if(!dbus.Timeout(5000).ConnectSession())
			throw DBusError("Failed to connect to D-Bus session bus.");
		
		DBusNotification note(dbus);
		note.App("Bobcat Terminal")
			.Icon("utilities-terminal")
			.Summary("Compiling U++ Project")
			.Body("Building DBus package... (50%)")
			.Urgency(1) // Normal
			.Timeout(0) // 0 means it shouldn't auto-close yet
			.Action("cancel", "Cancel Build");
						
		if(!note.Notify())
			throw DBusError("Failed to send notification.");

		if(int id = note.GetNotificationId(); id) {
			RLOG("Notification is successfully sent. ID: " << id);
			Sleep(2000); // Simulate work

			note.Summary("Compilation Complete")
				.Body("DBus package built successfully. Zero errors.")
				.Icon("dialog-information")
				.Replaces(id)
				.Timeout(3000); // Now it can auto-close after 3 seconds

			if(!note.Notify())
				throw DBusError("Failed to update notification.");
				
			RLOG("Notification updated.");
		}
	}
	catch(const DBusError& e) {
		RLOG(e);
	}
	catch(...) {
		RLOG("Unknown exception occured!");
	}
}