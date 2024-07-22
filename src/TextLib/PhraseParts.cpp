#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE



PhrasePartsProcess::PhrasePartsProcess() {
	
}

int PhrasePartsProcess::GetPhaseCount() const {
	return PHASE_COUNT;
}

int PhrasePartsProcess::GetBatchCount(int phase) const {
	TODO ; return 0;
}

int PhrasePartsProcess::GetSubBatchCount(int phase, int batch) const {
	TODO ; return 0;
}

void PhrasePartsProcess::DoPhase() {
	
	TODO
	
}

PhrasePartsProcess& PhrasePartsProcess::Get(int appmode) {
	static ArrayMap<String, PhrasePartsProcess> arr;
	
	String key = "APPMODE(" + IntStr(appmode) + ")";
	PhrasePartsProcess& ts = arr.GetAdd(key);
	ts.appmode = appmode;
	return ts;
}



END_TEXTLIB_NAMESPACE

