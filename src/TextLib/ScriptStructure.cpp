#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


ScriptStructureProcess::ScriptStructureProcess() {
	
}

int ScriptStructureProcess::GetPhaseCount() const {
	return PHASE_COUNT;
}

int ScriptStructureProcess::GetBatchCount(int phase) const {
	return 1;
}

int ScriptStructureProcess::GetSubBatchCount(int phase, int batch) const {
	return 1;
}

void ScriptStructureProcess::DoPhase() {
	
}

ScriptStructureProcess& ScriptStructureProcess::Get(int appmode) {
	static ArrayMap<String, ScriptStructureProcess> arr;
	
	String key = "APPMODE(" + IntStr(appmode) + ")";
	ScriptStructureProcess& ts = arr.GetAdd(key);
	ts.appmode = appmode;
	return ts;
}


END_TEXTLIB_NAMESPACE
