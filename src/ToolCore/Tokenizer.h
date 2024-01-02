#ifndef _ToolCore_Tokenizer_h_
#define _ToolCore_Tokenizer_h_


class NaturalTokenizer {
	WString ws;
	Vector<Vector<WString>> lines;
	
	static bool IsToken(int chr);
	
public:
	typedef NaturalTokenizer CLASSNAME;
	NaturalTokenizer();
	
	bool Parse(const String& txt);
	
	const Vector<Vector<WString>>& GetLines() const {return lines;}
	
};


#endif
