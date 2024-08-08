#ifndef _Task_TaskMgr_h_
#define _Task_TaskMgr_h_


BEGIN_TEXTLIB_NAMESPACE



struct TaskMgr {
	Array<AiTask> tasks;
	
	RWMutex lock;
	Mutex task_lock;
	AiTask* active_task = 0;
	int actual = 0, total = 0;
	String status;
	
	// Local
	Vector<hash_t> task_order;
	int task_i = 0;
	int keep_going_counter = 0;
	
	// Temp
	Vector<String> task_order_dbg;
	bool task_order_cache_missed = false;
	mutable hash_t hash = 0;
	int iters = 0;
	int spawn_id = 0;
	
	typedef TaskMgr CLASSNAME;
	virtual ~TaskMgr() {}
	
	AiTask& AddTask();
	void Process();
	void ProcessSingle(int task_i);
	void StartSingle(int task_i) {Thread::Start(THISBACK1(ProcessSingle, task_i));}
	
	void Translate(String orig_lang, String orig_txt, String trans_lang, Event<String> WhenResult, bool slightly_dialect=false);
	void CreateImage(String prompt, int count, Event<Array<Image>&> WhenResult, int reduce_size_mode=0, Event<> WhenError=Event<>());
	void GetEditImage(Image orig, Image mask, String prompt, int count, Event<Array<Image>&> WhenResult, Event<> WhenError=Event<>());
	void VariateImage(Image orig, int count, Event<Array<Image>&> WhenResult, Event<> WhenError=Event<>());
	void RawCompletion(String prompt, Event<String> WhenResult);
	
	void GetStructureSuggestions(int appmode, const StructureArgs& args, Event<String> WhenResult);
	void GetSuggestionAttributes(int appmode, const StructureArgs& args, Event<String> WhenResult);
	void GetSourceDataAnalysis(int appmode, const SourceDataAnalysisArgs& args, Event<String> WhenResult, bool keep_going=false);
	void GetActionAnalysis(int appmode, const ActionAnalysisArgs& args, Event<String> WhenResult);
	void GetTokenData(int appmode, const TokenArgs& args, Event<String> WhenResult);
	void GetPhraseData(int appmode, const PhraseArgs& args, Event<String> WhenResult);
	void GetAttributes(int appmode, const AttrArgs& args, Event<String> WhenResult);
	void GetScriptSolver(int appmode, const ScriptSolverArgs& args, Event<String> WhenResult);
	void GetLeadSolver(int appmode, const LeadSolverArgs& args, Event<String> WhenResult);
	void GetSocial(const SocialArgs& args, Event<String> WhenResult);
	void GetSnapshot(const SnapshotArgs& args, Event<String> WhenResult);
	void GetVideoSolver(const VideoSolverArgs& args, Event<String> WhenResult);
	void GetDemandSolver(const DemandArgs& args, Event<String> WhenResult);
	void GetSocialBeliefsProcess(const BeliefArgs& args, Event<String> WhenResult);
	void GetScriptPost(int appmode, const ScriptPostArgs& args, Event<String> WhenResult);
	void GetVision(const String& jpeg, const VisionArgs& args, Event<String> WhenResult);
	void GetScriptTextProcess(int appmode, const ScriptTextProcessArgs& args, Event<String> WhenResult);
	void GetBiography(const BiographyProcessArgs& args, Event<String> WhenResult);
	void GetBiographySummary(const BiographySummaryProcessArgs& args, Event<String> WhenResult);
	void GetConceptualFramework(int appmode, const ConceptualFrameworkArgs& args, Event<String> WhenResult);
	void GetBiographyGenerator(const BiographyGeneratorArgs& args, Event<String> WhenResult);
	
	static TaskMgr& Single() {static TaskMgr tm; return tm;}
	
	
	template <class T>
	String MakeName(T& o, int appmode, const char* name);
	
};

struct TaskMgrConfig {
	String openai_token;
	bool running = false, stopped = true;
	int max_tries = 3;
	
	typedef TaskMgrConfig CLASSNAME;
	void Load();
	void Store();
	void Serialize(Stream& s) {s % openai_token;}
	void Start() {running = true; stopped = false; Thread::Start(THISBACK(Process));}
	void Stop() {running = false; while (!stopped) Sleep(100);}
	void Process();
	
	static TaskMgrConfig& Single() {static TaskMgrConfig m; return m;}
};


END_TEXTLIB_NAMESPACE


#endif
