#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE



SocialContentProcess::SocialContentProcess() {
	//skip_ready = false;
}

int SocialContentProcess::GetPhaseCount() const {
	return PHASE_COUNT;
}

int SocialContentProcess::GetBatchCount(int phase) const {
	if (phase == PHASE_MERGE_MESSAGES)
		return max(1, msg_tasks.GetCount());
	return 1;
}

int SocialContentProcess::GetSubBatchCount(int phase, int batch) const {
	return 1;
}

void SocialContentProcess::DoPhase() {
	ASSERT(owner);
	
	if (phase == PHASE_MERGE_MESSAGES) {
		ProcessMergeMessages();
	}
	else TODO
		
}

ArrayMap<hash_t, SocialContentProcess>& __SocialContentProcesss() {
	static ArrayMap<hash_t, SocialContentProcess> map;
	return map;
}

SocialContentProcess& SocialContentProcess::Get(Profile& p, BiographySnapshot& snap) {
	String t = p.owner->name + ": " + p.name;
	hash_t h = t.GetHashValue();
	ArrayMap<hash_t, SocialContentProcess>& map = __SocialContentProcesss();
	int i = map.Find(h);
	if (i >= 0)
		return map[i];
	
	SocialContentProcess& ls = map.Add(h);
	ls.owner = p.owner;
	ls.profile = &p;
	ls.snap = &snap;
	return ls;
}

void SocialContentProcess::ProcessMergeMessages() {
	
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

void SocialContentProcess::OnProcessMergeMessages(String res) {
	const MessageTask& t = msg_tasks[batch];
	
	String& s = t.comments.Top()->text_merged_status;
	s = TrimBoth(res);
	if (s.Left(1) == "\"") s = s.Mid(1);
	if (s.Right(1) == "\"") s = s.Left(s.GetCount()-1);
	
	NextBatch();
	SetWaiting(0);
}

void SocialContentProcess::TraverseMessageTasks(int prof_i, int plat_i) {
	Profile& prof = owner->profiles[prof_i];
	ProfileData& pd = ProfileData::Get(prof);
	BiographyAnalysis& analysis = snap->analysis;
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
				if (j == pt.comments.GetCount()-1 && plc.message.IsEmpty())
					break;
				TraverseMessageTasks(comments_before, plc);
			}
		}
	}
}

void SocialContentProcess::TraverseMessageTasks(Vector<PlatformComment*>& before, PlatformComment& plc) {
	before.Add(&plc);
	
	if (phase == PHASE_MERGE_MESSAGES) {
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


END_TEXTLIB_NAMESPACE
