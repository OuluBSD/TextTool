#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE



RhymeContainerProcess::RhymeContainerProcess() {
	
}

int RhymeContainerProcess::GetPhaseCount() const {
	return PHASE_COUNT;
}

int RhymeContainerProcess::GetBatchCount(int phase) const {
	TODO ; return 0;
}

int RhymeContainerProcess::GetSubBatchCount(int phase, int batch) const {
	TODO ; return 0;
}

void RhymeContainerProcess::DoPhase() {
	
	TODO
	
}

RhymeContainerProcess& RhymeContainerProcess::Get(int appmode) {
	static ArrayMap<String, RhymeContainerProcess> arr;
	
	String key = "APPMODE(" + IntStr(appmode) + ")";
	RhymeContainerProcess& ts = arr.GetAdd(key);
	ts.appmode = appmode;
	return ts;
}



END_TEXTLIB_NAMESPACE

