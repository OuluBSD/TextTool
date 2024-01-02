#include "ToolCore.h"

NaturalTokenizer::NaturalTokenizer() {
	
}

bool NaturalTokenizer::Parse(const String& txt) {
	lines.Clear();
	Vector<WString> line;
	
	ws = txt.ToWString();
	
	line.SetCount(0);
	WString tmp;
	
	for(int i = 0; i < ws.GetCount(); i++) {
		int chr = ws[i];
		
		if (IsSpace(chr)) {
			if (!tmp.IsEmpty()) {
				line << tmp;
				tmp.Clear();
			}
			if (chr == '\n')
				Swap(line, lines.Add());
		}
		else if (IsToken(chr)) {
			if (!tmp.IsEmpty()) {
				line << tmp;
				tmp.Clear();
			}
			tmp.Cat(chr);
			line << tmp;
			tmp.Clear();
		}
		else {
			tmp.Cat(chr);
		}
	}
	if (!tmp.IsEmpty()) {
		line << tmp;
		tmp.Clear();
		Swap(lines.Add(), line);
	}
	
	return true;
}

bool NaturalTokenizer::IsToken(int chr) {
	const char* c = "!\"#¤\%&/()=?@£$€{[]}\\+`'*^~,.;:-_<>|";
	for (const char* cmp = c; *cmp; cmp++)
		if (*cmp == chr)
			return true;
	return false;
}
