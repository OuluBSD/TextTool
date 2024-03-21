#ifndef _TextLib_ScriptSolver_h_
#define _TextLib_ScriptSolver_h_


BEGIN_TEXTLIB_NAMESPACE


class ScriptSolver {
	enum {
		LS_BEGIN,
		LS_FILTER,
		LS_PRIMARY,
		LS_COMPARISON,
		LS_FINETUNING,
		
		/*LS_SECONDARY_WORD_CLASS,
		LS_SECONDARY_FILTER,
		LS_SECONDARY,*/
		
		LS_COUNT
	};
	Time time_started, time_stopped;
	int phase = LS_BEGIN;
	int batch = 0, sub_batch = 0, batch_count = 0, per_batch = 0;
	int ds_i = 0;
	Entity* artist = 0;
	Script* scripts = 0;
		
	bool waiting = false;
	bool running = false, stopped = true;
	bool skip_ready = true;
	int appmode = -1;
	
	// params
	double dist_limit = 0.005;
	int primary_count = 50;
	int rhyming_list_count = 5;
	int sugg_limit = 8;
	
	// temp
	Vector<VectorMap<int,double>> phrase_parts;
	Vector<String> phrases;
	Vector<Tuple2<int,int>> matches;
	Index<int> remaining;
	VectorMap<String,int> part_sizes;
	
	void Process();
	void ClearScript();
	void ProcessFilter();
	void ProcessPrimary();
	void ProcessComparison();
	void OnProcessPrimary(String res);
	void OnProcessComparison(String res);
	void PostProgress() {WhenProgress(phase, LS_COUNT);}
	void SetNotRunning() {running = false;}
	void SetWaiting(bool b) {waiting = b;}
	void NextPhase() {phase++; batch = 0;}
	void NextBatch() {batch++; sub_batch = 0;}
	void NextSubBatch() {sub_batch++;}
	
	TextDatabase& GetDatabase() {return GetAppModeDatabase(appmode);}
	
public:
	typedef ScriptSolver CLASSNAME;
	ScriptSolver();
	
	void Start() {if (!running) {running = true; stopped = false; Thread::Start(THISBACK(Process));}}
	void Stop() {running = false; while (!stopped) Sleep(1);}
	
	static ScriptSolver& Get(Entity& a, Script& l);
	
	Callback2<int,int> WhenProgress;
	
};


END_TEXTLIB_NAMESPACE


#endif
