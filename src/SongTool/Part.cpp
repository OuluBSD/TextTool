#include "SongTool.h"


int Part::GetLength(int mode) const {
	CHKMODE(mode);
	int len = 0;
	for (const Line& l : lines)
		len += l.GetLength(mode);
	return len;
}
