#include "SongTool.h"


void Line::ParseLine(Song& song, const String& txt) {
	breaks.Clear();
	Vector<String> parts = Split(txt, "[br]");
	breaks.SetCount(parts.GetCount());
	for(int i = 0; i < breaks.GetCount(); i++) {
		String& s = parts[i];
		s = TrimBoth(s);
		Break& b = breaks[i];
		b.txt = s;
		song.unique_lines.GetAdd(s);
	}
	this->txt = Join(parts, " ");
	FixPtrs();
}
