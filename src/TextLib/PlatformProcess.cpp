#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


PlatformProcess::PlatformProcess() {
	
}

int PlatformProcess::GetPhaseCount() const {
	return PHASE_COUNT;
}

int PlatformProcess::GetBatchCount(int phase) const {
	switch (phase) {
		case PHASE_ANALYZE_ROLE_SCORES:						return SOCIETYROLE_COUNT;
		case PHASE_ANALYZE_PLATFORM_ROLES:					return PLATFORM_COUNT;
		case PHASE_ANALYZE_PLATFORM_EPK_TEXT_FIELDS:		return PLATFORM_COUNT;
		case PHASE_ANALYZE_PLATFORM_EPK_PHOTO_TYPES:		return PLATFORM_COUNT;
		case PHASE_ANALYZE_PLATFORM_EPK_PHOTO_AI_PROMPTS:	return PLATFORM_COUNT;
		default: return 1;
	}
}

int PlatformProcess::GetSubBatchCount(int phase, int batch) const {
	if (phase == PHASE_ANALYZE_PLATFORM_EPK_PHOTO_AI_PROMPTS) {
		const Platform& plat = GetPlatforms()[batch];
		const PlatformAnalysis& pa = MetaDatabase::Single().GetAdd(plat);
		return pa.epk_photos.GetCount();
	}
	return 1;
}

void PlatformProcess::DoPhase() {
	switch (phase) {
		case PHASE_ANALYZE_ROLE_SCORES:						ProcessAnalyzeRoleScores(); return;
		case PHASE_ANALYZE_PLATFORM_ROLES:					ProcessAnalyzePlatformRoles(); return;
		case PHASE_ANALYZE_PLATFORM_EPK_TEXT_FIELDS:		ProcessAnalyzePlatformEpkTextFields(); return;
		case PHASE_ANALYZE_PLATFORM_EPK_PHOTO_TYPES:		ProcessAnalyzePlatformEpkPhotoTypes(); return;
		case PHASE_ANALYZE_PLATFORM_EPK_PHOTO_AI_PROMPTS:	ProcessAnalyzePlatformEpkPhotoAiPrompts(); return;
		default: return;
	}
}

PlatformProcess& PlatformProcess::Get(Profile& p, BiographySnapshot& snap) {
	static ArrayMap<String, PlatformProcess> arr;
	
	String key = "PROFILE(" + p.name + "), REVISION(" + IntStr(snap.revision) + ")";
	PlatformProcess& ts = arr.GetAdd(key);
	ts.owner = p.owner;
	ts.profile = &p;
	ts.snap = &snap;
	ASSERT(ts.owner);
	return ts;
}

void PlatformProcess::ProcessAnalyzeRoleScores() {
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

void PlatformProcess::OnProcessAnalyzeRoleScores(String res) {
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

void PlatformProcess::ProcessAnalyzePlatformRoles() {
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

void PlatformProcess::OnProcessAnalyzePlatformRoles(String res) {
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

void PlatformProcess::ProcessAnalyzePlatformEpkTextFields() {
	
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

void PlatformProcess::OnProcessAnalyzePlatformEpkTextFields(String res) {
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

void PlatformProcess::ProcessAnalyzePlatformEpkPhotoTypes() {
	
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

void PlatformProcess::OnProcessAnalyzePlatformEpkPhotoTypes(String res) {
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

void PlatformProcess::ProcessAnalyzePlatformEpkPhotoAiPrompts() {
	
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
		if (skip_ready && pap.prompts.GetCount()) {
		NextBatch();
		return;
	}
	
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

void PlatformProcess::OnProcessAnalyzePlatformEpkPhotoAiPrompts(String res) {
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


END_TEXTLIB_NAMESPACE
