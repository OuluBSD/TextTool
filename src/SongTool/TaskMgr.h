#ifndef _SongTool_TaskMgr_h_
#define _SongTool_TaskMgr_h_


struct TaskRule {
	using ArgTuple = Tuple3<TaskArgType, int, int>;
	
	int code = -1;
	String name;
	void (Task::*input)() = 0;
	void (Task::*process)() = 0;
	Vector<ArgTuple> args;
	Vector<TaskOutputType> reqs;
	Vector<TaskOutputType> results;
	Vector<TaskArgType> hashes;
	bool spawnable = false;
	bool multi_spawnable = false;
	bool allow_cross_mode = false;
	bool separate_items = false;
	bool debug_input = false;
	VectorMap<int, Tuple2<int,int>> req_mode_ranges;
	
	TaskRule& Input(void (Task::*fn)());
	TaskRule& Arg(TaskArgType arg, int i0=0, int i1=0);
	TaskRule& Require(TaskOutputType arg);
	TaskRule& RequireMode(TaskOutputType arg, SnapMode begin, SnapMode end);
	TaskRule& Process(void (Task::*fn)());
	TaskRule& Result(TaskOutputType arg);
	TaskRule& Spawnable(bool b=true);
	TaskRule& MultiSpawnable(bool b=true);
	TaskRule& CrossMode(bool b=true);
	TaskRule& Hash(TaskArgType t);
	TaskRule& SeparateItems(bool b=true);
	TaskRule& DebugInput(bool b=true);
	
};

struct TaskMgr {
	Array<Task> tasks;
	
	RWMutex lock;
	Mutex task_lock;
	Task* active_task = 0;
	int actual = 0, total = 0;
	String status;
	
	// Local
	Vector<hash_t> task_order;
	int task_i = 0;
	
	// Temp
	Vector<String> task_order_dbg;
	bool task_order_cache_missed = false;
	mutable hash_t hash = 0;
	int iters = 0;
	int spawn_id = 0;
	
	typedef TaskMgr CLASSNAME;
	virtual ~TaskMgr() {}
	
	// Task order is stored to avoid cache miss, when AI gives bad results and task is skipped.
	void LoadTaskOrder();
	void StoreTaskOrder();
	void Serialize(Stream& s) {s % task_order;}
	
	void Process();
	void ProcessSingle(int task_i);
	void StartSingle(int task_i) {Thread::Start(THISBACK1(ProcessSingle, task_i));}
	bool SpawnTasks();
	bool IsDepsReady(Task& t, Index<Task*>& seen) const;
	GroupContext GetGroupContextLimit() const;
	
	hash_t GetSongHash() const;
	
	void ImportSongAndMakeReversedSong();
	void TranslateSongData(String orig_lang, String orig_key, String trans_lang, String trans_key, Callback WhenDone);
	void UnpackStructureSongData(String orig_key, String struct_key, Callback WhenDone);
	void CheckSongStructureErrors(String main_key, String results_key, Callback WhenDone);
	void CheckSongNaturalErrors(String main_key, String results_key, Callback WhenDone);
	void ConvertSongStructureToEnglish(String src_key, String dst_key, Callback WhenDone);
	void EvaluateSongAudience(String src_key, String dst_key, Callback WhenDone);
	void MakePoetic(String style, String src_key, String dst_key, Callback WhenDone);
	
	
};

struct TaskMgrConfig {
	String openai_token;
	Array<TaskRule> rules;
	bool running = false, stopped = true;
	int max_tries = 3;
	
	typedef TaskMgrConfig CLASSNAME;
	void Load();
	void Store();
	void Serialize(Stream& s) {s % openai_token;}
	void CreateDefaultTaskRules();
	void Start() {running = true; stopped = false; Thread::Start(THISBACK(Process));}
	void Stop() {running = false; while (!stopped) Sleep(100);}
	void Process();
	
	TaskRule& AddRule(int code, String name);
	TaskRule& GetRule(int code);
	const TaskRule& GetRule(int code) const;
	
	static TaskMgrConfig& Single() {static TaskMgrConfig m; return m;}
};

#endif
