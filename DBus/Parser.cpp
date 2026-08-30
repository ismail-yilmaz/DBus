#include "DBus.h"

namespace Upp {

#define LLOG(x)  // DLOG(x)

void BParser::ThrowError(const char *s)
{
	LLOG("BParser::Error: " << s);
	Error err(fn + "(offset " + GetOffset() + "): " + s);
	throw err;
}

void BParser::Check(int64 n)
{
	if(n < 0 || n > stream.GetLeft())
		ThrowError("unexpected end of data");
}

float BParser::ReadFloat()
{
	dword v = ReadDword();
	float f;
	memcpy(&f, &v, sizeof(f));
	return f;
}

double BParser::ReadDouble()
{
	uint64 v = ReadUInt64();
	double d;
	memcpy(&d, &v, sizeof(d));
	return d;
}

int32 BParser::ReadInt32(int32 min, int32 max)
{
	int32 n = ReadInt32();
	if(n < min || n > max)
		ThrowError("value is out of range");
	return n;
}

int64 BParser::ReadInt64(int64 min, int64 max)
{
	int64 n = ReadInt64();
	if(n < min || n > max)
		ThrowError("value is out of range");
	return n;
}

String BParser::ReadString(int64 n)
{
	Check(n);
	if(n > INT_MAX)                          // String is int-indexed; a dword length
		ThrowError("string is too large");   // prefix (ReadPString32) can exceed that
	return stream.Get((int) n);
}

void BParser::Align(int n)
{
	if(int64 off = GetOffset() % n; off)
		Skip(n - off);
}

void BParser::Seek(int64 offset)
{
	if(offset < 0 || offset > stream.GetSize())
		ThrowError("seek is out of range");
	stream.Seek(offset);
}

void BParser::Set(const void *ptr, int64 size, const char *_fn)
{
	stream.Create(ptr, size);
	fn = _fn;
}

void BParser::Set(const void *ptr, int64 size)
{
	Set(ptr, size, "");
}

BParser::BParser(const void *ptr, int64 size)
: stream(ptr, size)
{
	bigendian = false;
}

BParser::BParser(const void *ptr, int64 size, const char *_fn)
: stream(ptr, size)
, fn(_fn)
{
	bigendian = false;
}

BParser::BParser(const String& s)
: stream((const char *)s, s.GetLength())
{
	bigendian = false;
}

BParser::BParser()
{
	bigendian = false;
}

}