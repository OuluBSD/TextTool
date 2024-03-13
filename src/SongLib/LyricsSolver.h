#ifndef _SongLib_LyricsSolver_h_
#define _SongLib_LyricsSolver_h_


BEGIN_SONGLIB_NAMESPACE


class LyricsSolver {
	enum {
		LS_BEGIN,
		LS_FILTER,
		LS_FILL_LINES,
		/*LS_PRIMARY,
		LS_MAKE_HOLES,
		LS_FILL_HOLES,*/
		LS_COMPARISON,
		
		LS_COUNT
	};
	Time time_started, time_stopped;
	int phase = LS_BEGIN;
	int batch = 0, sub_batch = 0, batch_count = 0, per_batch = 0;
	int ds_i = 0;
	Artist* artist = 0;
	Lyrics* lyrics = 0;
		
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
	SongAnalysis* sa = 0;
	Vector<int> phrase_src;
	
	void Process();
	void ClearLyrics();
	void ProcessFilter();
	void ProcessFillLines();
	void ProcessPrimary();
	void ProcessMakeHoles();
	void ProcessFillHoles();
	void ProcessComparison();
	void OnProcessPrimary(String res);
	void OnProcessFillLines(String res);
	void OnProcessMakeHoles(String res);
	void OnProcessFillHoles(String res);
	void OnProcessComparison(String res);
	void PostProgress() {WhenProgress(phase, LS_COUNT);}
	void SetNotRunning() {running = false;}
	void SetWaiting(bool b) {waiting = b;}
	void MovePhase(int p) {phase = p; batch = 0; sub_batch = 0;}
	void NextPhase() {phase++; batch = 0; sub_batch = 0;}
	void NextBatch() {batch++; sub_batch = 0;}
	void NextSubBatch() {sub_batch++;}
	
public:
	typedef LyricsSolver CLASSNAME;
	LyricsSolver();
	
	void Start() {if (!running) {running = true; stopped = false; Thread::Start(THISBACK(Process));}}
	void Stop() {running = false; while (!stopped) Sleep(1);}
	
	static LyricsSolver& Get(Artist& a, Lyrics& l);
	
	Callback2<int,int> WhenProgress;
	
};


END_SONGLIB_NAMESPACE


#endif
