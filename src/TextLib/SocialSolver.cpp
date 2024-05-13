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
		else if (phase == SS_AUDIENCE_REACTS_SUMMARY) {
			ProcessAudienceReactsSummary();
		}
		else if (phase == SS_PACK_ROLE_REACTIONS) {
			ProcessRoleReactions();
		}
		else if (phase == SS_PLATFORM_DESCRIPTIONS) {
			ProcessPlatformDescriptions();
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
	int role_i = batch;
	int prof_i = sub_batch;
	
	if (role_i >= SOCIETYROLE_COUNT) {
		NextPhase();
		return;
	}
	const Array<RoleProfile>& profs = GetRoleProfile(role_i);
	
	if (prof_i >= profs.GetCount()) {
		NextBatch();
		return;
	}
	const RoleProfile& prof = profs[prof_i];
	
	BiographyAnalysis& analysis = owner->biography_analysis;
	analysis.Realize();
	const BiographyProfileAnalysis& pa = analysis.profiles[role_i][prof_i];
	
	if (skip_ready && pa.categories.GetCount()) {
		NextSubBatch();
		return;
	}
	
	SocialArgs args;
	args.fn = 2;
	args.text = GetSocietyRoleDescription(batch);
	args.parts.Add(prof.name, prof.profile);
	
	SetWaiting(1);
	TaskMgr& m = TaskMgr::Single();
	m.GetSocial(args, THISBACK(OnProcessAudienceProfileCategories));
}

void SocialSolver::OnProcessAudienceProfileCategories(String res) {
	int role_i = batch;
	int prof_i = sub_batch;
	const Array<RoleProfile>& profs = GetRoleProfile(role_i);
	const RoleProfile& prof = profs[prof_i];
	BiographyAnalysis& analysis = owner->biography_analysis;
	analysis.Realize();
	BiographyProfileAnalysis& pa = analysis.profiles[role_i][prof_i];
	
	pa.categories.Clear();
	
	res = "1. Category " + res;
	Vector<String> lines = Split(res, "\n");
	if (lines.GetCount() == 20) {
		Vector<int> rm_list;
		for(int i = 0; i < 10; i++) {
			lines[i*2] += lines[i*2+1];
			rm_list << i+1;
		}
		lines.Remove(rm_list);
	}
	for(int i = 0; i < lines.GetCount(); i++) {
		String& l = lines[i];
		RemoveLineNumber(l);
		l = TrimBoth(l);
		if (l.IsEmpty() || l[0] == '-')
			lines.Remove(i--);
		
		int a = l.Find("ategory ");
		if (a < 0) {
			// Category number was not given: try to find the category by comparing text
			if (l.Left(2) == ": ")
				l = l.Mid(2);
			int b = l.Find("-");
			String key = ToLower(TrimBoth(l.Left(b)));
			int key_i = -1;
			for(int j = 0; j < BIOCATEGORY_COUNT; j++) {
				String cmp_key = ToLower(GetBiographyCategoryKey(j));
				if (key == cmp_key) {
					key_i = j;
					break;
				}
			}
			if (key_i >= 0) {
				a = 0;
				l = "ategory " + IntStr(key_i) + ": " + l;
			}
			// Category was not found
			else continue;
		}
		a += 8;
		l = l.Mid(a);
		
		int cat_num = ScanInt(l);
		
		a = l.Find("-");
		if (a >= 0) {
			String desc = TrimBoth(l.Mid(a+1));
			desc.Replace("person #1", "the subject person");
			desc.Replace("Person #1", "The subject person");
			desc.Replace("person #2", "this person");
			desc.Replace("Person #2", "This person");
			pa.categories.Add(cat_num, desc);
		}
		else {
			pa.categories.Add(cat_num);
		}
	}
	
	
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
	
	if (skip_ready && sum.text.GetCount()) {
		NextSubBatch();
		return;
	}
	
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
			OnProcessSummarize("(" + args.parts.GetKey(0) + ") " + args.parts[0]);
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
			OnProcessSummarize("(" + args.parts.GetKey(0) + ") " + args.parts[0]);
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

void SocialSolver::ProcessAudienceReactsSummary() {
	Biography& biography = owner->biography_detailed;
	
	if (batch == 0 && sub_batch == 0) {
		BiographyAnalysis& analysis = owner->biography_analysis;
		analysis.Realize();
		ptrs.Clear();
		role_descs.Clear();
		prof_ptrs.Clear();
		for(int i = 0; i < analysis.profiles.GetCount(); i++) {
			const auto& profs = GetRoleProfile(i);
			auto& profiles = analysis.profiles[i];
			int c = min(profiles.GetCount(), profs.GetCount());
			for(int j = 0; j < c; j++) {
				ptrs << &profiles[j];
				prof_ptrs << &profs[j];
				role_descs << GetSocietyRoleDescription(i);
			}
		}
	}
	
	if (batch >= ptrs.GetCount()) {
		NextPhase();
		return;
	}
	
	BiographyProfileAnalysis& pa = *ptrs[batch];
	const RoleProfile& rp = *prof_ptrs[batch];
	if (skip_ready && pa.biography_reaction.GetCount()) {
		NextBatch();
		return;
	}
	
	SocialArgs args;
	args.fn = 3;
	args.text = role_descs[batch];
	
	args.parts.Add(rp.name, rp.profile);
	
	int cat_count = min(pa.categories.GetCount(), 10);
	int total_length = 0;
	for(int i = 0; i < cat_count; i++) {
		int priority;
		switch (i) {
			case 0:
				priority = 2; break;
			case 1:
			case 2:
			case 3:
			case 4:
				priority = 1; break;
			default:
				priority = 0; break;
		}
		int bcat_i = pa.categories.GetKey(i);
		BiographyCategory& bcat = biography.GetAdd(*owner, bcat_i);
		
		int top_len = 0;
		for(int j = 0; j < bcat.summaries.GetCount(); j++)
			top_len = max(top_len, bcat.summaries.GetKey(j).len);
		
		int tgt_len = max(2, top_len >> priority);
		for(int j = 0; j < bcat.summaries.GetCount(); j++) {
			const auto& key = bcat.summaries.GetKey(j);
			if (key.len == tgt_len) {
				BioYear& by = bcat.summaries[j];
				if (by.text.GetCount()) {
					String title =
						GetBiographyCategoryKey(bcat_i) + ", years " +
						IntStr(key.off) + "-" + IntStr(key.off+key.len-1);
					args.parts.Add(title, by.text + "\n");
					total_length += title.GetCount() + by.text.GetCount() + 2;
					if (total_length >= 10000) break; // limit length because GPT prompt limits
				}
			}
		}
		if (total_length >= 10000) break; // limit length because GPT prompt limits
	}
	
	SetWaiting(1);
	TaskMgr& m = TaskMgr::Single();
	m.GetSocial(args, THISBACK(OnProcessAudienceReactsSummary));
}

void SocialSolver::OnProcessAudienceReactsSummary(String res) {
	BiographyProfileAnalysis& pa = *ptrs[batch];
	
	pa.biography_reaction = TrimBoth(res);
	
	NextBatch();
	SetWaiting(0);
}

void SocialSolver::ProcessRoleReactions() {
	Biography& biography = owner->biography_detailed;
	BiographyAnalysis& analysis = owner->biography_analysis;
	int role_i = batch;
	
	if (role_i >= analysis.profiles.GetCount()) {
		NextPhase();
		return;
	}
	
	if (role_i >= analysis.roles.GetCount())
		analysis.roles.SetCount(role_i+1);
	
	BiographyRoleAnalysis& role = analysis.roles[role_i];
	if (skip_ready && role.merged_biography_reaction.GetCount()) {
		NextBatch();
		return;
	}
	
	SocialArgs args;
	args.fn = 4;
	
	const auto& profs = GetRoleProfile(role_i);
	auto& profiles = analysis.profiles[role_i];
	int c = min(profiles.GetCount(), profs.GetCount());
	for(int j = 0; j < c; j++) {
		BiographyProfileAnalysis& pa = profiles[j];
		args.parts.Add(profs[j].name, pa.biography_reaction);
	}
	
	SetWaiting(1);
	TaskMgr& m = TaskMgr::Single();
	m.GetSocial(args, THISBACK(OnProcessRoleReactions));
}

void SocialSolver::OnProcessRoleReactions(String res) {
	Biography& biography = owner->biography_detailed;
	BiographyAnalysis& analysis = owner->biography_analysis;
	int role_i = batch;
	
	if (role_i >= analysis.roles.GetCount()) {
		analysis.roles.SetCount(role_i+1);
	}
	BiographyRoleAnalysis& role = analysis.roles[role_i];
	role.merged_biography_reaction = TrimBoth(res);
	if (role.merged_biography_reaction.Left(1) == "\"") role.merged_biography_reaction = role.merged_biography_reaction.Mid(1);
	if (role.merged_biography_reaction.Right(1) == "\"") role.merged_biography_reaction = role.merged_biography_reaction.Left(role.merged_biography_reaction.GetCount()-1);
	
	NextBatch();
	SetWaiting(0);
}

void SocialSolver::ProcessPlatformDescriptions() {
	BiographyAnalysis& analysis = owner->biography_analysis;
	
	if (batch >= PLATFORM_COUNT) {
		NextPhase();
		return;
	}
	
	NextPhase(); return;
	
	const Platform& plat = GetPlatforms()[batch];
	ASSERT(plat.roles.GetCount());
	
	SocialArgs args;
	args.fn = 5;
	
	for(int i = 0; i < plat.roles.GetCount(); i++) {
		int role_i = plat.roles[i];
		const BiographyRoleAnalysis& role = analysis.roles[role_i];
		args.parts.Add(GetSocietyRoleDescription(role_i), role.merged_biography_reaction);
	}
	
	SetWaiting(1);
	TaskMgr& m = TaskMgr::Single();
	m.GetSocial(args, THISBACK(OnProcessPlatformDescriptions));
}

void SocialSolver::OnProcessPlatformDescriptions(String res) {
	
	
	
	NextBatch();
	SetWaiting(0);
}

END_TEXTLIB_NAMESPACE
