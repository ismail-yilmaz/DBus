#include <DBusLib/DBusLib.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE | LOG_COUT);
	
	DBusConnection dbus;
	if(!dbus.ConnectSession()) {
		RLOG("D-Bus connection failed. " << dbus.GetErrorDesc());
		Exit(1);
	}
	
	const char *filetypes = "Images\t*.png *.jpg *.jpeg\nText\t*.txt *.doc *.odt";
	
	RLOG("Selected file(s): " << SelectOpenFiles(dbus, filetypes));
	RLOG("Selected Folder:  " << SelectDirectory(dbus));
//	RLOG("Selected file content: " << SelectLoadFile(dbus, filetypes));
}

