#ifndef _SocialLib_StoryGenerator_h_
#define _SocialLib_StoryGenerator_h_


BEGIN_SOCIALLIB_NAMESPACE


class StoryGenerator {
	enum {
		LG_BEGIN,
		LG_COLOR,
		LG_ATTR,
		LG_ACTION,
		LG_MAKE_SOURCE_POOL,
		LG_MAKE_PHRASE_PAIRS,
		LG_MAKE_RHYMES,
		LG_GET_AI_SCORES,
		
		LG_COUNT
	};
	Time time_started, time_stopped;
	int phase = LG_BEGIN;
	int batch = 0, sub_batch = 0, batch_count = 0, per_batch = 0;
	int ds_i = 0;
	TaskMgr* pipe = 0;
	Company* company = 0;
	Story* story = 0;
	
	bool waiting = false;
	bool running = false, stopped = true;
	bool skip_ready = true;
	
	// params
	int per_sub_batch =  50;
	int pair_limit = 300;
	int phrase_limit = 150;
	
	// temp
	Vector<VectorMap<int,double>> phrases;
	int iter = 0;
	VectorMap<String, int> pp_is;
	
	void RealizePipe();
	void Process();
	void ProcessSourcePool();
	void ProcessPairPhrases();
	void ProcessColor();
	void ProcessAttr();
	void ProcessAction();
	void ProcessRhymes();
	void ProcessScores();
	void OnProcessColor(String result);
	void OnProcessAttr(String result);
	void OnProcessPairPhrases(String result);
	void OnProcessRhymes(String result);
	void OnProcessScores(String result);
	void PostProgress() {WhenProgress(phase, LG_COUNT);}
	void SetNotRunning() {running = false;}
	void SetWaiting(bool b) {waiting = b;}
	void NextPhase() {phase++; batch = 0; sub_batch = 0;}
	void NextBatch() {batch++; sub_batch = 0;}
	void NextSubBatch() {sub_batch++;}
	
public:
	typedef StoryGenerator CLASSNAME;
	StoryGenerator();
	
	void Start() {if (!running) {running = true; stopped = false; Thread::Start(THISBACK(Process));}}
	void Stop() {running = false; while (!stopped) Sleep(1);}
	
	static StoryGenerator& Get(Company& a, Story& l);
	
	Callback2<int,int> WhenProgress;
	
};


END_SOCIALLIB_NAMESPACE


#endif
