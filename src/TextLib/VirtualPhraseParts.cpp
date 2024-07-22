#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE



VirtualPhrasePartsProcess::VirtualPhrasePartsProcess() {
	
}

int VirtualPhrasePartsProcess::GetPhaseCount() const {
	return PHASE_COUNT;
}

int VirtualPhrasePartsProcess::GetBatchCount(int phase) const {
	TODO ; return 0;
}

int VirtualPhrasePartsProcess::GetSubBatchCount(int phase, int batch) const {
	TODO ; return 0;
}

void VirtualPhrasePartsProcess::DoPhase() {
	
	TODO
	
}

VirtualPhrasePartsProcess& VirtualPhrasePartsProcess::Get(int appmode) {
	static ArrayMap<String, VirtualPhrasePartsProcess> arr;
	
	String key = "APPMODE(" + IntStr(appmode) + ")";
	VirtualPhrasePartsProcess& ts = arr.GetAdd(key);
	ts.appmode = appmode;
	return ts;
}



END_TEXTLIB_NAMESPACE

