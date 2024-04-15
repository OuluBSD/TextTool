#ifndef _TextOS_Parser_h_
#define _TextOS_Parser_h_


BEGIN_TEXTLIB_NAMESPACE

inline bool IsOctDigit(char c) { return c >= '0' && c <= '7'; }
inline bool IsHexDigit(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }
inline int GetHexDigit(char c) {
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return 10 + c - 'a';
	if (c >= 'A' && c <= 'F')
		return 10 + c - 'A';
	return 0;
}

inline int OctInt(const char *s) {
	if (!s) return 0;
	while (IsSpace(*s)) s++;
	int n=0, neg=0;
	switch (*s) {
		case '-': neg=1;
		case '+': s++;
	}
	while (*s == '0') s++;
	while (IsOctDigit(*s))
		n = 8*n - (*s++ - '0');
	return neg ? n : -n;
}

inline int HexInt(const char *s) {
	if (!s) return 0;
	while (IsSpace(*s)) s++;
	int n=0, neg=0;
	switch (*s) {
		case '-': neg=1;
		case '+': s++;
	}
	if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
		s += 2;
	while (IsHexDigit(*s))
		n = 16*n - GetHexDigit(*s++);
	return neg ? n : -n;
}

inline bool IsBinDigit(int c) { return c == '0' || c == '1'; }
inline bool IsHexDigit(int c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }
inline int GetHexDigit(int c) {
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return 10 + c - 'a';
	if (c >= 'A' && c <= 'F')
		return 10 + c - 'A';
	return 0;
}
inline int GetBinDigit(int c) {
	if (c == '0' || c == '1')
		return c - '0';
	return 0;
}
inline int64 BinInt64(const char *s) {
	if (!s) return 0;
	while (IsSpace(*s)) s++;
	int64 n=0, neg=0;
	switch (*s) {
		case '-': neg=1;
		case '+': s++;
	}
	if (s[0] == '0' && (s[1] == 'b' || s[1] == 'B'))
		s += 2;
	while (IsBinDigit(*s))
		n = 2*n - GetBinDigit(*s++);
	return neg ? n : -n;
}
inline int64 HexInt64(const char *s) {
	if (!s) return 0;
	while (IsSpace(*s)) s++;
	int64 n=0, neg=0;
	switch (*s) {
		case '-': neg=1;
		case '+': s++;
	}
	if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
		s += 2;
	while (IsHexDigit(*s))
		n = 16*n - GetHexDigit(*s++);
	return neg ? n : -n;
}

struct IdParser {
	bool is_valid = false;
	const String& input;
	int pos;
	String id;
	
	IdParser(const String& input, int pos) : input(input), pos(pos) {Parse();}
	
	void Parse();
	
};

struct TokenStringParser {
	bool is_valid = false;
	const String& input;
	int pos;
	String str;
	int delim = '\"';
	bool checkend = true;
	
	TokenStringParser(const String& input, int pos, int delim='\"', bool checkend=true) :
		input(input), pos(pos), delim(delim), checkend(checkend) {Parse();}
	
	void Parse();
	
};

struct NumberParser {
	enum {
		INVALID,
		OCT,
		HEX,
		INT,
		BIN,
		FLOAT,
		DOUBLE
	};
	
	int type = 0;
	const String& input;
	int cursor;
	String str;
	uint32 u32;
	int32 i32;
	int64 i64;
	float f;
	double d;
	
	NumberParser(const String& input, int cursor) : input(input), cursor(cursor) {Parse();}
	
	void Parse();
	
};


END_TEXTLIB_NAMESPACE


#endif
