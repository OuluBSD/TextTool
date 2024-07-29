#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


BiographyProcess::BiographyProcess() {
	
}

int BiographyProcess::GetPhaseCount() const {
	return PHASE_COUNT;
}

int BiographyProcess::GetBatchCount(int phase) const {
	switch (phase) {
		case PHASE_ELEMENTS_SINGLE_YEAR:	return snap->data.AllCategories().GetCount();
		default: TODO; return 1;
	}
}

int BiographyProcess::GetSubBatchCount(int phase, int batch) const {
	switch (phase) {
		case PHASE_ELEMENTS_SINGLE_YEAR:	return snap->data.AllCategories()[batch].summaries.GetCount();
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
	auto& data_categories = data.AllCategories();
	
	if (batch >= data_categories.GetCount()) {
		NextPhase();
		return;
	}
	BiographyCategory& bcat = data_categories[batch];
	
	
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
	args.category = data.GetCategoryName(batch);
	args.text = by.text;
	args.year = by.year;
	
	SetWaiting(true);
	TaskMgr& m = TaskMgr::Single();
	m.GetBiography(args, [this](String result) {
		auto& data_categories = snap->data.AllCategories();
		BiographyCategory& bcat = data_categories[batch];
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
			int i = by.FindElement(key);
			if (lvalue.IsEmpty() || lvalue == "none." || lvalue == "none" || lvalue.Left(6) == "none (") {
				if (i >= 0)
					by.elements.Remove(i);
				continue;
			}
			if (i < 0) {
				i = by.elements.GetCount();
				by.elements.Add();
			}
			auto& el = by.elements[i];
			el.key = key;
			el.value = value;
		}
		
		NextSubBatch();
		SetWaiting(false);
	});
}


END_TEXTLIB_NAMESPACE

