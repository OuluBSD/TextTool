#include "SongTool.h"


void Line::ParseLine(Song& song, int mode, const String& txt) {
	breaks.Clear();
	Vector<String> parts = Split(txt, "[br]");
	breaks.SetCount(parts.GetCount());
	for(int i = 0; i < breaks.GetCount(); i++) {
		String& s = parts[i];
		s = TrimBoth(s);
		Break& b = breaks[i];
		b.snap[mode].txt = s;
		song.headers[mode].unique_lines.GetAdd(s);
	}
	this->snap[mode].txt = Join(parts, " ");
	FixPtrs();
}
