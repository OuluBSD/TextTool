#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


ArrayMap<hash_t, SocialSolver>& __SocialSolvers() {
	static ArrayMap<hash_t, SocialSolver> map;
	return map;
}


SocialSolver::SocialSolver() {
	
}

SocialSolver& SocialSolver::Get(Owner& e) {
	String t = e.name;
	hash_t h = t.GetHashValue();
	ArrayMap<hash_t, SocialSolver>& map = __SocialSolvers();
	int i = map.Find(h);
	if (i >= 0)
		return map[i];
	
	SocialSolver& ls = map.Add(h);
	ls.owner = &e;
	return ls;
}

void SocialSolver::ClearTasks() {
	for (SocialSolver& g : __SocialSolvers().GetValues())
		g.SetNotRunning();
}

void SocialSolver::RestartTasks() {
	for (SocialSolver& g : __SocialSolvers().GetValues())
		g.Start();
}

void SocialSolver::Process() {
	MetaDatabase& db = MetaDatabase::Single();
	LeadData& sd = db.lead_data;
	LeadDataAnalysis& sda = db.lead_data.a;
	//sa = &sda.GetLeadEntityAnalysis(owner->name);
	int lng_i = db.GetLanguageIndex();
	
	while (running && !Thread::IsShutdownThreads()) {
		if (waiting) {
			Sleep(10);
			continue;
		}
		
		if (phase == SS_BEGIN) {
			time_started = GetSysTime();
			NextPhase();
		}
		else if (phase == SS_AUDIENCE_PROFILE_CATEGORIES) {
			ProcessAudienceProfileCategories();
		}
		else if (phase == SS_SUMMARIZE) {
			ProcessSummarize();
		}
		else /*if (phase == LS_COUNT)*/ {
			time_stopped = GetSysTime();
			phase = SS_BEGIN;
			break;
		}
		
		
		PostProgress();
		Sleep(1);
	}
	
	running = false;
	stopped = true;
}

void SocialSolver::ProcessAudienceProfileCategories() {
	if (batch >= SOCIETYROLE_COUNT) {
		NextPhase();
		return;
	}
	const Array<RoleProfile>& profs = GetRoleProfile(batch);
	
	if (sub_batch >= profs.GetCount()) {
		NextBatch();
		return;
	}
	const RoleProfile& prof = profs[sub_batch];
	
	SocialArgs args;
	args.fn = 2;
	args.text = GetSocietyRoleDescription(batch);
	args.parts.Add(prof.name, prof.profile);
	
	SetWaiting(1);
	TaskMgr& m = TaskMgr::Single();
	m.GetSocial(args, THISBACK(OnProcessAudienceProfileCategories));
}

void SocialSolver::OnProcessAudienceProfileCategories(String res) {
	
	
	SetWaiting(0);
	NextSubBatch();
}

void SocialSolver::ProcessSummarize() {
	Biography& biography = owner->biography_detailed;
	
	if (batch >= biography.categories.GetCount()) {
		NextPhase();
		return;
	}
	
	BiographyCategory& bcat = biography.GetAdd(*owner, batch);
	bcat.RealizeSummaries();
	if (sub_batch >= bcat.summaries.GetCount()) {
		NextBatch();
		return;
	}
	
	const BiographyCategory::Range& range = bcat.summaries.GetKey(sub_batch);
	BioYear& sum = bcat.summaries[sub_batch];
	
	SocialArgs args;
	args.fn = 1;
	
	if (range.len == 2) {
		int begin = range.off;
		int end = range.off + range.len;
		ASSERT(begin < end && end - begin < 100);
		for(int i = begin; i < end; i++) {
			BioYear& by = bcat.GetAdd(i);
			String title = GetBiographyCategoryKey(batch) +
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
			OnProcessSummarize(/*args.parts.GetKey(0) + ": " +*/ args.parts[0]);
			return;
		}
	}
	else {
		int step = range.len / 2;
		int begin = range.off;
		int end = range.off + range.len;
		for(int i = begin; i < end; i+=step) {
			BiographyCategory::Range sub_range;
			sub_range.off = i;
			sub_range.len = range.len >> 1;
			int j = bcat.summaries.Find(sub_range);
			ASSERT(j >= 0);
			BioYear& by = bcat.summaries[j];
			int from = sub_range.off;
			int to = sub_range.off + sub_range.len - 1;
			String title =
				GetBiographyCategoryKey(batch) +
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
			OnProcessSummarize(/*args.parts.GetKey(0) + ": " +*/ args.parts[0]);
			return;
		}
	}
	
	SetWaiting(1);
	TaskMgr& m = TaskMgr::Single();
	m.GetSocial(args, THISBACK(OnProcessSummarize));
}

void SocialSolver::OnProcessSummarize(String res) {
	Biography& biography = owner->biography_detailed;
	BiographyCategory& bcat = biography.GetAdd(*owner, batch);
	const BiographyCategory::Range& range = bcat.summaries.GetKey(sub_batch);
	BioYear& sum = bcat.summaries[sub_batch];
	
	sum.text = TrimBoth(res);
	
	
	NextSubBatch();
	SetWaiting(0);
}


END_TEXTLIB_NAMESPACE
