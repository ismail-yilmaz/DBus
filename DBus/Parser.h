class BParser { // Like CParser, but for binary data.
protected:
	MemReadStream stream;
	String        fn;
	bool          bigendian;

	void          Check(int64 n);

public:
	struct Error : public Exc                 { Error(const char *s) : Exc(s) {} };

	void ThrowError(const char *s);
	void ThrowError()                         { ThrowError(""); }

	byte   PeekByte()                         { const byte *p = stream.PeekPtr(1); return p ? *p : 0; }
	byte   GetByte()                          { Check(1); return (byte) stream.Get8(); }

	bool   IsAvail(int64 n) const             { return n >= 0 && n <= stream.GetLeft(); }
	bool   IsEof()                            { return stream.IsEof(); }
	operator bool()                           { return !IsEof(); }

	int8    ReadInt8()                        { Check(1); return (int8) stream.Get8(); }
	int16   ReadInt16()                       { Check(2); return (int16)  (bigendian ? stream.Get16be() : stream.Get16le()); }
	word    ReadWord()                        { Check(2); return (word)   (bigendian ? stream.Get16be() : stream.Get16le()); }
	int32   ReadInt32()                       { Check(4); return (int32)  (bigendian ? stream.Get32be() : stream.Get32le()); }
	uint32  ReadUInt32()                      { Check(4); return (uint32) (bigendian ? stream.Get32be() : stream.Get32le()); }
	dword   ReadDword()                       { return (dword) ReadUInt32(); } // for convenience
	int64   ReadInt64()                       { Check(8); return (int64)  (bigendian ? stream.Get64be() : stream.Get64le()); }
	uint64  ReadUInt64()                      { Check(8); return (uint64) (bigendian ? stream.Get64be() : stream.Get64le()); }
	qword   ReadQword()                       { return (qword) ReadUInt64(); } // for convenience
	float   ReadFloat();
	double  ReadDouble();

	int32   ReadInt32(int32 min, int32 max);
	int64   ReadInt64(int64 min, int64 max);

	void    ReadBytes(void *dst, int64 n)     { Check(n); stream.Get64(dst, n); }
	String  ReadString(int64 n);
	String  ReadAll()                         { return ReadString(GetCount()); }

	String  ReadPString8()                    { return ReadString(GetByte()); }   // byte-length-prefixed string
	String  ReadPString16()                   { return ReadString(ReadWord()); }  // word-length-prefixed string
	String  ReadPString32()                   { return ReadString(ReadDword()); } // dword-length-prefixed string (e.g. SSH wire "string")

	void    Skip(int64 n)                     { Check(n); stream.SeekCur(n); }
	void    SkipByte()                        { Skip(1); }
	void    Align(int n);

	void    Seek(int64 offset);
	void    SeekCur(int64 offset)             { Seek(GetOffset() + offset); }

	int64  GetOffset() const                  { return stream.GetPos(); }
	int64  GetCount() const                   { return stream.GetLeft(); }
	int64  GetLength() const                  { return stream.GetSize(); }
	String GetFileName() const                { return fn; }

	MemReadStream& GetStream()                { return stream; }

	BParser& BigEndian(bool b = true)         { bigendian = b; return *this; }
	BParser& LittleEndian(bool b = true)      { bigendian = !b; return *this; }
	bool     IsBigEndian() const              { return bigendian; }

	void   Set(const void *ptr, int64 size, const char *fn);
	void   Set(const void *ptr, int64 size);

	BParser(const void *ptr, int64 size);
	BParser(const void *ptr, int64 size, const char *fn);
	BParser(const String& s);
	BParser();
};
