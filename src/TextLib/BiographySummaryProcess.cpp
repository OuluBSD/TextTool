#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


BiographySummaryProcess::BiographySummaryProcess() {
	
}

int BiographySummaryProcess::GetPhaseCount() const {
	return PHASE_COUNT;
}

int BiographySummaryProcess::GetBatchCount(int phase) const {
	switch (phase) {
		case PHASE_FIX_SUMMARY_HASHES:			return 1;
		case PHASE_SUMMARIZE:					return BIOCATEGORY_COUNT;
		case PHASE_SUMMARIZE_USING_EXISTING:	return BIOCATEGORY_COUNT;
		default: TODO; return 1;
	}
}

int BiographySummaryProcess::GetSubBatchCount(int phase, int batch) const {
	int bcat_i = batch;
	BiographyCategory& bcat = snap->data.GetAdd(*owner, bcat_i);
	bcat.RealizeSummaries();
	switch (phase) {
		case PHASE_FIX_SUMMARY_HASHES:			return 1;
		case PHASE_SUMMARIZE:					return bcat.summaries.GetCount();
		case PHASE_SUMMARIZE_USING_EXISTING:	return bcat.summaries.GetCount();
		default: TODO; return 1;
	}
}

void BiographySummaryProcess::DoPhase() {
	switch (phase) {
		case PHASE_FIX_SUMMARY_HASHES:			FixSummaryHashes(); return;
		case PHASE_SUMMARIZE_USING_EXISTING:	SummarizeUsingExisting(); return;
		case PHASE_SUMMARIZE:					Summarize(); return;
		default: TODO; return;
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

void BiographySummaryProcess::Summarize() {
	Biography& biography = snap->data;
	
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
	
	BiographySummaryProcessArgs args; // 0
	args.fn = 0; // old SocialSolver 1`
	
	if (range.len == 2) {
		int begin = range.off;
		int end = range.off + range.len;
		ASSERT(begin < end && end - begin < 100);
		for(int i = begin; i < end; i++) {
			BioYear& by = bcat.GetAdd(i);
			String title = GetBiographyCategoryKey(bcat_i) +
				", year " + IntStr(by.year) +
				", age " + IntStr(by.year - owner->year_of_birth);
			if (by.text.GetCount())
				args.parts.Add(title, by.text);
		}
		if (args.parts.IsEmpty()) {
			NextSubBatch();
			return;
		}
		else if (args.parts.GetCount() == 1) {
			OnProcessSummarize("(" + args.parts.GetKey(0) + ") " + args.parts[0]);
			return;
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
			if (by.text.GetCount())
				args.parts.Add(title, by.text);
		}
		if (args.parts.IsEmpty()) {
			NextSubBatch();
			return;
		}
		else if (args.parts.GetCount() == 1) {
			OnProcessSummarize("(" + args.parts.GetKey(0) + ") " + args.parts[0]);
			return;
		}
	}
	
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


END_TEXTLIB_NAMESPACE

