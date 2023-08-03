#include "SongTool.h"


int Part::GetLength(const SnapArg& a) const {
	a.Chk();
	int len = 0;
	for (const Line& l : lines)
		len += l.GetLength(mode);
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
