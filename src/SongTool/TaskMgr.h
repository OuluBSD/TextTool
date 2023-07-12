#ifndef _SongTool_TaskMgr_h_
#define _SongTool_TaskMgr_h_


struct TaskMgr {
	String openai_token;
	Array<AI_Task> tasks;
	
	RWMutex lock;
	Mutex task_lock;
	bool running = false, stopped = true;
	AI_Task* active_task = 0;
	int actual = 0, total = 0;
	String status;
	
	typedef TaskMgr CLASSNAME;
	void Start() {running = true; stopped = false; Thread::Start(THISBACK(Process));}
	void Stop() {running = false; while (!stopped) Sleep(100);}
	void Process();
	void ProcessSingle(int task_i);
	void StartSingle(int task_i) {Thread::Start(THISBACK1(ProcessSingle, task_i));}
	void Load();
	void Store();
	void Serialize(Stream& s) {s % openai_token;}
	
	static TaskMgr& Single() {static TaskMgr m; return m;}
	
};


#endif
