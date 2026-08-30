#include "Notify.h"

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT | LOG_FILE);
	DBusConnection dbus;

	RLOG("Connecting to D-Bus Session...");
	if(!dbus.ConnectSession()) {
		RLOG("Connection failed.");
		return;
	}

	// Create and send our initial notification using the fluent interface
	DBusNotification note;
	dword note_id = note.AppName("Bobcat Terminal")
						.Icon("utilities-terminal")
						.Summary("Compiling U++ Project")
						.Body("Building DBus package... (50%)")
						.Urgency(1) // Normal
						.Timeout(0) // 0 means it shouldn't auto-close yet
						.Action("cancel", "Cancel Build")
						.Send(dbus);

	if(note_id > 0) {
		RLOG("Successfully launched notification. ID: " << note_id);

		// Simulate some work being done...
		Sleep(2000);
		RLOG("Updating existing notification bubble...");

		// Update the exact same notification bubble (instead of spawning a new one)
		// by passing the ID back into the Replaces() method.
		note.Summary("Compilation Complete")
			.Body("DBus package built successfully. Zero errors.")
			.Icon("dialog-information")
			.Replaces(note_id)
			.Timeout(3000) // Now it can auto-close after 3 seconds
			.Send(dbus);

		RLOG("Notification updated.");
	}
	else {
		RLOG("Failed to send notification: " << dbus.GetMessage().GetErrorDesc());
	}
}