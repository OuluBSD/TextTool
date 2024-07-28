#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


BiographyProcess::BiographyProcess() {
	
}

int BiographyProcess::GetPhaseCount() const {
	return PHASE_COUNT;
}

int BiographyProcess::GetBatchCount(int phase) const {
	switch (phase) {
		case PHASE_ELEMENTS_SINGLE_YEAR:	return snap->data.categories.GetCount();
		default: TODO; return 1;
	}
}

int BiographyProcess::GetSubBatchCount(int phase, int batch) const {
	switch (phase) {
		case PHASE_ELEMENTS_SINGLE_YEAR:	return snap->data.categories[batch].summaries.GetCount();
		default: TODO; return 1;
	}
}

void BiographyProcess::DoPhase() {
	switch (phase) {
		case PHASE_ELEMENTS_SINGLE_YEAR:	ElementsForSingleYears(); return;
		default: TODO; return;
	}
}

BiographyProcess& BiographyProcess::Get(Profile& p, BiographySnapshot& snap) {
	static ArrayMap<String, BiographyProcess> arr;
	
	String key = "PROFILE(" + p.name + "), REVISION(" + IntStr(snap.revision) + ")";
	BiographyProcess& ts = arr.GetAdd(key);
	ts.profile = &p;
	ts.snap = &snap;
	return ts;
}

void BiographyProcess::ElementsForSingleYears() {
	Biography& data = snap->data;
	BiographyAnalysis& analysis = snap->analysis;
	
	
	if (batch >= data.categories.GetCount()) {
		NextPhase();
		return;
	}
	BiographyCategory& bcat = data.categories[batch];
	
	
	if (sub_batch >= bcat.years.GetCount()) {
		NextBatch();
		return;
	}
	BioYear& by = bcat.years[sub_batch];
	
	if (by.text.IsEmpty()) {
		NextSubBatch();
		return;
	}
	
	
	BiographyProcessArgs args;
	args.fn = 0;
	args.category = KeyToName(data.categories.GetKey(batch));
	args.text = by.text;
	args.year = by.year;
	
	SetWaiting(true);
	TaskMgr& m = TaskMgr::Single();
	m.GetBiography(appmode, args, [this](String result) {
		Biography& data = snap->data;
		BiographyCategory& bcat = data.categories[batch];
		BioYear& by = bcat.years[sub_batch];
		
		RemoveEmptyLines3(result);
		Vector<String> lines = Split(result, "\n");
		
		snap->last_modified = GetSysTime();
		by.elements.Clear();
		
		for (String& line : lines) {
			int a = line.Find(":");
			if (a < 0) continue;
			String key = ToLower(TrimBoth(line.Left(a)));
			String value = TrimBoth(line.Mid(a+1));
			RemoveQuotes(value);
			String lvalue = ToLower(value);
			if (lvalue.IsEmpty() || lvalue == "none." || lvalue == "none" || lvalue.Left(6) == "none (") {
				int i = by.elements.Find(key);
				if (i >= 0)
					by.elements.Remove(i);
				continue;
			}
			by.elements.GetAdd(key) = value;
		}
		
		NextSubBatch();
		SetWaiting(false);
	});
}


END_TEXTLIB_NAMESPACE

