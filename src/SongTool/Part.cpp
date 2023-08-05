#include "SongTool.h"


int Part::GetLength(const SnapArg& a) const {
	a.Chk();
	int len = 0;
	for (const Line& l : lines)
		len += l.GetLength(a);
	return len;
}

int Part::GetLineIdx(const Line& l0) const {
	int i = 0;
	for (const Line& l : lines) {
		if (&l == &l0)
			return i;
		i++;
	}
	return -1;
}

String Part::CreateLyricsFromBreaks(const SnapArg& a, bool add_comma) const {
	String s;
	for (const Line& l : lines) {
		if (!s.IsEmpty()) s << "; ";
		int brk_i = 0;
		for (const Break& b : l.breaks) {
			if (brk_i++) {
				if (add_comma) s << ",";
				s << " ";
			}
			s << b.snap[a].txt;
		}
	}
	return s;
}
