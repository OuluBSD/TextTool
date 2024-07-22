#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE



VirtualPhrasesProcess::VirtualPhrasesProcess() {
	
}

int VirtualPhrasesProcess::GetPhaseCount() const {
	return PHASE_COUNT;
}

int VirtualPhrasesProcess::GetBatchCount(int phase) const {
	TODO ; return 0;
}

int VirtualPhrasesProcess::GetSubBatchCount(int phase, int batch) const {
	TODO ; return 0;
}

void VirtualPhrasesProcess::DoPhase() {
	
	TODO
	
}

VirtualPhrasesProcess& VirtualPhrasesProcess::Get(int appmode) {
	static ArrayMap<String, VirtualPhrasesProcess> arr;
	
	String key = "APPMODE(" + IntStr(appmode) + ")";
	VirtualPhrasesProcess& ts = arr.GetAdd(key);
	ts.appmode = appmode;
	return ts;
}



END_TEXTLIB_NAMESPACE

