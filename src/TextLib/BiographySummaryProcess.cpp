#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


BiographySummaryProcess::BiographySummaryProcess() {
	
}

int BiographySummaryProcess::GetPhaseCount() const {
	return PHASE_COUNT;
}

int BiographySummaryProcess::GetBatchCount(int phase) const {
	switch (phase) {
		case PHASE_FIX_SUMMARY_HASHES:					return 1;
		case PHASE_SUMMARIZE_USING_EXISTING:			return BIOCATEGORY_COUNT;
		case PHASE_SUMMARIZE:							return BIOCATEGORY_COUNT;
		case PHASE_SUMMARIZE_ELEMENTS_USING_EXISTING:	return BIOCATEGORY_COUNT;
		case PHASE_SUMMARIZE_ELEMENTS:					return BIOCATEGORY_COUNT;
		default: return 1;
	}
}

int BiographySummaryProcess::GetSubBatchCount(int phase, int batch) const {
	int bcat_i = batch;
	BiographyCategory& bcat = snap->data.GetAdd(*owner, bcat_i);
	bcat.RealizeSummaries();
	switch (phase) {
		case PHASE_FIX_SUMMARY_HASHES:					return 1;
		case PHASE_SUMMARIZE_USING_EXISTING:			return bcat.summaries.GetCount();
		case PHASE_SUMMARIZE:							return bcat.summaries.GetCount();
		case PHASE_SUMMARIZE_ELEMENTS_USING_EXISTING:	return bcat.summaries.GetCount();
		case PHASE_SUMMARIZE_ELEMENTS:					return bcat.summaries.GetCount();
		default: return 1;
	}
}

void BiographySummaryProcess::DoPhase() {
	switch (phase) {
		case PHASE_FIX_SUMMARY_HASHES:					FixSummaryHashes(); return;
		case PHASE_SUMMARIZE_USING_EXISTING:			SummarizeUsingExisting(); return;
		case PHASE_SUMMARIZE:							Summarize(); return;
		case PHASE_SUMMARIZE_ELEMENTS_USING_EXISTING:	SummarizeElementsUsingExisting(); return;
		case PHASE_SUMMARIZE_ELEMENTS:					SummarizeElements(); return;
		default: return;
	}
}

BiographySummaryProcess& BiographySummaryProcess::Get(Profile& p, BiographySnapshot& snap) {
	static ArrayMap<String, BiographySummaryProcess> arr;
	
	String key = "PROFILE(" + p.name + "), REVISION(" + IntStr(snap.revision) + ")";
	BiographySummaryProcess& ts = arr.GetAdd(key);
	ts.owner = p.owner;
	ts.profile = &p;
	ts.snap = &snap;
	ASSERT(ts.owner);
	return ts;
}

void BiographySummaryProcess::FixSummaryHashes() {
	// no latest snapsphot
	for(int i = 0; i < profile->snapshots.GetCount()-1; i++) {
		auto& snap = profile->snapshots[i];
		
		for(int j = 0; j < snap.data.categories.GetCount(); j++) {
			auto& bcat = snap.data.categories[j];
			
			for(int k = 0; k < bcat.summaries.GetCount(); k++) {
				const BioRange& range = bcat.summaries.GetKey(k);
				auto& sum = bcat.summaries[k];
				if (sum.source_hash != 0)
					continue;
				CombineHash ch;
				if (range.len == 2) {
					int begin = range.off;
					int end = range.off + range.len;
					ASSERT(begin < end && end - begin < 100);
					for(int i = begin; i < end; i++) {
						BioYear& by = bcat.GetAdd(i);
						ch.Do(by.text);
					}
				}
				else {
					int step = range.len / 2;
					int begin = range.off;
					int end = range.off + range.len;
					for(int i = begin; i < end; i+=step) {
						BioRange sub_range;
						sub_range.off = i;
						sub_range.len = range.len >> 1;
						int j = bcat.summaries.Find(sub_range);
						ASSERT(j >= 0);
						BioYear& by = bcat.summaries[j];
						ch.Do(by.text);
					}
				}
				sum.source_hash = ch;
			}
		}
	}
	
	NextPhase();
}

void BiographySummaryProcess::SummarizeUsingExisting() {
	Biography& biography = snap->data;
	
	// Source data hash must be updated in earlier phase, and it won't be done to the latest
	ASSERT(&biography != &profile->snapshots.Top().data);
	
	if (batch >= BIOCATEGORY_COUNT) {
		NextPhase();
		return;
	}
	int bcat_i = batch;
	
	BiographyCategory& bcat = biography.GetAdd(*owner, bcat_i);
	bcat.RealizeSummaries();
	if (sub_batch >= bcat.summaries.GetCount()) {
		NextBatch();
		return;
	}
	
	const BioRange& range = bcat.summaries.GetKey(sub_batch);
	BioYear& sum = bcat.summaries[sub_batch];
	
	if (skip_ready && sum.text.GetCount()) {
		NextSubBatch();
		return;
	}
	
	// Apparently there is no input text
	if (sum.source_hash == 0) {
		NextSubBatch();
		return;
	}
	
	
	// Loop other snapshots to search for older summary with the same hash
	hash_t cmp = sum.source_hash;
	bool found = false;
	for(int i = 0; i < profile->snapshots.GetCount(); i++) {
		const auto& snap = profile->snapshots[i];
		for(int j = 0; j < snap.data.categories.GetCount(); j++) {
			const auto& cat = snap.data.categories[j];
			for(int k = 0; k < cat.summaries.GetCount(); k++) {
				const auto& by = cat.summaries[k];
				
				if (cmp == by.source_hash && by.text.GetCount()) {
					sum.text = by.text;
					NextSubBatch();
					return;
				}
			}
		}
	}
	
	
	NextSubBatch();
}

bool BiographySummaryProcess::SummarizeBase(int fn, BiographySummaryProcessArgs& args) {
	Biography& biography = snap->data;
	
	if (batch >= BIOCATEGORY_COUNT) {
		NextPhase();
		return false;
	}
	int bcat_i = batch;
	
	BiographyCategory& bcat = biography.GetAdd(*owner, bcat_i);
	bcat.RealizeSummaries();
	if (sub_batch >= bcat.summaries.GetCount()) {
		NextBatch();
		return false;
	}
	
	const BioRange& range = bcat.summaries.GetKey(sub_batch);
	BioYear& sum = bcat.summaries[sub_batch];
	
	if (fn == 0 && skip_ready && sum.text.GetCount()) {
		NextSubBatch();
		return false;
	}
	else if (fn == 1 && skip_ready && sum.elements.GetCount()) {
		NextSubBatch();
		return false;
	}
	
	args.fn = fn;
	
	if (range.len == 2) {
		int begin = range.off;
		int end = range.off + range.len;
		ASSERT(begin < end && end - begin < 100);
		for(int i = begin; i < end; i++) {
			BioYear& by = bcat.GetAdd(i);
			String title = GetBiographyCategoryKey(bcat_i) +
				", year " + IntStr(by.year) +
				", age " + IntStr(by.year - owner->year_of_birth);
			if (fn == 0 && by.text.GetCount())
				args.parts.Add(title, by.text);
			else if (fn == 1 && by.elements.GetCount())
				args.parts.Add(title, JoinMap(by.elements, ": ", "\n"));
		}
		if (args.parts.IsEmpty()) {
			NextSubBatch();
			return false;
		}
		else if (args.parts.GetCount() == 1) {
			if (fn == 0)
				OnProcessSummarize("(" + args.parts.GetKey(0) + ") " + args.parts[0]);
			else if (fn == 1)
				OnProcessSummarizeElements(args.parts[0]);
			return false;
		}
	}
	else {
		int step = range.len / 2;
		int begin = range.off;
		int end = range.off + range.len;
		for(int i = begin; i < end; i+=step) {
			BioRange sub_range;
			sub_range.off = i;
			sub_range.len = range.len >> 1;
			int j = bcat.summaries.Find(sub_range);
			ASSERT(j >= 0);
			BioYear& by = bcat.summaries[j];
			int from = sub_range.off;
			int to = sub_range.off + sub_range.len - 1;
			String title =
				GetBiographyCategoryKey(bcat_i) +
				", from year " + IntStr(from) +
				" to year " + IntStr(to) +
				", age " + IntStr(from - owner->year_of_birth) + " - " + IntStr(to - owner->year_of_birth)
				;
			if (fn == 0 && by.text.GetCount())
				args.parts.Add(title, by.text);
			else if (fn == 1 && by.elements.GetCount())
				args.parts.Add(title, JoinMap(by.elements, ": ", "\n"));
		}
		if (args.parts.IsEmpty()) {
			NextSubBatch();
			return false;
		}
		else if (args.parts.GetCount() == 1) {
			if (fn == 0)
				OnProcessSummarize("(" + args.parts.GetKey(0) + ") " + args.parts[0]);
			else if (fn == 1)
				OnProcessSummarizeElements(args.parts[0]);
			return false;
		}
	}
	return true;
}


void BiographySummaryProcess::Summarize() {
	BiographySummaryProcessArgs args; // 0
	
	if (!SummarizeBase(0, args))
		return;
	
	SetWaiting(1);
	TaskMgr& m = TaskMgr::Single();
	m.GetBiographySummary(args, THISBACK(OnProcessSummarize));
}

void BiographySummaryProcess::OnProcessSummarize(String res) {
	Biography& biography = snap->data;
	
	int bcat_i = batch;
	BiographyCategory& bcat = biography.GetAdd(*owner, bcat_i);
	const BioRange& range = bcat.summaries.GetKey(sub_batch);
	BioYear& sum = bcat.summaries[sub_batch];
	
	sum.text = TrimBoth(res);
	
	NextSubBatch();
	SetWaiting(0);
}

void BiographySummaryProcess::SummarizeElementsUsingExisting() {
	Biography& biography = snap->data;
	
	// Source data hash must be updated in earlier phase, and it won't be done to the latest
	ASSERT(&biography != &profile->snapshots.Top().data);
	
	if (batch >= BIOCATEGORY_COUNT) {
		NextPhase();
		return;
	}
	int bcat_i = batch;
	
	BiographyCategory& bcat = biography.GetAdd(*owner, bcat_i);
	bcat.RealizeSummaries();
	if (sub_batch >= bcat.summaries.GetCount()) {
		NextBatch();
		return;
	}
	
	const BioRange& range = bcat.summaries.GetKey(sub_batch);
	BioYear& sum = bcat.summaries[sub_batch];
	
	if (skip_ready && sum.text.GetCount()) {
		NextSubBatch();
		return;
	}
	
	// Apparently there is no input text
	if (sum.source_hash == 0) {
		NextSubBatch();
		return;
	}
	
	
	// Loop other snapshots to search for older summary with the same hash
	hash_t cmp = sum.source_hash;
	bool found = false;
	for(int i = 0; i < profile->snapshots.GetCount(); i++) {
		const auto& snap = profile->snapshots[i];
		for(int j = 0; j < snap.data.categories.GetCount(); j++) {
			const auto& cat = snap.data.categories[j];
			for(int k = 0; k < cat.summaries.GetCount(); k++) {
				const auto& by = cat.summaries[k];
				
				if (cmp == by.source_hash && by.elements.GetCount()) {
					sum.elements <<= by.elements;
					NextSubBatch();
					return;
				}
			}
		}
	}
	
	
	NextSubBatch();
}

void BiographySummaryProcess::SummarizeElements() {
	BiographySummaryProcessArgs args; // 1
	
	if (!SummarizeBase(1, args))
		return;
	
	SetWaiting(1);
	TaskMgr& m = TaskMgr::Single();
	m.GetBiographySummary(args, THISBACK(OnProcessSummarizeElements));
}

void BiographySummaryProcess::OnProcessSummarizeElements(String result) {
	Biography& biography = snap->data;
	
	int bcat_i = batch;
	BiographyCategory& bcat = biography.GetAdd(*owner, bcat_i);
	const BioRange& range = bcat.summaries.GetKey(sub_batch);
	BioYear& sum = bcat.summaries[sub_batch];
	
	RemoveEmptyLines3(result);
	Vector<String> lines = Split(result, "\n");
	
	sum.elements.Clear();
	for (String& line : lines) {
		int a = line.Find(":");
		if (a < 0) continue;
		String key = ToLower(TrimBoth(line.Left(a)));
		String value = TrimBoth(line.Mid(a+1));
		RemoveQuotes(value);
		String lvalue = ToLower(value);
		if (lvalue.IsEmpty() || lvalue == "none." || lvalue == "none" || lvalue.Left(6) == "none (" || lvalue == "ready." || lvalue == "ready" || lvalue.Left(6) == "ready (" || lvalue == "n/a" || lvalue == "n/a." || lvalue == "n/a. n/a." ||  lvalue == "n/a; n/a" || lvalue == "n/a, n/a" || lvalue == "none mentioned.")
			continue;
		if (lvalue.Right(5) == " n/a.")
			value = value.Left(value.GetCount() - 5);
		if (lvalue.Left(4) == "n/a.")
			value = TrimBoth(value.Mid(4));
		else if (lvalue.Left(4) == "n/a,")
			value = TrimBoth(value.Mid(4));
		else if (lvalue.Left(3) == "n/a")
			value = TrimBoth(value.Mid(3));
		sum.elements.GetAdd(key) = value;
	}
	
	NextSubBatch();
	SetWaiting(0);
}


END_TEXTLIB_NAMESPACE

