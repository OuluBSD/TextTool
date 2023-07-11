#include "SongTool.h"


void TaskMgr::Process() {
	
	while (running) {
		this->total = tasks.GetCount();
		
		int ready = 0;
		for(int i = 0; i < tasks.GetCount(); i++) {
			AI_Task& t = tasks[i];
			if (!t.ready) {
				ProcessSingle(i);
				if (t.ready) {
					actual++;
					ready++;
				}
			}
			else
				ready++;
		}
		this->actual = ready;
		
		Sleep(10);
	}
	stopped = true;
}

void TaskMgr::ProcessSingle(int task_i) {
	task_lock.Enter();
	
	AI_Task& t = tasks[task_i];
	
	// Skip ready, failed and those with non-ready dependencies
	Index<AI_Task*> seen;
	if (t.ready || t.failed || !t.IsDepsReady(seen))
		;
	else {
		status = "Processing #" + IntStr(task_i);
		t.Process();
		status = "";
	}
	
	task_lock.Leave();
}

void TaskMgr::Load() {
	LoadFromFile(*this, ConfigFile("taskmgr.bin"));
}

void TaskMgr::Store() {
	StoreToFile(*this, ConfigFile("taskmgr.bin"));
	for (AI_Task& t : tasks)
		t.Store();
}
