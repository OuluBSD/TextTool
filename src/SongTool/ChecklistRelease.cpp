#include "SongTool.h"



ChecklistRelease::ChecklistRelease() {
	
}

void ChecklistRelease::InitKeys() {
	#define CHK_RELEASE(e, s) descs.Add(s); keys.Add(#e);
	CHK_RELEASE_LIST
	#undef CHK_RELEASE
}
