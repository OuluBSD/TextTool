#include "Database.h"


void Line::ParseLine(const SnapArg& a, const String& txt) {
	Vector<String> parts = Split(txt, "[br]");
	breaks.SetCount(parts.GetCount());
	for(int i = 0; i < breaks.GetCount(); i++) {
		String& s = parts[i];
		s = TrimBoth(s);
		Break& b = breaks[i];
		PatternSnap& snap = b.snap[a];
		snap.Clear();
		snap.txt = s;
		snap.syllables = max(1, s.GetCount() / 3); // 3 chars per syllable by average
	}
	String joined = Join(parts, " ");
	this->snap[a].txt = joined;
	ASSERT(this->snap[a].txt.GetCount());
	FixPtrs();
}

int Line::GetLength(const SnapArg& a) const {
	a.Chk();
	int len = 0;
	for (const Break& brk : breaks)
		len += brk.GetLength(a);
	return len;
}

int Line::GetBreakIdx(const Break& b0) const {
	int i = 0;
	for (const Break& b : breaks) {
		if (&b == &b0)
			return i;
		i++;
	}
	return -1;
}
