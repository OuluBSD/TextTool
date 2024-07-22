#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE



AmbiguousWordPairsProcess::AmbiguousWordPairsProcess() {
	
}

int AmbiguousWordPairsProcess::GetPhaseCount() const {
	return PHASE_COUNT;
}

int AmbiguousWordPairsProcess::GetBatchCount(int phase) const {
	TODO ; return 0;
}

int AmbiguousWordPairsProcess::GetSubBatchCount(int phase, int batch) const {
	TODO ; return 0;
}

void AmbiguousWordPairsProcess::DoPhase() {
	
	TODO
	
}

AmbiguousWordPairsProcess& AmbiguousWordPairsProcess::Get(int appmode) {
	static ArrayMap<String, AmbiguousWordPairsProcess> arr;
	
	String key = "APPMODE(" + IntStr(appmode) + ")";
	AmbiguousWordPairsProcess& ts = arr.GetAdd(key);
	ts.appmode = appmode;
	return ts;
}



END_TEXTLIB_NAMESPACE

