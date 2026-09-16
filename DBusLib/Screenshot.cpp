#include "Screenshot.h"

namespace Upp {

DBusScreenshot::DBusScreenshot(DBusConnection& session)
: dbus(session)
, interactive(true)
, modal(false)
{
}

bool DBusScreenshot::Execute(const String& parentwindow)
{
	ASSERT_(dbus.IsBlocking(), "DBusScreenshot can only be run in blocking mode!");

	if(dbus.InProgress())
		return false;
		
	uri.Clear();

	String token   = "Upp_DBus_" + Uuid::Create().ToString();
	String sender  = Filter(dbus.GetUniqueName(), [](int c) { return c == ':' ? 0 : c == '.' ? '_' : c; });
	String reqpath = "/org/freedesktop/portal/desktop/request/" + sender + "/" + token;

	String match;
	match << "type='signal',"
		  << "interface='org.freedesktop.portal.Request',"
		  << "path='" << reqpath << "'";

	dbus.AddMatch(match, [&](const DBusMessage& msg) {
		if(msg.ParseFields().member == "Response") {
			if(DBusValueArray args = msg.ParseBody(); args.GetCount() > 1) {
				if(uint32 rescode = args[0]; rescode == 0) {
					DBusValueMap res = args[1];
					if(int i = res.Find("uri"); i >= 0) {
						String s = res[i];
						UrlInfo u(s);
						uri = u.scheme == "file" ? u.path : s;
					}
					if(int i = res.Find("restore_token"); i >= 0) {
						restoretoken = res[i].To<String>();
					}
					dbus.Finish();
					return;
				}
			}
		}
		dbus.Abort();
	});

	DBusValueMap options;
	options.Add("handle_token", token);
	
	if(interactive)
		options.Add("interactive", interactive);

	if(modal)
		options.Add("modal", modal);

	if(restoretoken.GetCount())
		options.Add("restore_token", restoretoken);

	bool ok = false;
	int timeout = dbus.GetTimeout();
	
	ok = dbus.MethodCall(
		StdDBusScreenshotName,
		StdDBusScreenshotPath,
		StdDBusScreenshotInterface,
		"Screenshot", { parentwindow, options }
	);
	
	if(ok) {
		dbus.Timeout(Null).Listen();
		ok = dbus.IsSuccess();
	}

	dbus.Timeout(5000).RemoveMatch(match);
	dbus.Timeout(timeout);

	return ok;
}

String TakeScreenshot(DBusConnection& dbus)
{
	DBusScreenshot s(dbus);
	s.Interactive().Execute();
	return s.GetPath();
}

}