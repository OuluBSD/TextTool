#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE



TokenPhrasesProcess::TokenPhrasesProcess() {
	
}

int TokenPhrasesProcess::GetPhaseCount() const {
	return PHASE_COUNT;
}

int TokenPhrasesProcess::GetBatchCount(int phase) const {
	TODO ; return 0;
}

int TokenPhrasesProcess::GetSubBatchCount(int phase, int batch) const {
	TODO ; return 0;
}

void TokenPhrasesProcess::DoPhase() {
	
	TODO
	
}

TokenPhrasesProcess& TokenPhrasesProcess::Get(int appmode) {
	static ArrayMap<String, TokenPhrasesProcess> arr;
	
	String key = "APPMODE(" + IntStr(appmode) + ")";
	TokenPhrasesProcess& ts = arr.GetAdd(key);
	ts.appmode = appmode;
	return ts;
}



END_TEXTLIB_NAMESPACE

