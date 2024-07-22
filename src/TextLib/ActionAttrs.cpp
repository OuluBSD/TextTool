#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE



ActionAttrsProcess::ActionAttrsProcess() {
	
}

int ActionAttrsProcess::GetPhaseCount() const {
	return PHASE_COUNT;
}

int ActionAttrsProcess::GetBatchCount(int phase) const {
	TODO ; return 0;
}

int ActionAttrsProcess::GetSubBatchCount(int phase, int batch) const {
	TODO ; return 0;
}

void ActionAttrsProcess::DoPhase() {
	
	TODO
	
}

ActionAttrsProcess& ActionAttrsProcess::Get(int appmode) {
	static ArrayMap<String, ActionAttrsProcess> arr;
	
	String key = "APPMODE(" + IntStr(appmode) + ")";
	ActionAttrsProcess& ts = arr.GetAdd(key);
	ts.appmode = appmode;
	return ts;
}



END_TEXTLIB_NAMESPACE

