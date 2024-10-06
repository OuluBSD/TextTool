#include "TextDataCtrl.h"
#include <TextTool/TextTool.h>


BEGIN_TEXTLIB_NAMESPACE



TextDatabase& ToolAppCtrl::GetDatabase() const {
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

bool ToolAppCtrl::HasPointers() const {
	return HasAnyEditor();
}

EditorPtrs& ToolAppCtrl::GetPointers() const {
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

const Index<String>& ToolAppCtrl::GetTypeclasses() const {
	return TextLib::GetTypeclasses(GetAnyEditor().GetAppMode());
}

const Vector<ContentType>& ToolAppCtrl::GetContents() const {
	return TextLib::GetContents(GetAnyEditor().GetAppMode());
}

const Vector<String>& ToolAppCtrl::GetContentParts() const {
	return TextLib::GetContentParts(GetAnyEditor().GetAppMode());
}



END_TEXTLIB_NAMESPACE
