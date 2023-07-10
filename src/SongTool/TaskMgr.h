#ifndef _SongTool_TaskMgr_h_
#define _SongTool_TaskMgr_h_


struct TaskMgr {
	Array<AI_Task> tasks;
	RWMutex lock;
	
	AI_Task* active_task = 0;
	
	
	void Store();
	
	static TaskMgr& Single() {static TaskMgr m; return m;}
	
};


#endif
