#include "TextDatabase.h"


BEGIN_TEXTLIB_NAMESPACE



int PlatformData::GetTotalEntryCount() const {
	int i = 0;
	for (const auto& t : threads)
		i += t.entries.GetCount();
	return i;
}


END_TEXTLIB_NAMESPACE

