#include "TextDataCtrl.h"
#include <TextTool/TextTool.h>


BEGIN_TEXTLIB_NAMESPACE



TextDatabase& ToolAppCtrl::GetDatabase() {
	#if 1
	return GetAnyEditor().GetDatabase();
	#elif 1
	return GetAppModeDatabase();
	#else
	if (appmode < 0 || appmode >= DB_COUNT)
		throw NoPointerExc("Invalid appmode");
	return MetaDatabase::Single().db[appmode];
	#endif
}

EditorPtrs& ToolAppCtrl::GetPointers() {
	#if 1
	return GetAnyEditor().GetPointers();
	#elif 1
	return GetAppModePointers();
	#else
	if (appmode < 0 || appmode >= DB_COUNT)
		throw NoPointerExc("Invalid appmode");
	return MetaPtrs::Single().db[appmode];
	#endif
}

const Index<String>& ToolAppCtrl::GetTypeclasses() {
	return TextLib::GetTypeclasses(GetAnyEditor().GetAppMode());
}

const Vector<ContentType>& ToolAppCtrl::GetContents() {
	return TextLib::GetContents(GetAnyEditor().GetAppMode());
}

const Vector<String>& ToolAppCtrl::GetContentParts() {
	return TextLib::GetContentParts(GetAnyEditor().GetAppMode());
}



END_TEXTLIB_NAMESPACE
