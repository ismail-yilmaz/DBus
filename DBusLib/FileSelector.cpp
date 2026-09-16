#include "FileSelector.h"

namespace Upp {

namespace {
	
enum SelectorMode : int {
	FSEL_OPEN,
	FSEL_SAVE,
	FSEL_OPENDIR
};

DBusValueStruct MakeExt(const String& pattern)
{
	return { (uint32) 0, pattern };
}

DBusValueArray PathToBytes(const String& path)
{
	DBusValueArray bytes;
	for(byte b : path)
		bytes.Add(b);
	bytes.Add(0);
	return bytes;
}

DBusValueArray BuildFilters(const VectorMap<String, String>& types)
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

void ParseTypes(DBusFileSelector& fs, const String& d)
{
	Vector<String> s = Split(d, '\n');
	for(int i = 0; i < s.GetCount(); i++) {
		Vector<String> h = Split(s[i], '\t');
		if(h.GetCount() == 2)
			fs.Type(h[0], h[1]);
		if(h.GetCount() == 1)
			fs.Type(h[0], h[0]);
	}
	fs.AllFileTypes();
}

}

DBusFileSelector::DBusFileSelector(DBusConnection& session)
: dbus(session)
{
}

bool DBusFileSelector::Run(const char *method, const String& title, bool asdir)
{
	ASSERT_(dbus.IsBlocking(), "DBusFileSelector can only be run in blocking mode!");

	if(dbus.InProgress())
		return false;
	
	selected.Clear();

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

	if(!dbus.MethodCall(
		StdDBusFileChooserName,
		StdDBusFileChooserPath,
		StdDBusFileChooserInterface,
		method, { "", title, options })
	)	return false;

	if(dbus.GetMessage().IsError())
		return false;

	DBusValueArray reply = dbus.GetMessage().ParseBody();
	if(reply.GetCount() == 0)
		return false;
	
	String reqpath = reply[0];

	String match;
	match << "type='signal',"
	      << "interface='org.freedesktop.portal.Request',"
	      << "path='" << reqpath << "'";

	if(!dbus.AddMatch(match, [&](const DBusMessage& msg) {
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
	})) return false;

	int timeout = dbus.GetTimeout();
	dbus.Timeout(Null).Listen();
	bool ok = dbus.IsSuccess();
	dbus.Timeout(5000).RemoveMatch(match);
	dbus.Timeout(timeout);
	return ok;
}

// TODO: Move to a global FileSel instance (just as we did in CtrlLib)

String SelectOpenFile(DBusConnection& dbus, const String& types)
{
	DBusFileSelector fs(dbus);
	ParseTypes(fs, types);
	fs.ActiveDir(GetHomeDirectory()).ExecuteOpen();
	return fs.Get();
}

Vector<String> SelectOpenFiles(DBusConnection& dbus, const String& types)
{
	DBusFileSelector fs(dbus);
	ParseTypes(fs, types);
	fs.ActiveDir(GetHomeDirectory()).Multi().ExecuteOpen(t_("Open File(s)"));
	return clone(fs.GetFiles());
}

String SelectLoadFile(DBusConnection& dbus, const String& types)
{
	String path = SelectOpenFile(dbus, types);
	return LoadFile(path);
}

bool SelectSaveFile(DBusConnection& dbus, const String& data, const String& types)
{
	DBusFileSelector fs(dbus);
	ParseTypes(fs, types);
	fs.ActiveDir(GetHomeDirectory()).ExecuteSaveAs();
	String path = fs.Get();
	return !IsNull(path) && SaveFile(path, data);
}

String SelectDirectory(DBusConnection& dbus)
{
	DBusFileSelector fs(dbus);
	fs.ActiveDir(GetHomeDirectory()).ExecuteSelectDir();
	return fs.Get();
}

}