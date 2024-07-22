#include "TextLib.h"

#if 0
BEGIN_TEXTLIB_NAMESPACE


TaskManager::TaskManager() {
	
}

void TaskManager::Clear() {
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_CLEAR;
	t.cb = THISBACK1(DoClear, &t);
	lock.LeaveWrite();
}

void TaskManager::DoClear(Task* tp) {
	TimeStop ts;
	while (1) {
		bool any_running = false;
		for (Task& t : task_list) {
			if (&t == tp)
				continue;
			if (t.running)
				any_running = true;
		}
		if (any_running) {
			if (ts.Seconds() >= 30.0)
				return;
			Sleep(10);
		}
		else
			break;
	}
	lock.EnterWrite();
	task_list.Clear();
	lock.LeaveWrite();
}

bool TaskManager::IsInTaskList(TaskType type) const {
	for (const Task& t : task_list)
		if (t.type == type)
			return true;
	return false;
}

void TaskManager::Start() {
	//ImportPromptAnswers();
	
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
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	EnterAppMode(appmode);
	
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
	
	LeaveAppMode();
	
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



void TaskManager::ImportPromptAnswers() {
	String dir = ConfigFile("import");
	FindFile ff(AppendFileName(dir, "*.txt"));
	int i = 0;
	do {
		String path = ff.GetPath();
		String content = LoadFileBOM(path);
		
		Task t;
		if (content.Find("RGB(") >= 0) {
			OnDetails(content, &t);
		}
		else {
			OnSyllables(content, &t);
		}
		i++;
	}
	while (ff.Next());
	
}

TextDatabase& TaskManager::GetDatabase() {
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	return MetaDatabase::Single().db[appmode];
}

TaskManager& TaskManager::Single(int appmode) {
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	static TaskManager o[DB_COUNT];
	static bool init;
	if (!init) {
		for(int i = 0; i < DB_COUNT; i++)
			o[i].appmode = i;
		init = true;
	}
	return o[appmode];
}

int TaskManager::GetTypeclassCount() {
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	return TextLib::GetTypeclassCount(appmode);
}

int TaskManager::GetContentCount() {
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	return TextLib::GetContentCount(appmode);
}


END_TEXTLIB_NAMESPACE
#endif
