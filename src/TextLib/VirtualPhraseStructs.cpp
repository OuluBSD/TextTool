#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE



VirtualPhraseStructsProcess::VirtualPhraseStructsProcess() {
	
}

int VirtualPhraseStructsProcess::GetPhaseCount() const {
	return PHASE_COUNT;
}

int VirtualPhraseStructsProcess::GetBatchCount(int phase) const {
	TODO ; return 0;
}

int VirtualPhraseStructsProcess::GetSubBatchCount(int phase, int batch) const {
	TODO ; return 0;
}

void VirtualPhraseStructsProcess::DoPhase() {
	
	TODO
	
}

VirtualPhraseStructsProcess& VirtualPhraseStructsProcess::Get(int appmode) {
	static ArrayMap<String, VirtualPhraseStructsProcess> arr;
	
	String key = "APPMODE(" + IntStr(appmode) + ")";
	VirtualPhraseStructsProcess& ts = arr.GetAdd(key);
	ts.appmode = appmode;
	return ts;
}



END_TEXTLIB_NAMESPACE

