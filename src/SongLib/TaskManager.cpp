#include "SongLib.h"

namespace SongLib {

TaskManager::TaskManager() {
	
}

void TaskManager::DoActionlistCache(int ds_i) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	Task task;
	task.ds_i = ds_i;
	
	String dir = ConfigFile("share") + DIR_SEPS + "task_actionlist" + DIR_SEPS +
		sda.datasets.GetKey(ds_i);
	if (!DirectoryExists(dir))
		return;
	
	FindFile ff(dir + DIR_SEPS + "*.txt");
	do {
		String title = GetFileTitle(ff.GetName());
		if (title == "." ||title == "..") continue;
		String content = LoadFile(ff.GetPath());
		if (content.Find("RGB(") >= 0)
			OnActionlistColors(content, &task);
		else
			OnActionlistAttrs(content, &task);
	}
	while (ff.Next());
}

void TaskManager::DoActionlist(int ds_i, int fn) {
	if (IsInTaskList(TASK_ACTIONLIST))
		return;
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	if (uniq_acts.IsEmpty()) {
		for (const ActionPhrase& ap : da.action_phrases) {
			for (const auto& a : ap.actions)
				uniq_acts.GetAdd(a.action).GetAdd(a.arg, 0)++;
		}
		struct Sorter {
			bool operator()(const VectorMap<String, int>& a, const VectorMap<String, int>& b) const {
				return a.GetCount() > b.GetCount();
			}
		};
		SortByValue(uniq_acts, Sorter());
		for (auto& v : uniq_acts.GetValues())
			SortByValue(v, StdGreater<int>());
	}
	
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_ACTIONLIST;
	t.cb = THISBACK1(GetActionlist, &t);
	t.ds_i = ds_i;
	t.batch_i = 0;
	t.fn = fn;
	lock.LeaveWrite();
}

void TaskManager::DoActionParallel(int ds_i, int fn) {
	if (IsInTaskList(TASK_ACTION_PARALLELS))
		return;
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_ACTION_PARALLELS;
	t.cb = THISBACK1(GetActionParallels, &t);
	t.ds_i = ds_i;
	t.batch_i = 0;
	t.fn = fn;
	lock.LeaveWrite();
}

void TaskManager::DoActionTransition(int ds_i, int fn) {
	if (IsInTaskList(TASK_ACTION_TRANSITIONS))
		return;
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_ACTION_TRANSITIONS;
	t.cb = THISBACK1(GetActionTransitions, &t);
	t.ds_i = ds_i;
	t.batch_i = 0;
	t.fn = fn;
	lock.LeaveWrite();
}

bool TaskManager::IsInTaskList(TaskType type) const {
	for (const Task& t : task_list)
		if (t.type == type)
			return true;
	return false;
}

void TaskManager::Start() {
	if (running) return;
	running = true;
	stopped = false;
	Thread::Start(THISBACK(Process));
}

void TaskManager::Stop() {
	running = false;
	while (!stopped) Sleep(10);
}

void TaskManager::Process() {
	
	while (running && !Thread::IsShutdownThreads()) {
		
		if (task_list.IsEmpty()) {
			Sleep(10);
			continue;
		}
		
		lock.EnterRead();
		Task* tp = 0;
		for (Task& t : task_list) {
			if (!t.running) {
				tp = &t;
				break;
			}
		}
		lock.LeaveRead();
		
		if (!tp) {
			Sleep(10);
			continue;
		}
		
		tp->running = true;
		tp->cb();
	}
	
	running = false;
	stopped = true;
}

void TaskManager::RemoveTask(Task& t) {
	lock.EnterWrite();
	int pos = -1;
	for(int i = 0; i < task_list.GetCount(); i++) {
		if (&task_list[i] == &t) {
			pos = i;
			break;
		}
	}
	ASSERT(pos >= 0);
	if (pos >= 0)
		task_list.Remove(pos);
	lock.LeaveWrite();
}

void TaskManager::RealizePipe() {
	Database& db = Database::Single();
	if (!pipe) {
		pipe = &db.pipes.Add();
		pipe->song = 0;
	}
}

void TaskManager::GetActionlist(Task* t) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	ActionAnalysisArgs args;
	args.fn = t->fn;
	
	int per_action_task = 0;
	if (t->fn == 0)	per_action_task = per_action_clrs;
	if (t->fn == 1)	per_action_task = per_action_attrs;
	
	int begin = t->batch_i * per_action_task;
	int end = begin + per_action_task;
	
	int iter = 0;
	for(int i = 0; i < uniq_acts.GetCount(); i++) {
		const VectorMap<String,int>& idx = uniq_acts[i];
		for(int j = 0; j < idx.GetCount(); j++) {
			if (iter >= begin && iter < end) {
				String s = uniq_acts.GetKey(i) + "(" + idx.GetKey(j) + ")";
				args.actions << s;
			}
			iter++;
		}
	}
	if (args.actions.IsEmpty()) {
		RemoveTask(*t);
		return; // ready
	}
	
	RealizePipe();
	TaskMgr& m = *pipe;
	if (t->fn == 0)
		m.GetActionAnalysis(args, THISBACK1(OnActionlistColors, t));
	else if (t->fn == 1)
		m.GetActionAnalysis(args, THISBACK1(OnActionlistAttrs, t));
}

void TaskManager::OnActionlistColors(String result, Task* t) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	// "attention-humor(not taking life too seriously)" RGB(255, 255, 0)
	
	result.Replace("\r", "");
	Vector<String> lines = Split(result, "\n");
	for (String& line : lines) {
		line = TrimBoth(line);
		if (line.IsEmpty()) continue;
		if (line[0] != '\"') continue;
		int a = 1;
		int b = line.Find("\"", 1);
		if (b < 0) continue;
		String full_action = line.Mid(a, b-a);
		a = line.Find("RGB(", b);
		if (a < 0) continue;
		a += 4;
		b = line.Find(")", a);
		if (b < 0) continue;
		String clr_str = line.Mid(a,b-a);
		Vector<String> clr_parts = Split(clr_str, ",");
		if (clr_parts.GetCount() != 3) continue;
		int R = ScanInt(TrimLeft(clr_parts[0]));
		int G = ScanInt(TrimLeft(clr_parts[1]));
		int B = ScanInt(TrimLeft(clr_parts[2]));
		Color clr(R,G,B);
		a = full_action.Find("(");
		if (a < 0) continue;
		ActionHeader ah;
		ah.action = full_action.Left(a);
		a++;
		b = full_action.Find(")", a);
		ah.arg = full_action.Mid(a,b-a);
		
		ActionAttrs& aa = da.action_attrs.GetAdd(ah);
		aa.clr = clr;
	}
	
	t->batch_i++;
	t->running = false;
}

void TaskManager::OnActionlistAttrs(String result, Task* t) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	// "attention-procedures(planning)" problem solving strategy / shortcut taking
	
	result.Replace("\r", "");
	Vector<String> lines = Split(result, "\n");
	for (String& line : lines) {
		line = TrimBoth(line);
		if (line.IsEmpty()) continue;
		if (line[0] != '\"') continue;
		int a = 1;
		int b = line.Find("\"", 1);
		if (b < 0) continue;
		String full_action = line.Mid(a, b-a);
		
		a = b+1;
		b = line.Find("/", b);
		if (a < 0) continue;
		String attr_group = TrimBoth(line.Mid(a,b-a));
		a = b+1;
		String attr_value = TrimBoth(line.Mid(a));
		
		a = full_action.Find("(");
		if (a < 0) continue;
		ActionHeader ah;
		ah.action = full_action.Left(a);
		a++;
		b = full_action.Find(")", a);
		ah.arg = full_action.Mid(a,b-a);
		
		ActionAttrs& aa = da.action_attrs.GetAdd(ah);
		aa.group = attr_group;
		aa.value = attr_value;
	}
	
	t->batch_i++;
	t->running = false;
}

void TaskManager::GetActionParallels(Task* t) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	TimeStop ts;
	da.action_parallel.Clear();
	
	// Loop from previous to 'current' to get score at the 'current'
	// (Instead of 'current' to next)
	
	for(const ActionPhrase& prev : da.action_phrases) {
		for(int k = 0; k < prev.next_phrases.GetCount(); k++) {
			int next_i = prev.next_phrases[k];
			const ActionPhrase& ap = da.action_phrases[next_i];
			
			for(int i = 0; i < ap.actions.GetCount(); i++) {
				const auto& aa0 = ap.actions[i];
				for(int j = 0; j < ap.actions.GetCount(); j++) {
					if (i == j) continue;
					
					const auto& aa1 = ap.actions[j];
					// we have a transition
					ActionHeader ah0, ah1;
					ah0.action = aa0.action;
					ah0.arg = aa0.arg;
					ah1.action = aa1.action;
					ah1.arg = aa1.arg;
					ActionParallel& ap = da.action_parallel.GetAdd(ah0).GetAdd(ah1);
					ap.count++; // increase count
					
					if (k < prev.next_scores.GetCount()) {
						auto& sc = prev.next_scores[k];
						int total_score = 0;
						for(int j = 0; j < SCORE_MODE_COUNT; j++) {
							for(int k = 0; k < SCORE_ATTR_COUNT; k++) {
								int v = sc.scores[j][k];
								total_score += v;
							}
						}
						ap.score_sum += max(0, min(
							SCORE_MODE_COUNT * SCORE_ATTR_COUNT,
							total_score));
					}
				}
			}
		}
	}
	
	LOG("TaskManager::GetActionParallels took " << ts.ToString());
}

void TaskManager::GetActionTransitions(Task* t) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[t->ds_i];
	
	TimeStop ts;
	da.action_trans.Clear();
	
	for(const ActionPhrase& ap : da.action_phrases) {
		int ap_i0 = 0;
		for(int next_i : ap.next_phrases) {
			if (ap_i0 < ap.next_scores.GetCount()) {
				const ActionPhrase& next = da.action_phrases[next_i];
				
				for (const auto& aa0 : ap.actions) {
					for (const auto& aa1 : next.actions) {
						if (aa0.action == aa1.action) {
							// we have a transition
							ActionHeader ah0, ah1;
							ah0.action = aa0.action;
							ah0.arg = aa0.arg;
							ah1.action = aa1.action;
							ah1.arg = aa1.arg;
							ActionTransition& at = da.action_trans.GetAdd(ah0).GetAdd(ah1);
							at.count++; // increase count
							
							auto& sc = ap.next_scores[ap_i0];
							int total_score = 0;
							for(int j = 0; j < SCORE_MODE_COUNT; j++) {
								for(int k = 0; k < SCORE_ATTR_COUNT; k++) {
									int v = sc.scores[j][k];
									total_score += v;
								}
							}
							at.score_sum += max(0, min(
								SCORE_MODE_COUNT * SCORE_ATTR_COUNT,
								total_score));
						}
					}
				}
			}
			ap_i0++;
		}
	}
	
	LOG("TaskManager::GetActionTransitions took " << ts.ToString());
}

}
