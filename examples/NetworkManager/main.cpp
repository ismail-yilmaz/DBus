#include <DBusLib/DBusLib.h>

using namespace Upp;

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_COUT | LOG_FILE);

	try {
		DBusConnection dbus;

		RLOG("Connecting to D-Bus system bus...");
		if(!dbus.Timeout(5000).ConnectSystem())
			throw DBusError("Failed to connect to D-Bus system bus.");

		DBusNetworkManager nm(dbus);

		if(nm.FetchVersion())
			RLOG("NetworkManager Version: " << nm.GetVersion() << "\n");

		if(nm.FetchState())
			RLOG("State:        " << (int) nm.GetState());

		if(nm.FetchConnectivity())
			RLOG("Connectivity: " << (int) nm.GetConnectivity());

		if(nm.FetchNetworkingStatus())
			RLOG("Networking:   " << (nm.IsNetworkingEnabled() ? "Enabled" : "Disabled"));

		if(nm.FetchWirelessStatus())
			RLOG("Wireless:     " << (nm.IsWirelessEnabled() ? "Enabled" : "Disabled"));

		if(nm.FetchWwanStatus())
			RLOG("WWAN:         " << (nm.IsWwanEnabled() ? "Enabled" : "Disabled"));

		if(nm.FetchPrimaryConnectionType())
			RLOG("\nPrimary Link: " << nm.GetPrimaryConnectionType());

		if(nm.FetchActiveConnections()) {
			Vector<String> active = nm.GetActiveConnections();
			RLOG("Active Connections (" << active.GetCount() << "):");
			for(const String& path : active)
				RLOG(" -> " << path);
		}

		if(nm.FetchDevices()) {
			Vector<String> devices = nm.GetDevices();
			RLOG("\nHardware Devices (" << devices.GetCount() << "):");
			for(const String& path : devices)
				RLOG(" -> " << path);
		}
		
		// Example: Toggling Wi-Fi (Requires active PolicyKit privileges)
		// if(nm.DisableWireless()) {
		//		Sleep(5000);
		//		nm.EnableWireless();
		// }
				

		RLOG("\nDone!");
	}
	catch(const DBusError& e) {
		RLOG(e);
	}
	catch(...) {
		RLOG("Unknown exception occurred!");
	}
}