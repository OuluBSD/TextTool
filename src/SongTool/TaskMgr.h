#ifndef _SongTool_TaskMgr_h_
#define _SongTool_TaskMgr_h_


typedef enum : int {
	V_ATTR_SCORING,
	V_PTR_SONG,
	V_PTR_LINE,
	V_PTR_SONG_UNIQUELINES,
	V_MODE,
	V_ARGS,
	V_SONG_LYRICS,
	V_SONG_PARTS,
	V_LINE_TXT,
} TaskArgType;

typedef enum : int {
	O_ORDER_IMPORT,
	O_ORDER_IMPORT_DETAILED,
	O_ORDER_REVERSE,
	
	O_DB_ATTRS,
	O_DB_ATTR_SCORES,
	
	O_TASKS,
	
	O_SONG_MASK,
	O_SONG_ANALYSIS,
	O_SONG_DATA_STORYLINE,
	O_SONG_UNIQLINES,
	O_SONG_UNIQLINE_ATTRS,
	O_SONG_SNAP,
	O_SONG_REVERSED_MASK_COMMON,
	O_SONG_REVERSED_MASK,
	O_SONG_REVERSED_LYRICS,
	O_SONG_REVERSED_TRANSLATED_LYRICS,
	
	O_PART_MASK,
	O_PART_MASK_SCORE,
	O_PART_DATA_STORYLINE,
	O_PART_SNAP,
	O_PART_SNAP_SCORE,
	O_PART_REVERSED_SNAP,
	
	O_LINE_SNAP,
	O_LINE_SNAP_SCORE,
	O_LINE_REVERSED_SNAP,
	
	O_BREAK_SNAP,
	O_BREAK_SNAP_SCORE,
	O_BREAK_IMPACT,
	O_BREAK_IMPACT_SCORES,
	O_BREAK_REVERSED_IMPACT,
	O_BREAK_REVERSED_SNAP,
	
} TaskOutputType;

typedef enum : int {
	TASK_IMPORT_AND_REVERSE,
	
	TASK_PATTERNMASK,
	TASK_ANALYSIS,
	TASK_STORYARC,
	TASK_IMPACT,
	
	TASK_MAKE_IMPACT_SCORING_TASKS,
	TASK_IMPACT_SCORING,
	
	TASK_MAKE_PATTERN_TASKS,
	TASK_PATTERN,
	
	TASK_MAKE_ATTRSCORES_TASKS,
	TASK_ATTRSCORES,
	TASK_SONGSCORE,
	
	TASK_MAKE_REVERSE_IMPACT_TASK,
	TASK_REVERSE_IMPACT,
	
	TASK_MAKE_REVERSE_MASK_TASK,
	TASK_REVERSE_COMMON_MASK,
	TASK_REVERSE_SEPARATE_MASK,
	
	TASK_MAKE_REVERSEPATTERN_TASK,
	TASK_REVERSEPATTERN,
	
	TASK_MAKE_LYRICS_TASK,
	TASK_LYRICS,
	TASK_LYRICS_TRANSLATE,
	
	TASK_COUNT
} TaskType;

NAMESPACE_UPP
NTL_MOVEABLE(TaskArgType)
NTL_MOVEABLE(TaskOutputType)
NTL_MOVEABLE(TaskType)
END_UPP_NAMESPACE

struct TaskRule {
	using ArgTuple = Tuple3<TaskArgType, int, int>;
	
	int code = -1;
	String name;
	void (Task::*input)() = 0;
	void (Task::*process)() = 0;
	Vector<ArgTuple> args;
	Vector<TaskOutputType> reqs;
	Vector<TaskOutputType> results;
	bool spawnable = false;
	
	TaskRule& Input(void (Task::*fn)());
	TaskRule& Arg(TaskArgType arg, int i0=0, int i1=0);
	TaskRule& Require(TaskOutputType arg);
	TaskRule& Process(void (Task::*fn)());
	TaskRule& Result(TaskOutputType arg);
	TaskRule& Spawnable(bool b=true);
	
};

struct TaskMgr {
	String openai_token;
	Array<Task> tasks;
	Array<TaskRule> rules;
	
	RWMutex lock;
	Mutex task_lock;
	bool running = false, stopped = true;
	Task* active_task = 0;
	int actual = 0, total = 0;
	String status;
	int max_tries = 3;
	
	typedef TaskMgr CLASSNAME;
	void Start() {running = true; stopped = false; Thread::Start(THISBACK(Process));}
	void Stop() {running = false; while (!stopped) Sleep(100);}
	void Process();
	void ProcessSingle(int task_i);
	void StartSingle(int task_i) {Thread::Start(THISBACK1(ProcessSingle, task_i));}
	void Load();
	void Store();
	void Serialize(Stream& s) {s % openai_token;}
	void CreateDefaultTaskRules();
	bool SpawnTasks();
	bool IsDepsReady(Task& t, Index<Task*>& seen) const;
	
	void ImportSongAndMakeReversedSong(Song& s);
	
	TaskRule& AddRule(int code, String name);
	TaskRule& GetRule(int code);
	
	static TaskMgr& Single() {static TaskMgr m; return m;}
	
};


#endif
