#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


ScriptTextProcess::ScriptTextProcess() {
	
}

int ScriptTextProcess::GetPhaseCount() const {
	return PHASE_COUNT;
}

int ScriptTextProcess::GetBatchCount(int phase) const {
	return 1;
}

int ScriptTextProcess::GetSubBatchCount(int phase, int batch) const {
	return 1;
}

void ScriptTextProcess::DoPhase() {
	
}

ScriptTextProcess& ScriptTextProcess::Get(int appmode) {
	static ArrayMap<String, ScriptTextProcess> arr;
	
	String key = "APPMODE(" + IntStr(appmode) + ")";
	ScriptTextProcess& ts = arr.GetAdd(key);
	ts.appmode = appmode;
	return ts;
}


END_TEXTLIB_NAMESPACE
