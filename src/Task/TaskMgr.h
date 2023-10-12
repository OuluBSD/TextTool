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
	TaskRule& RequireMode(TaskOutputType arg, SnapMode begin, SnapMode end);
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
	
	Task& AddTask();
	void Process();
	void ProcessSingle(int task_i);
	void StartSingle(int task_i) {Thread::Start(THISBACK1(ProcessSingle, task_i));}
	bool SpawnTasks();
	bool IsDepsReady(Task& t, Index<Task*>& seen) const;
	GroupContext GetGroupContextLimit() const;
	
	hash_t GetSongHash() const;
	
	void ImportSongAndMakeReversedSong();
	void Translate(String orig_lang, String orig_txt, String trans_lang, Event<String> WhenResult);
	void TranslateSongData(String orig_lang, String orig_key, String trans_lang, String trans_key, Callback WhenDone);
	void UnpackStructureSongData(String orig_key, String struct_key, Callback WhenDone);
	void CheckSongStructureErrors(String main_key, String results_key, Callback WhenDone);
	void CheckSongNaturalErrors(String main_key, String results_key, Callback WhenDone);
	void ConvertSongStructureToEnglish(String struct_txt, Event<String> WhenResult);
	void EvaluateSongAudience(String src_key, String dst_key, int mode, Callback WhenDone);
	void MakePoetic(String style, String src_key, String dst_key, Callback WhenDone);
	void EvaluatePoeticStyles(String rhyme, String rhyme_scheme, int rhyme_scheme_line_count, String attrs, String syllable_count_str, String forbidden_words, String frozen_begin, String frozen_end, Event<String> WhenResult);
	void MorphToAttributes(const Vector<String>& rhyme_lines, const Vector<String>& attrs, Event<String> WhenResult);
	void ConvertScreenplayToPlan(String src_key, String dst_key, Callback WhenDone);
	void ConvertScreenplayToStructure(String orig_txt, Event<String> WhenResult);
	void ConvertStructureToScreenplay(String orig_txt, Event<String> WhenResult);
	void CheckScreenplayStructureErrors(String txt, Event<String> WhenResult);
	void CreateImage(String prompt, int count, Event<Array<Image>&> WhenResult, int reduce_size_mode=0, Event<> WhenError=Event<>());
	void GetEditImage(Image orig, Image mask, String prompt, int count, Event<Array<Image>&> WhenResult, Event<> WhenError=Event<>());
	void VariateImage(Image orig, int count, Event<Array<Image>&> WhenResult, Event<> WhenError=Event<>());
	void RawCompletion(String prompt, Event<String> WhenResult);
	void EvaluateSuggestionScores(const Vector<String>& strs, Event<String> WhenResult);
	void EvaluateSuggestionOrder(const Vector<String>& strs, Event<String> WhenResult);
	void ImproveSourceText(const Vector<String>& strs, int style, Event<String> WhenResult);
	void LimitSyllableCount(const Vector<String>& strs, int syllables, Event<String> WhenResult);
	void GetAIAttributes(String orig_txt, int attr_count, Event<String> WhenResult);
	void GetStructureSuggestions(String req, String avoid, String desc, int total, Event<String> WhenResult);
	void GetSuggestionAttributes(Vector<String>& structs, Event<String> WhenResult);
	void GetNovelThemes(Vector<String>& attrs, Event<String> WhenResult);
	void GetNovelIdeas(String theme, Vector<String>& attrs, Event<String> WhenResult);
	void GetToneSuggestions(String theme, String idea, Vector<String>& attrs, Event<String> WhenResult);
	void GetAllegorySuggestions(String theme, String idea, String tone, Vector<String>& attrs, Event<String> WhenResult);
	void GetContentSuggestions(String theme, String idea, String tone, String alleg, Vector<String>& attrs, Event<String> WhenResult);
	void GetSymbolismSuggestions(String theme, String idea, Vector<String>& attrs, Event<String> WhenResult);
	void GetImagerySuggestions(String theme, String idea, Vector<String>& attrs, Event<String> WhenResult);
	
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
