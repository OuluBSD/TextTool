#ifndef _Task_TaskMgr_h_
#define _Task_TaskMgr_h_


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
	bool image_task = false;
	bool imageedit_task = false;
	bool imagevariate_task = false;
	VectorMap<int, Tuple2<int,int>> req_mode_ranges;
	
	TaskRule& Input(void (Task::*fn)());
	TaskRule& Arg(TaskArgType arg, int i0=0, int i1=0);
	TaskRule& Require(TaskOutputType arg);
	TaskRule& Process(void (Task::*fn)());
	TaskRule& Result(TaskOutputType arg);
	TaskRule& Spawnable(bool b=true);
	TaskRule& MultiSpawnable(bool b=true);
	TaskRule& CrossMode(bool b=true);
	TaskRule& Hash(TaskArgType t);
	TaskRule& SeparateItems(bool b=true);
	TaskRule& DebugInput(bool b=true);
	TaskRule& ImageTask(bool b=true);
	TaskRule& ImageEditTask(bool b=true);
	TaskRule& ImageVariateTask(bool b=true);
	
};

struct TaskMgr {
	Array<Task> tasks;
	
	RWMutex lock;
	Mutex task_lock;
	Task* active_task = 0;
	int actual = 0, total = 0;
	String status;
	Song* song = 0;
	
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
	
	Task& AddTask();
	void Process();
	void ProcessSingle(int task_i);
	void StartSingle(int task_i) {Thread::Start(THISBACK1(ProcessSingle, task_i));}
	
	void Translate(String orig_lang, String orig_txt, String trans_lang, Event<String> WhenResult);
	void GetStructureSuggestions(String req, String avoid, String desc, int total, Event<String> WhenResult);
	void GetSuggestionAttributes(Vector<String>& structs, Event<String> WhenResult);
	void CreateImage(String prompt, int count, Event<Array<Image>&> WhenResult, int reduce_size_mode=0, Event<> WhenError=Event<>());
	void GetEditImage(Image orig, Image mask, String prompt, int count, Event<Array<Image>&> WhenResult, Event<> WhenError=Event<>());
	void VariateImage(Image orig, int count, Event<Array<Image>&> WhenResult, Event<> WhenError=Event<>());
	void RawCompletion(String prompt, Event<String> WhenResult);
	void GetColorIdea(const ColorIdeaArgs& args, Event<String> WhenResult);
	void GetVocabulary(const VocabularyArgs& args, Event<String> WhenResult);
	void GetVocabularyIdea(const VocabularyIdeaArgs& args, Event<String> WhenResult);
	void GetWordSaladIdea(const WordSaladIdeaArgs& args, Event<String> WhenResult);
	void GetContextIdea(const ContextIdeaArgs& args, Event<String> WhenResult);
	void GetSongDataAnalysis(const SongDataAnalysisArgs& args, Event<String> WhenResult, bool keep_going=false);
	void GetActionAnalysis(const ActionAnalysisArgs& args, Event<String> WhenResult);
	void GetLyricsPhrase(const LyricsPhraseArgs& args, Event<String> WhenResult);
	void GetTokenData(const TokenArgs& args, Event<String> WhenResult);
	void GetPhraseData(const PhraseArgs& args, Event<String> WhenResult);
	void GetAttributes(const AttrArgs& args, Event<String> WhenResult);
	void GetNanaData(const NanaArgs& args, Event<String> WhenResult);
	void GetLyricsSolver(const LyricsSolverArgs& args, Event<String> WhenResult);
	
	static TaskMgr& Single() {static TaskMgr tm; return tm;}
	
	
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
