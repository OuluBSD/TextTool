#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


PlatformProfileProcess::PlatformProfileProcess() {
	
}

int PlatformProfileProcess::GetPhaseCount() const {
	return PHASE_COUNT;
}

int PlatformProfileProcess::GetBatchCount(int phase) const {
	switch (phase) {
		case PHASE_ANALYZE_PROFILE_EPK_PHOTO_AI_PROMPTS:			return max(1, prof_epk_tasks.GetCount());
		case PHASE_ANALYZE_PROFILE_EPK_SUMMARIZE_PHOTO_AI_PROMPTS:	return snap->analysis.image_types.GetCount();
		case PHASE_ANALYZE_PROFILE_EPK_PHOTO_DALLE2_EXAMPLES:		return max(1, prof_epk_tasks.GetCount());
		default: return 1;
	}
}

int PlatformProfileProcess::GetSubBatchCount(int phase, int batch) const {
	return 1;
}

void PlatformProfileProcess::DoPhase() {
	switch (phase) {
		case PHASE_ANALYZE_PROFILE_EPK_PHOTO_AI_PROMPTS:			ProcessAnalyzeProfileEpkPhotoAiPrompts();
		case PHASE_ANALYZE_PROFILE_EPK_SUMMARIZE_PHOTO_AI_PROMPTS:	ProcessAnalyzeProfileEpkSummarizePhotoAiPrompts();
		case PHASE_ANALYZE_PROFILE_EPK_PHOTO_DALLE2_EXAMPLES:		ProcessAnalyzeProfileEpkPhotoDalle2Examples();
		default: return;
	}
}

PlatformProfileProcess& PlatformProfileProcess::Get(Profile& p, BiographySnapshot& snap) {
	static ArrayMap<String, PlatformProfileProcess> arr;
	
	String key = "PROFILE(" + p.name + "), REVISION(" + IntStr(snap.revision) + ")";
	PlatformProfileProcess& ts = arr.GetAdd(key);
	ts.owner = p.owner;
	ts.profile = &p;
	ts.snap = &snap;
	ASSERT(ts.owner);
	return ts;
}

void PlatformProfileProcess::ProcessAnalyzeProfileEpkPhotoAiPrompts() {
	if (batch == 0) {
		prof_epk_tasks.Clear();
		TraverseProfileEPKTasks();
	}
	
	if (batch >= prof_epk_tasks.GetCount()) {
		NextPhase();
		return;
	}
	const ProfileEPKTask& t = prof_epk_tasks[batch];
	
	SocialArgs args;
	args.fn = 16;
	args.text = (String)t.plat->name + ": " + t.plat->description;
	args.description = t.pba->profile_description_from_biography;
	for(int i = 0; i < t.pap->prompts.GetCount(); i++) {
		const PhotoPrompt& pp = t.pap->prompts[i];
		args.parts.Add(pp.prompt);
	}
	
	SetWaiting(1);
	TaskMgr& m = TaskMgr::Single();
	m.GetSocial(args, THISBACK(OnProcessAnalyzeProfileEpkPhotoAiPrompts));
}

void PlatformProfileProcess::OnProcessAnalyzeProfileEpkPhotoAiPrompts(String res) {
	const ProfileEPKTask& t = prof_epk_tasks[batch];
	
	if (res.Left(2) != "1.")
		res = "1. " + res;
	
	RemoveEmptyLines2(res);
	
	Vector<String> lines = Split(res, "\n");
	
	t.prof_pap->prompts.Clear();
	for(int i = 0; i < lines.GetCount(); i++) {
		String l = TrimBoth(lines[i]);
		RemoveQuotes(l);
		if (l.IsEmpty())
			continue;
		PhotoPrompt& pp = t.prof_pap->prompts.Add();
		pp.prompt = l;
	}
	
	SetWaiting(0);
	NextBatch();
}

void PlatformProfileProcess::ProcessAnalyzeProfileEpkSummarizePhotoAiPrompts() {
	BiographyAnalysis& analysis = snap->analysis;
	
	if (batch == 0) {
		analysis.RealizePromptImageTypes();
	}
	
	if (batch >= analysis.image_types.GetCount()) {
		NextPhase();
		return;
	}
	PhotoPromptGroupAnalysis& ppga = analysis.image_types[batch];
	if (skip_ready && ppga.prompt.GetCount()) {NextBatch(); return;}
	
	SocialArgs args;
	args.fn = 17;
	args.text = analysis.image_types.GetKey(batch);
	Vector<PhotoPromptLink> links = analysis.GetImageTypePrompts(args.text);
	
	if (links.GetCount() <= 1) {
		ppga.prompt = links[0].pp->prompt;
		NextBatch();
		return;
	}
	
	for(int i = 0; i < links.GetCount(); i++) {
		const PhotoPromptLink& ppl = links[i];
		args.parts.Add(ppl.pp->prompt);
	}
	
	SetWaiting(1);
	TaskMgr& m = TaskMgr::Single();
	m.GetSocial(args, THISBACK(OnProcessAnalyzeProfileEpkSummarizePhotoAiPrompts));
}

void PlatformProfileProcess::OnProcessAnalyzeProfileEpkSummarizePhotoAiPrompts(String res) {
	BiographyAnalysis& analysis = snap->analysis;
	PhotoPromptGroupAnalysis& ppga = analysis.image_types[batch];
	
	res = TrimBoth(res);
	if (res.Left(2) == "1.") res = res.Mid(2);
	
	String& s = ppga.prompt;
	s = TrimBoth(res);
	RemoveQuotes(s);
	
	SetWaiting(0);
	NextBatch();
}

void PlatformProfileProcess::ProcessAnalyzeProfileEpkPhotoDalle2Examples() {
	if (batch == 0) {
		prof_epk_tasks.Clear();
		TraverseProfileEPKTasks();
	}
	
	if (batch >= prof_epk_tasks.GetCount()) {
		NextPhase();
		return;
	}
	const ProfileEPKTask& t = prof_epk_tasks[batch];
	
	String s = t.pp->prompt;
	s.Replace(" intimate ", " close-knit ");
	s.Replace("Intimate ", "Close-knit ");
	
	SetWaiting(1);
	TaskMgr& m = TaskMgr::Single();
	m.CreateImage(s, 4, THISBACK1(OnProcessAnalyzeProfileEpkPhotoDalle2Examples, batch), 0, THISBACK(OnBatchError));
}

void PlatformProfileProcess::OnProcessAnalyzeProfileEpkPhotoDalle2Examples(Array<Image>& images, int batch) {
	if (batch >= prof_epk_tasks.GetCount()) return;
	const ProfileEPKTask& t = prof_epk_tasks[batch];
	
	for(int i = 0; i < images.GetCount(); i++) {
		Image& img = images[i];
		String path = t.pp->GetFilePath(i);
		String dir = GetFileDirectory(path);
		if (i == 0) RealizeDirectory(dir);
		JPGEncoder enc(95);
		enc.SaveFile(path, img);
	}
	
	// Basic limiting: don't exceed 100 tasks in minute: sleep 1 second
	Sleep(1000);
	
	SetWaiting(0);
	NextBatch();
}

void PlatformProfileProcess::TraverseProfileEPKTasks() {
	MetaDatabase& mdb = MetaDatabase::Single();
	BiographyAnalysis& analysis = snap->analysis;
	
	for(int i = 0; i < analysis.platforms.GetCount(); i++) {
		PlatformBiographyAnalysis& pba = analysis.platforms[i];
		if (!pba.platform_enabled)
			continue;
		
		const Platform& plat = GetPlatforms()[i];
		const PlatformAnalysis& pa = mdb.GetAdd(plat);
		int c = pa.epk_photos.GetCount();
		
		for(int j = 0; j < pa.epk_photos.GetCount(); j++) {
			String key = pa.epk_photos.GetKey(j);
			const PlatformAnalysisPhoto& pap = pa.epk_photos[j];
			PlatformAnalysisPhoto& prof_pap = pba.epk_photos.GetAdd(key);
			
			if (phase == PHASE_ANALYZE_PROFILE_EPK_PHOTO_AI_PROMPTS) {
				if (skip_ready && prof_pap.prompts.GetCount())
					continue;
				
				ProfileEPKTask& t = prof_epk_tasks.Add();
				t.pa = &pa;
				t.pap = &pap;
				t.prof_pap = &prof_pap;
				t.pba = &pba;
				t.plat = &plat;
			}
			else if (phase == PHASE_ANALYZE_PROFILE_EPK_PHOTO_DALLE2_EXAMPLES) {
				ASSERT(!prof_pap.prompts.IsEmpty());
				
				PhotoPrompt& pp = prof_pap.prompts[0];
				String path = pp.GetFilePath(0);
				if (skip_ready && FileExists(path))
					continue;
				
				ProfileEPKTask& t = prof_epk_tasks.Add();
				t.pa = &pa;
				t.pap = &pap;
				t.prof_pap = &prof_pap;
				t.pba = &pba;
				t.plat = &plat;
				t.pp = &pp;
			}
		}
	}
}


END_TEXTLIB_NAMESPACE
