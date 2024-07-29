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
		case PHASE_ELEMENT_SCORES:			return snap->data.AllCategories().GetCount();
		default: TODO; return 1;
	}
}

int BiographyProcess::GetSubBatchCount(int phase, int batch) const {
	switch (phase) {
		case PHASE_ELEMENTS_SINGLE_YEAR:	return snap->data.AllCategories()[batch].summaries.GetCount();
		case PHASE_ELEMENT_SCORES:			return snap->data.AllCategories()[batch].summaries.GetCount();
		default: TODO; return 1;
	}
}

void BiographyProcess::DoPhase() {
	switch (phase) {
		case PHASE_ELEMENTS_USING_EXISTING:			GetElementsUsingExisting(); return;
		case PHASE_ELEMENTS_SINGLE_YEAR:			ElementsForSingleYears(); return;
		case PHASE_ELEMENT_SCORES:					GetElementScores(); return;
		default: TODO; return;
	}
}

BiographyProcess& BiographyProcess::Get(Profile& p, BiographySnapshot& snap) {
	static ArrayMap<String, BiographyProcess> arr;
	
	String key = "PROFILE(" + p.name + "), REVISION(" + IntStr(snap.revision) + ")";
	BiographyProcess& ts = arr.GetAdd(key);
	ts.profile = &p;
	ts.owner = p.owner;
	ts.snap = &snap;
	return ts;
}

void BiographyProcess::GetElementsUsingExisting() {
	Biography& data = snap->data;
	BiographyAnalysis& analysis = snap->analysis;
	auto& data_categories = data.AllCategories();
	
	if (batch >= BIOCATEGORY_COUNT) {
		NextPhase();
		return;
	}
	int bcat_i = batch;
	
	BiographyCategory& bcat = data.GetAdd(*owner, bcat_i);
	
	
	if (sub_batch >= bcat.years.GetCount()) {
		NextBatch();
		return;
	}
	BioYear& by = bcat.years[sub_batch];
	
	if (by.text.IsEmpty()) {
		NextSubBatch();
		return;
	}
	
	if (skip_ready && by.elements.GetCount()) {
		NextSubBatch();
		return;
	}
	
	
	// Loop other snapshots to search for older summary with the same hash
	const String& cmp = by.text;
	bool found = false;
	for(int i = 0; i < profile->snapshots.GetCount(); i++) {
		const auto& snap = profile->snapshots[i];
		BiographyCategory& bcat1 = data.GetAdd(*owner, bcat_i);
		for(int k = 0; k < bcat1.years.GetCount(); k++) {
			const auto& by1 = bcat1.years[k];
			
			if (by1.elements.GetCount() && cmp == by1.text) {
				by.elements <<= by1.elements;
				NextSubBatch();
				return;
			}
		}
	}
	
	
	NextSubBatch();
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
	
	if (skip_ready && by.elements.GetCount()) {
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

void BiographyProcess::GetElementScores() {
	Biography& data = snap->data;
	BiographyAnalysis& analysis = snap->analysis;
	
	
	if (batch >= BIOCATEGORY_COUNT) {
		NextPhase();
		return;
	}
	ASSERT(owner);
	BiographyCategory& bcat = data.GetAdd(*owner, batch);
	
	
	if (sub_batch >= bcat.years.GetCount()) {
		NextBatch();
		return;
	}
	BioYear& by = bcat.years[sub_batch];
	
	if (by.elements.IsEmpty()) {
		NextSubBatch();
		return;
	}
	
	double av = by.GetAverageScore();
	if (skip_ready && av > 0 && av <= 10.0) {
		NextSubBatch();
		return;
	}
	
	
	BiographyProcessArgs args;
	args.fn = 2;
	args.category = GetBiographyCategoryEnum(batch);
	args.text = by.JoinElementMap(": ", "\n");
	args.year = by.year;
	TaskMgr& m = TaskMgr::Single();
	
	SetWaiting(1);
	auto* by_ptr = &by;
	m.GetBiography(args, [this, by_ptr](String result) {
		RemoveEmptyLines3(result);
		Vector<String> lines = Split(result, "\n");
		
		snap->last_modified = GetSysTime();
		
		for (String& line : lines) {
			int a = line.Find(":");
			if (a < 0) continue;
			String key = ToLower(TrimBoth(line.Left(a)));
			String value = TrimBoth(line.Mid(a+1));
			int i = by_ptr->FindElement(key);
			if (i < 0)
				continue;
			auto& el = by_ptr->elements[i];
			
			Vector<String> scores = Split(value, ",");
			int j = -1;
			el.ResetScore();
			for (String& s : scores) {
				j++;
				Vector<String> parts = Split(s, ":");
				if (parts.GetCount() != 2)
					continue;
				int sc = ScanInt(TrimLeft(parts[1]));
				el.scores[j] = sc;
			}
		}
		
		NextSubBatch();
		SetWaiting(0);
	});
}


END_TEXTLIB_NAMESPACE

