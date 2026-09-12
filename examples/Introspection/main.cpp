#include <DBusLib/DBusLib.h>

using namespace Upp;

void PrintList(const String& title, const Vector<String>& list, int limit = -1)
{
	RLOG(title);
	int count = limit < 0 ? list.GetCount() : min(list.GetCount(), limit);

	for(int i = 0; i < count; i++)
		RLOG(" - " << list[i]);

	if(list.GetCount() > count)
		RLOG(" - ... (" << list.GetCount() << " total)");
}

void DumpIntrospection(const String& xml)
{
	RLOG("Deep Interface Traversal:");
	try {
		XmlNode node = ParseXML(xml);
		const XmlNode& root = node["node"];

		for(int i = 0; i < root.GetCount(); i++) {
			const XmlNode& n = root[i];
			if(!n.IsTag("interface"))
				continue;
			RLOG("Interface: " << n.Attr("name"));
			for(int j = 0; j < n.GetCount(); j++) {
				const XmlNode& sub = n.Node(j);

				if(sub.IsTag("method"))
					RLOG(" - [Method]   " << sub.Attr("name"));
				else
				if(sub.IsTag("signal"))
					RLOG(" - [Signal]   " << sub.Attr("name"));
				else
				if(sub.IsTag("property"))
					RLOG(" - [Property] " << sub.Attr("name") << " (" << sub.Attr("type") << ", " << sub.Attr("access") << ")");
			}
		}
	}
	catch(const XmlError& e) {
		RLOG("XML Parsing Error: " << e);
	}
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE|LOG_COUT);

	String service = "org.freedesktop.Notifications";
	String path    = "/org/freedesktop/Notifications";

	try {
		DBusConnection dbus;
		if(!dbus.Timeout(5000).ConnectSession())
			throw DBusError("Failed to connect to D-Bus server.");

		DBusInspector inspector(dbus);

		if(inspector.Enumerate())
			PrintList("\nActive Services:", inspector.GetServiceNames(), 5);

		if(inspector.EnumerateActivatable())
			PrintList("\nActivatable Services:", inspector.GetServiceNames(), 5);

		RLOG("\nPeer Check");
		RLOG(" - Name: " << service);
		if(inspector.Ping(service))
			RLOG(" - Alive: " << (inspector.IsAlive() ? "Yes" : "No"));

		if(inspector.FetchMachineId(service))
			RLOG("\nMachine ID: " << inspector.GetMachineId());

		RLOG("\nIntrospecting: " << service);
		if(!inspector.Introspect(service, path))
			throw DBusError("Failed to introspect notification server.");

		String xml = inspector.GetResult();
		DumpIntrospection(xml);
	}
	catch(const DBusError& e) {
		RLOG("D-Bus Error: " << e);
	}
	catch(...) {
		RLOG("Unknown exception occured!");
	}
}