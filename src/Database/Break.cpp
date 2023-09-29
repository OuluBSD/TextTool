#include "Database.h"

int Break::GetLength(const SnapArg& a) const {
	a.Chk();
	const PatternSnap& snap = this->Get(a);
	if (snap.syllables == 0 && snap.txt.GetCount())  {
		PatternSnap& s = const_cast<PatternSnap&>(snap);
		s.syllables = max(1, s.txt.GetCount() / 3);
	}
	return snap.syllables;
}
