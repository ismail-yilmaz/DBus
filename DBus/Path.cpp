#include "DBus.h"

namespace Upp {

namespace {

String DecodeDBusPath(const String& val)
{
	String res;
	for(int i = 0; i < val.GetCount(); i++) {
		if(val[i] == '%' && i + 2 < val.GetCount() && IsXDigit(val[i + 1]) && IsXDigit(val[i + 2])) {
			auto HexToNibble = [](char c) -> int {
				if(c >= '0' && c <= '9')
					return c - '0';
				if(c >= 'A' && c <= 'F')
					return c - 'A' + 10;
				if(c >= 'a' && c <= 'f')
					return c - 'a' + 10;
				return 0;
			};
			res.Cat((char)((HexToNibble(val[i + 1]) << 4) | HexToNibble(val[i + 2])));
			i += 2;
		}
		else
			res.Cat(val[i]);
	}
	return res;
}

}

void DBusPathInfo::Parse(const String& addr)
{
	Clear();
	path = addr;
	String s = addr;
	
	// D-Bus addresses can contain multiple fallbacks separated by ';'.
	// We parse the primary (first) endpoint.
	
	int semi = s.Find(';');
	if(semi >= 0)
		s = s.Left(semi);
		
	int colon = s.Find(':');
	if(colon < 0)
		return;

	method = s.Left(colon);
	String params = s.Mid(colon + 1);

	for(const String& pair : Split(params, ',')) {
		if(int eq = pair.Find('='); eq >= 0) {
			String key = pair.Left(eq);
			String val = DecodeDBusPath(pair.Mid(eq + 1));
			parameters.Add(key, val);
		}
	}
}

}