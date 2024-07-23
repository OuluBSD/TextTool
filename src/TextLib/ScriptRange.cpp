#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


ScriptRangeProcess::ScriptRangeProcess() {
	
}

int ScriptRangeProcess::GetPhaseCount() const {
	return PHASE_COUNT;
}

int ScriptRangeProcess::GetBatchCount(int phase) const {
	return 1;
}

int ScriptRangeProcess::GetSubBatchCount(int phase, int batch) const {
	return 1;
}

void ScriptRangeProcess::DoPhase() {
	
}

ScriptRangeProcess& ScriptRangeProcess::Get(int appmode) {
	static ArrayMap<String, ScriptRangeProcess> arr;
	
	String key = "APPMODE(" + IntStr(appmode) + ")";
	ScriptRangeProcess& ts = arr.GetAdd(key);
	ts.appmode = appmode;
	return ts;
}


END_TEXTLIB_NAMESPACE
