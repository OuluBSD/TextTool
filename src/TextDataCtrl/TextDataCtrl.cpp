#include "TextDataCtrl.h"


BEGIN_TEXTLIB_NAMESPACE



TextDatabase& ToolAppCtrl::GetDatabase() {
	#if 1
	return GetAppModeDatabase();
	#else
	if (appmode < 0 || appmode >= DB_COUNT)
		throw NoPointerExc("Invalid appmode");
	return MetaDatabase::Single().db[appmode];
	#endif
}

EditorPtrs& ToolAppCtrl::GetPointers() {
	#if 1
	return GetAppModePointers();
	#else
	if (appmode < 0 || appmode >= DB_COUNT)
		throw NoPointerExc("Invalid appmode");
	return MetaPtrs::Single().db[appmode];
	#endif
}


END_TEXTLIB_NAMESPACE
