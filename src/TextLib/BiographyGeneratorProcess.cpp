#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


BiographyGeneratorProcess::BiographyGeneratorProcess() {
	
}

int BiographyGeneratorProcess::GetPhaseCount() const {
	return PHASE_COUNT;
}

int BiographyGeneratorProcess::GetBatchCount(int phase) const {
	switch (phase) {
		/*case PHASE_ELEMENTS_SINGLE_YEAR:	return snap->data.AllCategories().GetCount();
		case PHASE_ELEMENT_SCORES:			return snap->data.AllCategories().GetCount();*/
		default: return 1;
	}
}

int BiographyGeneratorProcess::GetSubBatchCount(int phase, int batch) const {
	switch (phase) {
		/*case PHASE_ELEMENTS_SINGLE_YEAR:	return snap->data.AllCategories()[batch].summaries.GetCount();
		case PHASE_ELEMENT_SCORES:			return snap->data.AllCategories()[batch].summaries.GetCount();*/
		default: return 1;
	}
}

void BiographyGeneratorProcess::DoPhase() {
	switch (phase) {
		/*case PHASE_ELEMENTS_USING_EXISTING:			GetElementsUsingExisting(); return;
		case PHASE_ELEMENTS_SINGLE_YEAR:			ElementsForSingleYears(); return;
		case PHASE_ELEMENT_SCORES:					GetElementScores(); return;*/
		default: return;
	}
}

BiographyGeneratorProcess& BiographyGeneratorProcess::Get(Profile& p, BiographySnapshot& snap) {
	static ArrayMap<String, BiographyGeneratorProcess> arr;
	
	String key = "PROFILE(" + p.name + "), REVISION(" + IntStr(snap.revision) + ")";
	BiographyGeneratorProcess& ts = arr.GetAdd(key);
	ts.profile = &p;
	ts.owner = p.owner;
	ts.snap = &snap;
	return ts;
}


END_TEXTLIB_NAMESPACE

