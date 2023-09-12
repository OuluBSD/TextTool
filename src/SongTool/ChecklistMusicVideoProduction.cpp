#include "SongTool.h"



ChecklistMusicVideoProduction::ChecklistMusicVideoProduction() {
	
}

void ChecklistMusicVideoProduction::InitKeys() {
	#define CHK_MUSICVID(e, s) descs.Add(s); keys.Add(#e);
	CHK_MUSICVID_LIST
	#undef CHK_MUSICVID
}
