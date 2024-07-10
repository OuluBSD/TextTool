#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE




SourceDataImporter::SourceDataImporter() {
	
}

int SourceDataImporter::GetPhaseCount() const {
	return PHASE_COUNT;
}

void SourceDataImporter::DoPhase() {
	
}

SourceDataImporter& SourceDataImporter::Get(int appmode) {
	static ArrayMap<String, SourceDataImporter> arr;
	
	String key = "APPMODE(" + IntStr(appmode) + ")";
	SourceDataImporter& ts = arr.GetAdd(key);
	ts.appmode = appmode;
	return ts;
}


END_TEXTLIB_NAMESPACE

