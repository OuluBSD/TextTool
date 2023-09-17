#include "SongTool.h"


ChecklistComposition::ChecklistComposition() {
	
}

void ChecklistComposition::InitKeys() {
	#define CHK_COMP(t, e, s) descs.Add(s); keys.Add(#e);
	CHK_COMPOSITION_LIST
	#undef CHK_COMP
}



ChecklistProduction::ChecklistProduction() {
	
}

void ChecklistProduction::InitKeys() {
	#define CHK_PRODUCTION(t, e, s) descs.Add(s); keys.Add("MPROD_" #e);
	CHK_PRODUCTION_LIST
	#undef CHK_PRODUCTION
}



ChecklistMusicVideoProduction::ChecklistMusicVideoProduction() {
	
}

void ChecklistMusicVideoProduction::InitKeys() {
	#define CHK_MUSICVID(e, s) descs.Add(s); keys.Add("VPROD_" #e);
	CHK_MUSICVID_LIST
	#undef CHK_MUSICVID
}





ChecklistRelease::ChecklistRelease() {
	
}

void ChecklistRelease::InitKeys() {
	#define CHK_RELEASE(e, s) descs.Add(s); keys.Add("REL_" #e);
	CHK_RELEASE_LIST
	#undef CHK_RELEASE
}






SocialMediaChecklist::SocialMediaChecklist() {
	
}

void SocialMediaChecklist::InitKeys() {
	#define CHK_SOME(e, s) descs.Add(s); keys.Add("REL_" #e);
	CHK_SOME_LIST
	#undef CHK_SOME
}

