#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


BiographyElementsProcess::BiographyElementsProcess() {
	
}

int BiographyElementsProcess::GetPhaseCount() const {
	return PHASE_COUNT;
}

int BiographyElementsProcess::GetBatchCount(int phase) const {
	return 1;
}

int BiographyElementsProcess::GetSubBatchCount(int phase, int batch) const {
	return 1;
}

void BiographyElementsProcess::DoPhase() {
	switch (phase) {
		case PHASE_SEARCH_TOPIC_BUNDLES:		SearchTopicBundles(); return;
		case PHASE_GET_TOPIC_BUNDLE_ELEMENTS:	GetTopicBundleElements(); return;
		default: TODO; return;
	}
}

BiographyElementsProcess& BiographyElementsProcess::Get(Profile& p, BiographySnapshot& snap) {
	static ArrayMap<String, BiographyElementsProcess> arr;
	
	String key = "PROFILE(" + p.name + "), REVISION(" + IntStr(snap.revision) + ")";
	BiographyElementsProcess& ts = arr.GetAdd(key);
	ts.profile = &p;
	ts.snap = &snap;
	return ts;
}

void BiographyElementsProcess::SearchTopicBundles() {
	
	TODO
	
}

void BiographyElementsProcess::GetTopicBundleElements() {
	
	TODO
	
}



END_TEXTLIB_NAMESPACE

