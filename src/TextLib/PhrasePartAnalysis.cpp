#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE



PhrasePartAnalysisProcess::PhrasePartAnalysisProcess() {
	
}

int PhrasePartAnalysisProcess::GetPhaseCount() const {
	return PHASE_COUNT;
}

int PhrasePartAnalysisProcess::GetBatchCount(int phase) const {
	TODO ; return 0;
}

int PhrasePartAnalysisProcess::GetSubBatchCount(int phase, int batch) const {
	TODO ; return 0;
}

void PhrasePartAnalysisProcess::DoPhase() {
	
	TODO
	
}

PhrasePartAnalysisProcess& PhrasePartAnalysisProcess::Get(int appmode) {
	static ArrayMap<String, PhrasePartAnalysisProcess> arr;
	
	String key = "APPMODE(" + IntStr(appmode) + ")";
	PhrasePartAnalysisProcess& ts = arr.GetAdd(key);
	ts.appmode = appmode;
	return ts;
}



END_TEXTLIB_NAMESPACE

