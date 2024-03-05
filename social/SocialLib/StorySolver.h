#ifndef _SocialLib_StorySolver_h_
#define _SocialLib_StorySolver_h_


BEGIN_SOCIALLIB_NAMESPACE


class StorySolver {
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
	TaskMgr* pipe = 0;
	Company* company = 0;
	Story* story = 0;
		
	bool waiting = false;
	bool running = false, stopped = true;
	bool skip_ready = true;
	
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
	
	void RealizePipe();
	void Process();
	void ClearStory();
	void ProcessFilter();
	void ProcessPrimary();
	void ProcessComparison();
	//void ProcessFineTuning();
	/*void ProcessSecondaryWordClass();
	void ProcessSecondaryFilter();
	void ProcessSecondary();*/
	void OnProcessPrimary(String res);
	void OnProcessComparison(String res);
	//void OnProcessFineTuning(String res);
	/*void OnProcessSecondaryWordClass(String res);
	void OnProcessSecondary();*/
	void PostProgress() {WhenProgress(phase, LS_COUNT);}
	void SetNotRunning() {running = false;}
	void SetWaiting(bool b) {waiting = b;}
	void NextPhase() {phase++; batch = 0;}
	void NextBatch() {batch++; sub_batch = 0;}
	void NextSubBatch() {sub_batch++;}
	
public:
	typedef StorySolver CLASSNAME;
	StorySolver();
	
	void Start() {if (!running) {running = true; stopped = false; Thread::Start(THISBACK(Process));}}
	void Stop() {running = false; while (!stopped) Sleep(1);}
	
	static StorySolver& Get(Company& a, Story& l);
	
	Callback2<int,int> WhenProgress;
	
};


END_SOCIALLIB_NAMESPACE


#endif