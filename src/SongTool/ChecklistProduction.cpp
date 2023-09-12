#include "SongTool.h"



ChecklistProduction::ChecklistProduction() {
	
}

void ChecklistProduction::InitKeys() {
	#define CHK_PRODUCTION(e, s) descs.Add(s); keys.Add(#e);
	CHK_PRODUCTION_LIST
	#undef CHK_PRODUCTION
}
