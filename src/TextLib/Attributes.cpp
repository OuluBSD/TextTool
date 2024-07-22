#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE



AttributesProcess::AttributesProcess() {
	
}

int AttributesProcess::GetPhaseCount() const {
	return PHASE_COUNT;
}

int AttributesProcess::GetBatchCount(int phase) const {
	TODO ; return 0;
}

int AttributesProcess::GetSubBatchCount(int phase, int batch) const {
	TODO ; return 0;
}

void AttributesProcess::DoPhase() {
	
	TODO
	
}

AttributesProcess& AttributesProcess::Get(int appmode) {
	static ArrayMap<String, AttributesProcess> arr;
	
	String key = "APPMODE(" + IntStr(appmode) + ")";
	AttributesProcess& ts = arr.GetAdd(key);
	ts.appmode = appmode;
	return ts;
}



END_TEXTLIB_NAMESPACE

