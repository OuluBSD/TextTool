#include "TextDataCtrl.h"


BEGIN_TEXTLIB_NAMESPACE



TextDatabase& ToolAppCtrl::GetDatabase() {
	if (appmode < 0 || appmode >= DB_COUNT)
		throw NoPointerExc("Invalid appmode");
	return MetaDatabase::Single().db[appmode];
}

EditorPtrs& ToolAppCtrl::GetPointers() {
	if (appmode < 0 || appmode >= DB_COUNT)
		throw NoPointerExc("Invalid appmode");
	return MetaPtrs::Single().db[appmode];
}


END_TEXTLIB_NAMESPACE
