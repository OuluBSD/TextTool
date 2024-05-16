#include "TextDatabase.h"


BEGIN_TEXTLIB_NAMESPACE


void PlatformComment::ClearMerged() {
	text_merged_status.Clear();
	for (auto& o : responses)
		o.ClearMerged();
}

int PlatformData::GetTotalEntryCount() const {
	int i = 0;
	for (const auto& e : entries)
		i += e.threads.GetCount();
	return i;
}


END_TEXTLIB_NAMESPACE

