struct DBusPathInfo {
	String                    path;
	String                    method;
	VectorMap<String, String> parameters;
	
	void   Parse(const String& addr);
	String operator[](const char *id) const           { return parameters.Get(id, String::GetVoid()); }

	void   Clear()                                    { *this = DBusPathInfo(); }
	
	DBusPathInfo()                                    {}
	DBusPathInfo(const String& addr)                  { Parse(addr); }
};
