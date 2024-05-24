#include "TextLib.h"
#include <TextCtrl/TextCtrl.h>


BEGIN_TEXTLIB_NAMESPACE


ArrayMap<hash_t, SocialSolver>& __SocialSolvers() {
	static ArrayMap<hash_t, SocialSolver> map;
	return map;
}


SocialSolver::SocialSolver() {
	
}

SocialSolver& SocialSolver::Get(Profile& e) {
	String t = e.owner->name + ": " + e.name;
	hash_t h = t.GetHashValue();
	ArrayMap<hash_t, SocialSolver>& map = __SocialSolvers();
	int i = map.Find(h);
	if (i >= 0)
		return map[i];
	
	SocialSolver& ls = map.Add(h);
	ASSERT(e.owner);
	ls.owner = e.owner;
	ls.profile = &e;
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
		else if (phase > SS_AUDIENCE_PROFILE_CATEGORIES && only_categories) {
			only_categories = false;
			phase = SS_COUNT;
		}
		else if (phase == SS_ANALYZE_ROLE_SCORES) {
			ProcessAnalyzeRoleScores();
		}
		else if (phase == SS_ANALYZE_PLATFORM_ROLES) {
			ProcessAnalyzePlatformRoles();
		}
		else if (phase == SS_ANALYZE_PLATFORM_EPK_TEXT_FIELDS) {
			ProcessAnalyzePlatformEpkTextFields();
		}
		else if (phase == SS_ANALYZE_PLATFORM_EPK_PHOTO_TYPES) {
			ProcessAnalyzePlatformEpkPhotoTypes();
		}
		else if (phase == SS_ANALYZE_PLATFORM_EPK_PHOTO_AI_PROMPTS) {
			ProcessAnalyzePlatformEpkPhotoAiPrompts();
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
		else if (phase == SS_PACK_PLATFORM_REACTIONS) {
			ProcessPlatformReactions();
		}
		else if (phase == SS_PLATFORM_DESCRIPTIONS) {
			ProcessPlatformDescriptions();
		}
		else if (phase == SS_PLATFORM_DESCRIPTION_REFINEMENTS) {
			ProcessPlatformDescriptionRefinements();
		}
		else if (phase == SS_PLATFORM_DESCRIPTION_TRANSLATED) {
			ProcessPlatformDescriptionTranslated();
		}
		else if (phase == SS_MERGE_MESSAGES) {
			ProcessMergeMessages();
		}
		else if (phase == SS_ANALYZE_IMAGE_BIOGRAPHY) {
			ProcessAnalyzeImageBiography();
		}
		else if (phase == SS_SUMMARIZE_IMAGE_CATEGORY_YEAR) {
			ProcessSummarizeImageCategoryYear();
		}
		else if (phase == SS_SUMMARIZE_IMAGE_BIOGRAPHY) {
			ProcessSummarizeImageBiography();
		}
		else /*if (phase == SS_COUNT)*/ {
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

void SocialSolver::ProcessAnalyzeRoleScores() {
	if (batch >= SOCIETYROLE_COUNT) {
		NextPhase();
		return;
	}
	
	const SocietyRoleAnalysis& sra = MetaDatabase::Single().GetAddRole(batch);
	
	if (skip_ready && sra.GetScoreSum() > 0) {
		NextBatch();
		return;
	}
	
	SocialArgs args;
	args.fn = 13;
	args.text = GetSocietyRoleKey(batch);
	args.description = GetSocietyRoleDescription(batch);
	
	SetWaiting(1);
	TaskMgr& m = TaskMgr::Single();
	m.GetSocial(args, THISBACK(OnProcessAnalyzeRoleScores));
}

void SocialSolver::OnProcessAnalyzeRoleScores(String res) {
	SocietyRoleAnalysis& sra = MetaDatabase::Single().GetAddRole(batch);
	
	memset(sra.scores, 0, sizeof(sra.scores));
	
	if (res.Left(2) != "1.")
		res = "1. " + res;
	
	Vector<String> lines = Split(res, "\n");
	for (String& l : lines) {
		Vector<String> parts;
		if (l.Find(":") >= 0)
			parts = Split(l, ":");
		else
			parts = Split(l, ".");
		
		if (parts.GetCount() < 2)
			continue;
		for (String& s : parts) s = TrimBoth(s);
		
		int score_i = ScanInt(parts[0]) - 1;
		int score = max(0, min(10, ScanInt(parts[1])));
		
		if (score_i >= 0 && score_i < SOCIETYROLE_SCORE_COUNT)
			sra.scores[score_i] = score;
	}
	
	SetWaiting(0);
	NextBatch();
}

void SocialSolver::ProcessAnalyzePlatformRoles() {
	if (batch >= PLATFORM_COUNT) {
		NextPhase();
		return;
	}
	
	const Platform& plat = GetPlatforms()[batch];
	const PlatformAnalysis& pa = MetaDatabase::Single().GetAdd(plat);
	
	if (skip_ready && pa.roles.GetCount()) {
		NextBatch();
		return;
	}
	
	SocialArgs args;
	args.fn = 11;
	for(int i = 0; i < SOCIETYROLE_COUNT; i++)
		args.parts.Add(GetSocietyRoleKey(i));
	args.text = plat.name;
	args.description = plat.description;
	
	SetWaiting(1);
	TaskMgr& m = TaskMgr::Single();
	m.GetSocial(args, THISBACK(OnProcessAnalyzePlatformRoles));
}

void SocialSolver::OnProcessAnalyzePlatformRoles(String res) {
	const Platform& plat = GetPlatforms()[batch];
	PlatformAnalysis& pa = MetaDatabase::Single().GetAdd(plat);
	
	res = "1. #" + res;
	
	RemoveEmptyLines2(res);
	
	pa.roles.Clear();
	Vector<String> lines = Split(res, "\n");
	for (String& l : lines) {
		int a = l.Find("#");
		if (a >= 0) {
			a++;
			int role_i = ScanInt(l.Mid(a)); // 0 is first
			if (role_i >= 0 && role_i < SOCIETYROLE_COUNT)
				pa.roles << role_i;
		}
	}
	
	SetWaiting(0);
	NextBatch();
}

void SocialSolver::ProcessAudienceProfileCategories() {
	BiographyAnalysis& analysis = profile->biography_analysis;
	int role_i = batch;
	int prof_i = sub_batch;
	
	if (role_i >= SOCIETYROLE_COUNT) {
		NextPhase();
		return;
	}
	
	analysis.Realize();
	if (analysis.GetRequiredRoles().Find(role_i) < 0) {
		NextBatch();
		return;
	}
	
	const Array<RoleProfile>& profs = GetRoleProfile(role_i);
	
	if (prof_i >= profs.GetCount()) {
		NextBatch();
		return;
	}
	const RoleProfile& prof = profs[prof_i];
	
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
	BiographyAnalysis& analysis = profile->biography_analysis;
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

void SocialSolver::ProcessAnalyzePlatformEpkTextFields() {
	
	if (batch >= PLATFORM_COUNT) {
		NextPhase();
		return;
	}
	
	const Platform& plat = GetPlatforms()[batch];
	const PlatformAnalysis& pa = MetaDatabase::Single().GetAdd(plat);
	
	if (skip_ready && pa.epk_text_fields.GetCount()) {
		NextBatch();
		return;
	}
	
	SocialArgs args;
	args.fn = 12;
	args.text = plat.name;
	args.description = plat.description;
	for(int i = 0; i < pa.roles.GetCount(); i++)
		args.parts.Add(GetSocietyRoleKey(pa.roles[i]));
	
	if (plat.profile_type == PLATFORM_PROFILE_ANY)
		args.profile = "any type of real person or artist profile";
	else if (plat.profile_type == PLATFORM_PROFILE_MUSIC_ARTIST)
		args.profile = "an music artist";
	else if (plat.profile_type == PLATFORM_PROFILE_VISUAL_ARTIST)
		args.profile = "an visual artist";
	else if (plat.profile_type == PLATFORM_PROFILE_PHOTOGRAPHER)
		args.profile = "a photographer";
	else if (plat.profile_type == PLATFORM_PROFILE_REAL_PERSON)
		args.profile = "a real person";
	else
		args.profile = GetPlatformProfileKey(plat.profile_type);
	
	SetWaiting(1);
	TaskMgr& m = TaskMgr::Single();
	m.GetSocial(args, THISBACK(OnProcessAnalyzePlatformEpkTextFields));
}

void SocialSolver::OnProcessAnalyzePlatformEpkTextFields(String res) {
	const Platform& plat = GetPlatforms()[batch];
	PlatformAnalysis& pa = MetaDatabase::Single().GetAdd(plat);
	
	if (res.Left(2) != "1.")
		res = "1. " + res;
	
	RemoveEmptyLines2(res);
	
	res.Replace("Name: ", "");
	res.Replace("name: ", "");
	res.Replace("\nDescription: ", ":");
	res.Replace("\ndescription: ", ":");
	res.Replace("Description: ", ":");
	res.Replace("description: ", ":");
	res.Replace("\n\n","\n");
	//LOG(res);
	
	
	pa.epk_text_fields.Clear();
	Vector<String> lines = Split(res, "\n");
	for (String& l : lines) {
		l = TrimBoth(l);
		if (l.IsEmpty()) continue;
		String key, value;
		int a = l.Find(":");
		if (a == -1)
			a = l.Find("-");
		if (a >= 0) {
			key = TrimBoth(l.Left(a));
			value = TrimBoth(l.Mid(a+1));
			if (value.Left(1) == ":") value = TrimBoth(value.Mid(1));
			value = Capitalize(value);
		}
		else {
			key = l;
		}
		pa.epk_text_fields.Add(key, value);
	}
	
	SetWaiting(0);
	NextBatch();
}

void SocialSolver::ProcessAnalyzePlatformEpkPhotoTypes() {
	
	if (batch >= PLATFORM_COUNT) {
		NextPhase();
		return;
	}
	
	const Platform& plat = GetPlatforms()[batch];
	const PlatformAnalysis& pa = MetaDatabase::Single().GetAdd(plat);
	
	if (skip_ready && pa.epk_photos.GetCount()) {
		NextBatch();
		return;
	}
	
	SocialArgs args;
	args.fn = 14;
	args.text = plat.name;
	args.description = plat.description;
	for(int i = 0; i < pa.roles.GetCount(); i++)
		args.parts.Add(GetSocietyRoleKey(pa.roles[i]));
	
	if (plat.profile_type == PLATFORM_PROFILE_ANY)
		args.profile = "any type of real person or artist profile";
	else if (plat.profile_type == PLATFORM_PROFILE_MUSIC_ARTIST)
		args.profile = "an music artist";
	else if (plat.profile_type == PLATFORM_PROFILE_VISUAL_ARTIST)
		args.profile = "an visual artist";
	else if (plat.profile_type == PLATFORM_PROFILE_PHOTOGRAPHER)
		args.profile = "a photographer";
	else if (plat.profile_type == PLATFORM_PROFILE_REAL_PERSON)
		args.profile = "a real person";
	else
		args.profile = GetPlatformProfileKey(plat.profile_type);
	
	SetWaiting(1);
	TaskMgr& m = TaskMgr::Single();
	m.GetSocial(args, THISBACK(OnProcessAnalyzePlatformEpkPhotoTypes));
}

void SocialSolver::OnProcessAnalyzePlatformEpkPhotoTypes(String res) {
	const Platform& plat = GetPlatforms()[batch];
	PlatformAnalysis& pa = MetaDatabase::Single().GetAdd(plat);
	
	if (res.Left(2) != "1.")
		res = "1. " + res;
	
	RemoveEmptyLines2(res);
	
	res.Replace("Name: ", "");
	res.Replace("name: ", "");
	res.Replace("\nDescription: ", ":");
	res.Replace("\ndescription: ", ":");
	res.Replace("Description: ", ":");
	res.Replace("description: ", ":");
	res.Replace("\n\n","\n");
	//LOG(res);
	
	
	pa.epk_photos.Clear();
	Vector<String> lines = Split(res, "\n");
	for (String& l : lines) {
		l = TrimBoth(l);
		if (l.IsEmpty()) continue;
		String key, value;
		int a = l.Find(":");
		if (a == -1)
			a = l.Find("-");
		if (a >= 0) {
			key = TrimBoth(l.Left(a));
			value = TrimBoth(l.Mid(a+1));
			if (value.Left(1) == ":") value = TrimBoth(value.Mid(1));
			value = Capitalize(value);
		}
		else {
			key = l;
		}
		RemoveQuotes(key);
		pa.epk_photos.Add(key).description = value;
	}
	
	SetWaiting(0);
	NextBatch();
}

void SocialSolver::ProcessAnalyzePlatformEpkPhotoAiPrompts() {
	
	if (batch >= PLATFORM_COUNT) {
		NextPhase();
		return;
	}
	
	const Platform& plat = GetPlatforms()[batch];
	const PlatformAnalysis& pa = MetaDatabase::Single().GetAdd(plat);
	if (sub_batch >= pa.epk_photos.GetCount()) {
		NextBatch();
		return;
	}
	
	const PlatformAnalysisPhoto& pap = pa.epk_photos[sub_batch];
	/*if (skip_ready && pap.prompts.GetCount()) {
		NextBatch();
		return;
	}*/
	
	SocialArgs args;
	args.fn = 15;
	args.text = plat.name;
	args.description = plat.description;
	for(int i = 0; i < pa.roles.GetCount(); i++)
		args.parts.Add(GetSocietyRoleKey(pa.roles[i]));
	
	if (plat.profile_type == PLATFORM_PROFILE_ANY)
		args.profile = "any type of real person or artist profile";
	else if (plat.profile_type == PLATFORM_PROFILE_MUSIC_ARTIST)
		args.profile = "an music artist";
	else if (plat.profile_type == PLATFORM_PROFILE_VISUAL_ARTIST)
		args.profile = "an visual artist";
	else if (plat.profile_type == PLATFORM_PROFILE_PHOTOGRAPHER)
		args.profile = "a photographer";
	else if (plat.profile_type == PLATFORM_PROFILE_REAL_PERSON)
		args.profile = "a real person";
	else
		args.profile = GetPlatformProfileKey(plat.profile_type);
	
	args.photo_description = pa.epk_photos.GetKey(sub_batch) + ": " + pap.description;
	
	SetWaiting(1);
	TaskMgr& m = TaskMgr::Single();
	m.GetSocial(args, THISBACK(OnProcessAnalyzePlatformEpkPhotoAiPrompts));
}

void SocialSolver::OnProcessAnalyzePlatformEpkPhotoAiPrompts(String res) {
	const Platform& plat = GetPlatforms()[batch];
	PlatformAnalysis& pa = MetaDatabase::Single().GetAdd(plat);
	PlatformAnalysisPhoto& pap = pa.epk_photos[sub_batch];
	
	RemoveEmptyLines3(res);
	
	pap.prompts.Clear();
	Vector<String> lines = Split(res, "\n");
	for (String& l : lines) {
		l = TrimBoth(l);
		RemoveQuotes(l);
		pap.prompts.Add().prompt = l;
	}
	
	SetWaiting(0);
	NextSubBatch();
}

void SocialSolver::ProcessSummarize() {
	Biography& biography = profile->biography_detailed;
	
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
	
	SocialArgs args;
	args.fn = 1;
	
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
	m.GetSocial(args, THISBACK(OnProcessSummarize));
}

void SocialSolver::OnProcessSummarize(String res) {
	Biography& biography = profile->biography_detailed;
	int bcat_i = batch;
	BiographyCategory& bcat = biography.GetAdd(*owner, bcat_i);
	const BioRange& range = bcat.summaries.GetKey(sub_batch);
	BioYear& sum = bcat.summaries[sub_batch];
	
	sum.text = TrimBoth(res);
	
	
	NextSubBatch();
	SetWaiting(0);
}

void SocialSolver::ProcessAudienceReactsSummary() {
	Biography& biography = profile->biography_detailed;
	
	if (batch == 0 && sub_batch == 0) {
		BiographyAnalysis& analysis = profile->biography_analysis;
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
	Biography& biography = profile->biography_detailed;
	BiographyAnalysis& analysis = profile->biography_analysis;
	int role_i = batch;
	
	if (role_i >= analysis.profiles.GetCount()) {
		NextPhase();
		return;
	}
	
	if (analysis.GetRequiredRoles().Find(role_i) < 0) {
		NextBatch();
		return;
	}
	
	if (role_i >= analysis.roles.GetCount())
		analysis.roles.SetCount(role_i+1);
	BiographyRoleAnalysis& role = analysis.roles[role_i];
	const auto& profs = GetRoleProfile(role_i);
	auto& profiles = analysis.profiles[role_i];
	int c = min(profiles.GetCount(), profs.GetCount());
	
	if (sub_batch == 0) {
		ranges.Clear();
		CreateRange(0, c);
		//DUMPC(ranges);
	}
	if (sub_batch >= ranges.GetCount()) {
		NextBatch();
		return;
	}
	if (skip_ready && role.merged_biography_reactions.GetCount() && role.merged_biography_reactions[0].GetCount()) {
		NextBatch();
		return;
	}
	int range_i = ranges.GetCount() - 1 - sub_batch;
	
	if (role.merged_biography_reactions.GetCount() != ranges.GetCount())
		role.merged_biography_reactions.Clear();
	role.merged_biography_reactions.SetCount(ranges.GetCount());
	const String& range_output = role.merged_biography_reactions[range_i];
	
	if (skip_ready && range_output.GetCount()) {
		NextSubBatch();
		return;
	}
	
	SocialArgs args;
	args.fn = 4;
	Range& range = ranges[range_i];
	if (range.input[0] >= 0) {
		const String& r0 = role.merged_biography_reactions[range.input[0]];
		args.parts.Add("<merged reaction #1>", r0);
	}
	else {
		int begin = range.off;
		int len_2 = range.len / 2;
		int end = begin + len_2;
		for(int i = begin; i < end; i++) {
			BiographyProfileAnalysis& pa = profiles[i];
			args.parts.Add(Capitalize(profs[i].name), pa.biography_reaction);
		}
	}
	
	if (range.input[1] >= 0) {
		const String& r0 = role.merged_biography_reactions[range.input[1]];
		args.parts.Add("<merged reaction #2>", r0);
	}
	else {
		int len_2 = range.len / 2;
		int begin = range.off + len_2;
		int end = begin + range.len - len_2;
		for(int i = begin; i < end; i++) {
			BiographyProfileAnalysis& pa = profiles[i];
			args.parts.Add(Capitalize(profs[i].name), pa.biography_reaction);
		}
	}
	
	SetWaiting(1);
	TaskMgr& m = TaskMgr::Single();
	m.GetSocial(args, THISBACK(OnProcessRoleReactions));
}

void SocialSolver::OnProcessRoleReactions(String res) {
	Biography& biography = profile->biography_detailed;
	BiographyAnalysis& analysis = profile->biography_analysis;
	int role_i = batch;
	int range_i = ranges.GetCount() - 1 - sub_batch;
	
	if (role_i >= analysis.roles.GetCount())
		analysis.roles.SetCount(role_i+1);
	BiographyRoleAnalysis& role = analysis.roles[role_i];
	String& s = role.merged_biography_reactions[range_i];
	
	s = TrimBoth(res);
	if (s.Left(1) == "\"") s = s.Mid(1);
	if (s.Right(1) == "\"") s = s.Left(s.GetCount()-1);
	
	NextSubBatch();
	SetWaiting(0);
}

int SocialSolver::CreateRange(int off, int len) {
	if (len <= 1) return -1;
	int pos = ranges.GetCount();
	Range& range = ranges.Add();
	range.off = off;
	range.len = len;
	int len_2 = len / 2;
	if (len_2 == 0) return pos;
	ranges[pos].input[1] = CreateRange(off + len_2, len - len_2);
	ranges[pos].input[0] = CreateRange(off + 0, len_2);
	return pos;
}

void SocialSolver::ProcessPlatformReactions() {
	BiographyAnalysis& analysis = profile->biography_analysis;
	int plat_i = batch;
	
	if (plat_i >= PLATFORM_COUNT) {
		NextPhase();
		return;
	}
	
	const Platform& plat = GetPlatforms()[plat_i];
	const PlatformAnalysis& pa = MetaDatabase::Single().GetAdd(plat);
	ASSERT(pa.roles.GetCount());
	
	if (sub_batch == 0) {
		ranges.Clear();
		CreateRange(0, pa.roles.GetCount());
		//DUMPC(ranges);
	}
	if (sub_batch >= ranges.GetCount()) {
		NextBatch();
		return;
	}
	int range_i = ranges.GetCount() - 1 - sub_batch;
	
	if (plat_i >= analysis.platforms.GetCount())
		analysis.platforms.SetCount(plat_i+1);
	
	PlatformBiographyAnalysis& plat_anal = analysis.platforms[plat_i];
	if (plat_anal.packed_reactions.GetCount() != ranges.GetCount())
		plat_anal.packed_reactions.Clear();
	plat_anal.packed_reactions.SetCount(ranges.GetCount());
	const String& range_output = plat_anal.packed_reactions[range_i];
	
	if (skip_ready && range_output.GetCount()) {
		NextSubBatch();
		return;
	}
	
	SocialArgs args;
	args.fn = 5;
	Range& range = ranges[range_i];
	if (range.input[0] >= 0) {
		const String& r0 = plat_anal.packed_reactions[range.input[0]];
		args.parts.Add("<merged reaction #1>", r0);
	}
	else {
		int begin = range.off;
		int len_2 = range.len / 2;
		int end = begin + len_2;
		for(int i = begin; i < end; i++) {
			int role_i = pa.roles[i];
			const BiographyRoleAnalysis& role = analysis.roles[role_i];
			args.parts.Add(Capitalize(GetSocietyRoleDescription(role_i)), role.merged_biography_reactions[0]);
		}
	}
	
	if (range.input[1] >= 0) {
		const String& r0 = plat_anal.packed_reactions[range.input[1]];
		args.parts.Add("<merged reaction #2>", r0);
	}
	else {
		int len_2 = range.len / 2;
		int begin = range.off + len_2;
		int end = begin + range.len - len_2;
		for(int i = begin; i < end; i++) {
			int role_i = pa.roles[i];
			const BiographyRoleAnalysis& role = analysis.roles[role_i];
			args.parts.Add(Capitalize(GetSocietyRoleDescription(role_i)), role.merged_biography_reactions[0]);
		}
	}
	
	SetWaiting(1);
	TaskMgr& m = TaskMgr::Single();
	m.GetSocial(args, THISBACK(OnProcessPlatformReactions));
}

void SocialSolver::OnProcessPlatformReactions(String res) {
	BiographyAnalysis& analysis = profile->biography_analysis;
	int plat_i = batch;
	int range_i = ranges.GetCount() - 1 - sub_batch;
	if (plat_i >= analysis.platforms.GetCount())
		analysis.platforms.SetCount(plat_i+1);
	
	PlatformBiographyAnalysis& plat_anal = analysis.platforms[plat_i];
	String& s = plat_anal.packed_reactions[range_i];
	
	s = TrimBoth(res);
	if (s.Left(1) == "\"") s = s.Mid(1);
	if (s.Right(1) == "\"") s = s.Left(s.GetCount()-1);
	
	NextSubBatch();
	SetWaiting(0);
}

void SocialSolver::ProcessPlatformDescriptions() {
	BiographyAnalysis& analysis = profile->biography_analysis;
	int plat_i = batch;
	
	if (plat_i >= PLATFORM_COUNT) {
		NextPhase();
		return;
	}
	
	const Platform& plat = GetPlatforms()[plat_i];
	
	if (plat_i >= analysis.platforms.GetCount())
		analysis.platforms.SetCount(plat_i+1);
	
	PlatformBiographyAnalysis& plat_anal = analysis.platforms[plat_i];
	if (skip_ready && plat_anal.profile_description_from_biography.GetCount()) {
		NextBatch();
		return;
	}
	
	if (plat_anal.packed_reactions.IsEmpty()) {
		LOG("error: plat_anal.packed_reactions is empty");
		NextBatch();
		return;
	}
	
	const String& merged_reactions = plat_anal.packed_reactions[0];
	SocialArgs args;
	args.fn = 6;
	args.parts.Add("", merged_reactions);
	
	SetWaiting(1);
	TaskMgr& m = TaskMgr::Single();
	m.GetSocial(args, THISBACK(OnProcessPlatformDescriptions));
}

void SocialSolver::OnProcessPlatformDescriptions(String res) {
	BiographyAnalysis& analysis = profile->biography_analysis;
	int plat_i = batch;
	if (plat_i >= analysis.platforms.GetCount())
		analysis.platforms.SetCount(plat_i+1);
	
	PlatformBiographyAnalysis& plat_anal = analysis.platforms[plat_i];
	
	String& s = plat_anal.profile_description_from_biography;
	s = TrimBoth(res);
	if (s.Left(1) == "\"") s = s.Mid(1);
	if (s.Right(1) == "\"") s = s.Left(s.GetCount()-1);
	
	NextBatch();
	SetWaiting(0);
}

void SocialSolver::ProcessPlatformDescriptionRefinements() {
	BiographyAnalysis& analysis = profile->biography_analysis;
	int plat_i = batch;
	
	if (plat_i >= PLATFORM_COUNT) {
		NextPhase();
		return;
	}
	
	if (sub_batch >= PLATDESC_LEN_COUNT) {
		NextBatch();
		return;
	}
	int len_i = sub_batch;
	int mode_i = PLATDESC_MODE_FINAL;
	
	const Platform& plat = GetPlatforms()[plat_i];
	
	PlatformBiographyAnalysis& plat_anal = analysis.platforms[plat_i];
	String s = plat_anal.descriptions[len_i][mode_i];
	if (skip_ready && s.GetCount()) {
		NextSubBatch();
		return;
	}
	
	String source;
	if(len_i == 0) {
		source = plat_anal.profile_description_from_biography;
	}
	else {
		source = plat_anal.descriptions[len_i-1][PLATDESC_MODE_FINAL];
	}
	
	const String& merged_reactions = plat_anal.packed_reactions[0];
	SocialArgs args;
	args.fn = len_i == 0 ? 7 : 8;
	args.text = source;
	args.len = GetPlatformDescriptionLength(len_i);
	
	SetWaiting(1);
	TaskMgr& m = TaskMgr::Single();
	m.GetSocial(args, THISBACK(OnProcessPlatformDescriptionRefinements));
}

void SocialSolver::OnProcessPlatformDescriptionRefinements(String res) {
	BiographyAnalysis& analysis = profile->biography_analysis;
	int plat_i = batch;
	const Platform& plat = GetPlatforms()[plat_i];
	PlatformBiographyAnalysis& plat_anal = analysis.platforms[plat_i];
	int len_i = sub_batch;
	int mode_i = PLATDESC_MODE_FINAL;
	String& s = plat_anal.descriptions[len_i][mode_i];
	
	s = TrimBoth(res);
	if (s.Left(1) == "\"") s = s.Mid(1);
	if (s.Right(1) == "\"") s = s.Left(s.GetCount()-1);
	
	NextSubBatch();
	SetWaiting(0);
}

void SocialSolver::ProcessPlatformDescriptionTranslated() {
	BiographyAnalysis& analysis = profile->biography_analysis;
	int plat_i = batch;
	
	if (plat_i >= PLATFORM_COUNT) {
		NextPhase();
		return;
	}
	int total_subbatches = PLATDESC_MODE_COUNT * PLATDESC_LEN_COUNT;
	if (sub_batch >= total_subbatches) {
		NextBatch();
		return;
	}
	
	int len_i = sub_batch / PLATDESC_MODE_COUNT;
	int mode_i = sub_batch % PLATDESC_MODE_COUNT;
	if (mode_i == PLATDESC_MODE_FINAL) {
		NextSubBatch();
		return;
	}
	
	const Platform& plat = GetPlatforms()[plat_i];
	
	PlatformBiographyAnalysis& plat_anal = analysis.platforms[plat_i];
	String s = plat_anal.descriptions[len_i][mode_i];
	String source = plat_anal.descriptions[len_i][PLATDESC_MODE_FINAL];
	String dst_ln = mode_i == PLATDESC_MODE_FINAL_DIALECT ? "EN-EN" : "FI-FI";
	bool slightly_dialect = (mode_i == PLATDESC_MODE_FINAL_DIALECT || mode_i == PLATDESC_MODE_FINAL_TRANSLATED_DIALECT);
	
	if (skip_ready && s.GetCount()) {
		NextSubBatch();
		return;
	}
	
	const String& merged_reactions = plat_anal.packed_reactions[0];
	
	SetWaiting(1);
	TaskMgr& m = TaskMgr::Single();
	m.Translate("EN-EN", source, dst_ln, THISBACK(OnProcessPlatformDescriptionTranslated), slightly_dialect);
}

void SocialSolver::OnProcessPlatformDescriptionTranslated(String res) {
	BiographyAnalysis& analysis = profile->biography_analysis;
	int plat_i = batch;
	const Platform& plat = GetPlatforms()[plat_i];
	PlatformBiographyAnalysis& plat_anal = analysis.platforms[plat_i];
	int len_i = sub_batch / PLATDESC_MODE_COUNT;
	int mode_i = sub_batch % PLATDESC_MODE_COUNT;
	
	String& s = plat_anal.descriptions[len_i][mode_i];
	s = TrimBoth(res);
	if (s.Left(1) == "\"") s = s.Mid(1);
	if (s.Right(1) == "\"") s = s.Left(s.GetCount()-1);
	
	NextSubBatch();
	SetWaiting(0);
}

void SocialSolver::ProcessMergeMessages() {
	
	if (batch == 0) {
		msg_tasks.Clear();
		for(int i = 0; i < owner->profiles.GetCount(); i++)
			for(int j = 0; j < PLATFORM_COUNT; j++)
				TraverseMessageTasks(i, j);
	}
	
	if (batch >= msg_tasks.GetCount()) {
		NextPhase();
		return;
	}
	
	const MessageTask& t = msg_tasks[batch];
	
	SocialArgs args;
	args.fn = 9;
	
	if (t.comments.GetCount() == 1) {
		args.text = "<no existing summary, because we are waiting the first message>";
	}
	else {
		// Find lastest merged status (should be second last)
		for (int i = t.comments.GetCount()-2; i >= 0; i--) {
			PlatformComment& pc = *t.comments[i];
			if (pc.text_merged_status.GetCount()) {
				args.text = pc.text_merged_status;
				break;
			}
			else if (i == 0) {
				if (pc.user.IsEmpty())
					args.parts.Add("me", pc.message);
				else
					args.parts.Add(pc.user, pc.message);
			}
		}
	}
	
	// Add new comment
	PlatformComment& pc = *t.comments.Top();
	if (pc.user.IsEmpty())
		args.parts.Add("me", pc.message);
	else
		args.parts.Add(pc.user, pc.message);
	
	SetWaiting(1);
	TaskMgr& m = TaskMgr::Single();
	m.GetSocial(args, THISBACK(OnProcessMergeMessages));
}

void SocialSolver::OnProcessMergeMessages(String res) {
	const MessageTask& t = msg_tasks[batch];
	
	String& s = t.comments.Top()->text_merged_status;
	s = TrimBoth(res);
	if (s.Left(1) == "\"") s = s.Mid(1);
	if (s.Right(1) == "\"") s = s.Left(s.GetCount()-1);
	
	NextBatch();
	SetWaiting(0);
}

void SocialSolver::ProcessAnalyzeImageBiography() {
	
	if (batch == 0) {
		vision_tasks.Clear();
		TraverseVisionTasks();
	}
	
	if (batch >= vision_tasks.GetCount()) {
		NextPhase();
		return;
	}
	
	const VisionTask& t = vision_tasks[batch];
	
	VisionArgs args;
	args.fn = 0;
	
	SetWaiting(1);
	TaskMgr& m = TaskMgr::Single();
	m.GetVision(t.jpeg, args, THISBACK(OnProcessAnalyzeImageBiography));
}

void SocialSolver::OnProcessAnalyzeImageBiography(String res) {
	const VisionTask& t = vision_tasks[batch];
	
	String& s = t.bimg->image_text;
	s = TrimBoth(res);
	if (s.Left(1) == "\"") s = s.Mid(1);
	if (s.Right(1) == "\"") s = s.Left(s.GetCount()-1);
	
	NextBatch();
	SetWaiting(0);
}

void SocialSolver::ProcessSummarizeImageCategoryYear() {
	
	if (batch == 0) {
		imgsum_tasks.Clear();
		TraverseImageSummaryTasks();
	}
	
	if (batch >= imgsum_tasks.GetCount()) {
		NextPhase();
		return;
	}
	
	const ImageSummaryTask& t = imgsum_tasks[batch];
	const BioRange& range = t.range;
	const BiographyCategory& bcat = *t.bcat;
	BioYear& by = *t.by;
	
	SocialArgs args;
	args.fn = 10;
	
	if (range.len == 2) {
		int begin = range.off;
		int end = range.off + range.len;
		ASSERT(begin < end && end - begin < 100);
		for(int i = begin; i < end && i < by.images.GetCount(); i++) {
			BioImage& bi = by.images[i];
			String title = GetBiographyCategoryKey(t.bcat_i) +
				", year " + IntStr(by.year) +
				", age " + IntStr(by.year - owner->year_of_birth) +
				", image #" + IntStr(i);
			if (bi.image_text.GetCount())
				args.parts.Add(title, bi.image_text);
		}
		if (args.parts.IsEmpty()) {
			NextBatch();
			return;
		}
		else if (args.parts.GetCount() == 1) {
			OnProcessSummarizeImageCategoryYear("(" + args.parts.GetKey(0) + ") " + args.parts[0]);
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
			int j = by.image_summaries.Find(sub_range);
			ASSERT(j >= 0);
			BioImage& bi = by.image_summaries[j];
			int from = sub_range.off;
			int to = sub_range.off + sub_range.len - 1;
			String title =
				GetBiographyCategoryKey(t.bcat_i) +
				", year " + IntStr(by.year) +
				", age " + IntStr(by.year - owner->year_of_birth) +
				", images from #" + IntStr(from) + " to #" + IntStr(to);
				;
			if (bi.image_text.GetCount())
				args.parts.Add(title, bi.image_text);
		}
		if (args.parts.IsEmpty()) {
			NextBatch();
			return;
		}
		else if (args.parts.GetCount() == 1) {
			OnProcessSummarizeImageCategoryYear("(" + args.parts.GetKey(0) + ") " + args.parts[0]);
			return;
		}
	}
	
	SetWaiting(1);
	TaskMgr& m = TaskMgr::Single();
	m.GetSocial(args, THISBACK(OnProcessSummarizeImageCategoryYear));
}

void SocialSolver::OnProcessSummarizeImageCategoryYear(String res) {
	const ImageSummaryTask& t = imgsum_tasks[batch];
	
	int j = t.by->image_summaries.Find(t.range);
	ASSERT(j >= 0);
	BioImage& bi = t.by->image_summaries[j];
	String& s = bi.image_text;
	
	s = TrimBoth(res);
	if (s.Left(1) == "\"") s = s.Mid(1);
	if (s.Right(1) == "\"") s = s.Left(s.GetCount()-1);
	
	if (j == t.by->image_summaries.GetCount()-1)
		t.by->image_text = s;
	
	NextBatch();
	SetWaiting(0);
}

void SocialSolver::ProcessSummarizeImageBiography() {
	Biography& biography = profile->biography_detailed;
	
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
	
	SocialArgs args;
	args.fn = 10;
	
	if (range.len == 2) {
		int begin = range.off;
		int end = range.off + range.len;
		ASSERT(begin < end && end - begin < 100);
		for(int i = begin; i < end; i++) {
			BioYear& by = bcat.GetAdd(i);
			String title = GetBiographyCategoryKey(bcat_i) +
				", year " + IntStr(by.year) +
				", age " + IntStr(by.year - owner->year_of_birth);
			if (by.image_text.GetCount())
				args.parts.Add(title, by.image_text);
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
			if (by.image_text.GetCount())
				args.parts.Add(title, by.image_text);
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
	m.GetSocial(args, THISBACK(OnProcessSummarizeImageBiography));
}

void SocialSolver::OnProcessSummarizeImageBiography(String res) {
	Biography& biography = profile->biography_detailed;
	int bcat_i = batch;
	BiographyCategory& bcat = biography.GetAdd(*owner, bcat_i);
	const BioRange& range = bcat.summaries.GetKey(sub_batch);
	BioYear& sum = bcat.summaries[sub_batch];
	
	sum.image_text = TrimBoth(res);
	
	
	NextSubBatch();
	SetWaiting(0);
}

void SocialSolver::TraverseMessageTasks(int prof_i, int plat_i) {
	Profile& prof = owner->profiles[prof_i];
	ProfileData& pd = ProfileData::Get(prof);
	BiographyAnalysis& analysis = profile->biography_analysis;
	const Platform& plat = GetPlatforms()[plat_i];
	PlatformBiographyAnalysis& plat_anal = analysis.platforms[plat_i];
	PlatformData& pld = pd.platforms[plat_i];
	
	tmp_task.plat = &plat;
	tmp_task.plat_data = &pld;
	
	for(int i = 0; i < pld.entries.GetCount(); i++) {
		PlatformEntry& e = pld.entries[i];
		tmp_task.entry = &e;
		for(int j = 0; j < e.threads.GetCount(); j++) {
			PlatformThread& pt = e.threads[j];
			tmp_task.thrd = &pt;
			Vector<PlatformComment*> comments_before;
			for(int j = 0; j < pt.comments.GetCount(); j++) {
				PlatformComment& plc = pt.comments[j];
				TraverseMessageTasks(comments_before, plc);
			}
		}
	}
}

void SocialSolver::TraverseMessageTasks(Vector<PlatformComment*>& before, PlatformComment& plc) {
	before.Add(&plc);
	
	if (phase == SS_MERGE_MESSAGES) {
		if (plc.text_merged_status.IsEmpty() && before.GetCount() > 1) {
			MessageTask& t = msg_tasks.Add();
			t.plat = tmp_task.plat;
			t.plat_data = tmp_task.plat_data;
			t.thrd = tmp_task.thrd;
			t.entry = tmp_task.entry;
			t.comments <<= before;
		}
	}
	
	int begin_count = before.GetCount();
	for(int i = 0; i < plc.responses.GetCount(); i++) {
		TraverseMessageTasks(before, plc.responses[i]);
		before.SetCount(begin_count);
	}
	
}

void SocialSolver::TraverseVisionTasks() {
	Biography& biography = profile->biography_detailed;
	for(int i = 0; i < BIOCATEGORY_COUNT; i++) {
		BiographyCategory& bcat = biography.GetAdd(*owner, i);
		for(int j = 0; j < bcat.years.GetCount(); j++) {
			BioYear& by = bcat.years[j];
			
			for(int k = 0; k < by.images.GetCount(); k++) {
				BioImage& bimg = by.images[k];
				if (phase == SS_ANALYZE_IMAGE_BIOGRAPHY && bimg.image_text.IsEmpty() && bimg.image_hash != 0) {
					String path = CacheImageFile(bimg.image_hash);
					if (!FileExists(path))
						path = ThumbnailImageFile(bimg.image_hash);
					String jpeg = LoadFile(path);
					if (!jpeg.IsEmpty()) {
						VisionTask& t = vision_tasks.Add();
						t.bimg = &bimg;
						t.jpeg = jpeg;
					}
				}
			}
		}
	}
}

void SocialSolver::TraverseImageSummaryTasks() {
	Biography& biography = profile->biography_detailed;
	for(int i = 0; i < BIOCATEGORY_COUNT; i++) {
		BiographyCategory& bcat = biography.GetAdd(*owner, i);
		int bcat_i = i;
		for(int j = 0; j < bcat.years.GetCount(); j++) {
			BioYear& by = bcat.years[j];
			by.RealizeImageSummaries();
			for(int k = 0; k < by.image_summaries.GetCount(); k++) {
				const BioRange& range = by.image_summaries.GetKey(k);
				BioImage& bimg = by.image_summaries[k];
				if (phase == SS_SUMMARIZE_IMAGE_CATEGORY_YEAR && bimg.image_text.IsEmpty()) {
					ImageSummaryTask& t = imgsum_tasks.Add();
					t.bcat = &bcat;
					t.by = &by;
					t.range = range;
					t.summary = &bimg;
					t.bcat_i = bcat_i;
				}
			}
		}
	}
}

END_TEXTLIB_NAMESPACE
