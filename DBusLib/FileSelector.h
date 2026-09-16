#ifndef _DBusLib_FileSelector_h_
#define _DBusLib_FileSelector_h_

#include <DBus/DBus.h>

namespace Upp {
	
class DBusFileSelector {
public:
	DBusFileSelector(DBusConnection& session);
	virtual ~DBusFileSelector()                                                  {}
	
	DBusFileSelector&      Type(const String& desc, const String& pattern)  { types.Add(desc, pattern); return *this; }
	DBusFileSelector&      AllFileTypes()									{ return Type(t_("All Files"), "*"); }
	DBusFileSelector&      ClearTypes()										{ types.Clear(); return *this; }
	DBusFileSelector&      ActiveDir(const String& dir)                     { activedir = dir; return *this; }
	DBusFileSelector&      ActiveFile(const String& name)                   { activefile = name; return *this; }
	DBusFileSelector&      Multi(bool b = true)                             { multisel = b; return *this; }
		                   
	bool                   ExecuteOpen(const String& title = Null)          { return Run("OpenFile", Nvl(title, t_("Open File")), false); }
	bool                   ExecuteSaveAs(const String& title = Null)        { return Run("SaveFile", Nvl(title, t_("Save File")), false); }
	bool                   ExecuteSelectDir(const String& title = Null)     { return Run("OpenFile", Nvl(title, t_("Select Folder")), true); }
	
	String                 Get() const                                      { return selected.GetCount() > 0 ? selected[0] : Null; }
	const Vector<String>&  GetFiles() const                                 { return selected; }
	
private:
	bool                   Run(const char *method, const String& title, bool asdir);

	DBusConnection&        dbus;
	String                 activedir;
	String                 activefile;
	Vector<String>         selected;
	VectorMap<String, String> types;
	bool                   multisel = false;
};

String         SelectOpenFile(DBusConnection& dbus, const String& types = Null);
Vector<String> SelectOpenFiles(DBusConnection& dbus, const String& types = Null);
String         SelectLoadFile(DBusConnection& dbus, const String& types = Null);
bool           SelectSaveFile(DBusConnection& dbus, const String& data, const String& types = Null);
String         SelectDirectory(DBusConnection& dbus);

inline constexpr const char *StdDBusFileChooserName      = "org.freedesktop.portal.Desktop";
inline constexpr const char *StdDBusFileChooserPath      = "/org/freedesktop/portal/desktop";
inline constexpr const char *StdDBusFileChooserInterface = "org.freedesktop.portal.FileChooser";

}

#endif
