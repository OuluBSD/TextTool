#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


WordDataProcess::WordDataProcess() {
	
}

int WordDataProcess::GetPhaseCount() const {
	return PHASE_COUNT;
}

int WordDataProcess::GetBatchCount(int phase) const {
	TODO ; return 0;
}

int WordDataProcess::GetSubBatchCount(int phase, int batch) const {
	TODO ; return 0;
}

void WordDataProcess::DoPhase() {
	
	TODO
	
}

WordDataProcess& WordDataProcess::Get(int appmode) {
	static ArrayMap<String, WordDataProcess> arr;
	
	String key = "APPMODE(" + IntStr(appmode) + ")";
	WordDataProcess& ts = arr.GetAdd(key);
	ts.appmode = appmode;
	return ts;
}


END_TEXTLIB_NAMESPACE

