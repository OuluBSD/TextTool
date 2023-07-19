#include "SongTool.h"


void Line::ParseLine(Song& song, int mode, const String& txt) {
	Vector<String> parts = Split(txt, "[br]");
	breaks.SetCount(parts.GetCount());
	for(int i = 0; i < breaks.GetCount(); i++) {
		String& s = parts[i];
		s = TrimBoth(s);
		Break& b = breaks[i];
		PatternSnap& snap = b.snap[mode];
		snap.Clear();
		snap.txt = s;
		snap.syllables = max(1, s.GetCount() / 3); // 3 chars per syllable by average
		song.headers[mode].unique_lines.GetAdd(s);
	}
	this->snap[mode].txt = Join(parts, " ");
	ASSERT(this->snap[mode].txt.GetCount());
	FixPtrs();
}

int Line::GetLength(int mode) const {
	CHKMODE(mode);
	int len = 0;
	for (const Break& brk : breaks)
		len += brk.GetLength(mode);
	return len;
}
