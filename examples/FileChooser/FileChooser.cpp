#include "FileChooser.h"

using namespace Upp;

constexpr const char *PORTAL_NAME  = "org.freedesktop.portal.Desktop";
constexpr const char *PORTAL_PATH  = "/org/freedesktop/portal/desktop";
constexpr const char *PORTAL_IFACE = "org.freedesktop.portal.FileChooser";

static DBusValueStruct MakeExt(const String& pattern)
{
	return { (uint32) 0, pattern };
}

static DBusValueArray PathToBytes(const String& path)
{
	DBusValueArray bytes;
	for(byte b : path)
		bytes.Add(b);
	bytes.Add(0);
	return bytes;
}

static DBusValueArray BuildFilters(const VectorMap<String, String>& types)
{
	DBusValueArray filters;
	for(int i = 0; i < types.GetCount(); i++) {
		DBusValueArray patterns;
		for(const String& raw : Split(types[i], ' ')) {
			if(String pattern = TrimBoth(raw); !pattern.IsEmpty())
				patterns.Add(MakeExt(pattern));
		}
		DBusValueStruct filter;
		filter.Add(types.GetKey(i));
		filter.Add(patterns);
		filters.Add(filter);
	}
	return filters;
}

bool DBusFileSel::DoExecute(const char *method, const String& title, bool asdir)
{
	selected.Clear();

	DBusConnection dbus;
	if(!dbus.ConnectSession()) {
		RLOG("Failed to connect to session bus: " << dbus.GetErrorDesc());
		return false;
	}

	DBusValueMap options;
	if(multisel)
		options.Add("multiple", true);
	if(asdir)
		options.Add("directory", true);
	if(!activedir.IsEmpty())
		options.Add("current_folder", PathToBytes(activedir));
	if(!activefile.IsEmpty())
		options.Add("current_name", activefile);
	if(!asdir && types.GetCount() > 0)
		options.Add("filters", BuildFilters(types));

	if(!dbus.MethodCall(PORTAL_NAME, PORTAL_PATH, PORTAL_IFACE, method, { "", title, options })) {
		RLOG(t_("Failed to request FileChooser: ") << dbus.GetErrorDesc());
		return false;
	}
	if(dbus.GetMessage().IsError()) {
		RLOG(t_("FileChooser portal rejected the request: ") << dbus.GetMessage().GetErrorDesc());
		return false;
	}
	DBusValueArray reply = dbus.GetMessage().ParseBody();
	if(reply.GetCount() == 0) {
		RLOG(t_("FileChooser portal returned an empty reply."));
		return false;
	}
	
	String reqpath = reply[0];
	String match = Format("type='signal',interface='org.freedesktop.portal.Request',path='%s'", reqpath);

	dbus.AddMatch(match, [&](const DBusMessage& msg) {
		if(msg.ParseFields().member == "Response") {
			DBusValueArray args = msg.ParseBody();
			if(args.GetCount() > 1 && (uint32) args[0] == 0) {
				DBusValueMap res = args[1];
				for(int i = 0; i < res.GetCount(); i++) {
					if(res.GetKey(i) != "uris")
						continue;
					DBusValueArray uris = res[i];
					for(const String& uri : uris) {
						UrlInfo u(uri);
						selected.Add(u.scheme == "file" ? u.path : uri);
					}
					break;
				}
				dbus.Finish();
				return;
			}
		}
		dbus.Abort();
	});

	dbus.Timeout(Null).Listen();
	return dbus.IsSuccess();
}

CONSOLE_APP_MAIN
{
	StdLogSetup(LOG_FILE | LOG_COUT);
	
	SetLanguage(GetSystemLNG());
	
	DBusFileSel fs;
	fs.Type(t_("Images"), "*.png *.jpg *.jpeg")
	  .Type(t_("Text"),   "*.txt *.odt *.doc")
	  .AllFileTypes()
	  .Multi()
	  .ActiveDir(GetHomeDirectory());

	if(fs.ExecuteOpen(t_("Select File"))) {
		for(const String& s : fs.GetFiles())
			RLOG(t_("Selected: ") << s);
	}
	else
		RLOG(t_("Selection cancelled or failed."));
}

