#ifndef _FileChooser_h_
#define _FileChooser_h_

#include <DBus/DBus.h>

namespace Upp {

#define  TFILE <examples/FileChooser/FileChooser.t>
#include <Core/t.h>

class DBusFileSel {
public:
	DBusFileSel()															{}
	
	DBusFileSel&           Type(const char *desc, const char *pattern)		{ types.Add(desc, pattern); return *this; }
	DBusFileSel&           AllFileTypes()									{ return Type(t_("All Files"), "*"); }
	DBusFileSel&           ClearTypes()										{ types.Clear(); return *this; }
	DBusFileSel&           ActiveDir(const char *dir)                       { activedir = dir; return *this; }
	DBusFileSel&           ActiveFile(const char *name)                     { activefile = name; return *this; }
	DBusFileSel&           Multi(bool b = true)                             { multisel = b; return *this; }
		                   
	bool                   ExecuteOpen(const char *title = nullptr)         { return DoExecute("OpenFile", title ? title : t_("Open File"), false); }
	bool                   ExecuteSaveAs(const char *title = nullptr)       { return DoExecute("SaveFile", title ? title : t_("Save File"), false); }
	bool                   ExecuteSelectDir(const char *title = nullptr)    { return DoExecute("OpenFile", title ? title : t_("Select Folder"), true); }
	
	String                 Get() const                                      { return selected.GetCount() > 0 ? selected[0] : Null; }
	const Vector<String>&  GetFiles() const                                 { return selected; }
	
private:
	bool                   DoExecute(const char *method, const String& title, bool asdir);

	String                 activedir;
	String                 activefile;
	Vector<String>         selected;
	VectorMap<String, String> types;
	bool                   multisel = false;
};

}

#endif
