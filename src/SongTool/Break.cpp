#include "SongTool.h"

int Break::GetLength(int mode) const {
	CHKMODE(mode);
	const PatternSnap& snap = this->snap[mode];
	if (snap.syllables == 0 && snap.txt.GetCount())  {
		PatternSnap& s = const_cast<PatternSnap&>(snap);
		s.syllables = max(1, s.txt.GetCount() / 3);
	}
	return snap.syllables;
}
