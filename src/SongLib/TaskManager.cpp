#include "SongLib.h"

namespace SongLib {

TaskManager::TaskManager() {
	
}

void TaskManager::DoActionlist(int ds_i, int fn) {
	if (IsInTaskList(TASK_ACTIONLIST))
		return;
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
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
	
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_ACTIONLIST;
	t.cb = THISBACK1(GetActionlist, &t);
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
	
	
	t->batch_i++;
	t->running = false;
}

void TaskManager::OnActionlistAttrs(String result, Task* t) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	
	t->batch_i++;
	t->running = false;
}

}
