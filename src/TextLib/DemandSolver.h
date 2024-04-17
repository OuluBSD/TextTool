#ifndef _TextLib_DemandSolver_h_
#define _TextLib_DemandSolver_h_


BEGIN_TEXTLIB_NAMESPACE

/*
Notes:
	- single page text/manifest for startup
	- growing phase / maintaining phase / legacy phase
	- fast (everyday) / regular (weekly or bi-weekly) / slow (from monthly to once in a half year)
	- list of products that can be demanded:
		songs, albums, tweets, blogs, reels, music videos, podcasts
	- list of "ingredients of minimum viable product" e.g. product of products
		- what static albums, messages, reels, etc. must be out at minimum
	- list of promotion that can be demanded for the product
		- attention from fast to regular / from regular to slow
	- list of demands for images
		- profile, target group, etc.
*/
class DemandSolver {
	enum {
		LS_BEGIN,
		
		LS_COUNT
	};
	Time time_started, time_stopped;
	int phase = LS_BEGIN;
	int batch = 0, sub_batch = 0, batch_count = 0, per_batch = 0;
	MetaEntity* entity = 0;
		
	bool waiting = false;
	bool running = false, stopped = true;
	bool skip_ready = true;
	
	void Process();
	
	void PostProgress() {WhenProgress(phase, LS_COUNT);}
	void SetNotRunning() {running = false;}
	void SetWaiting(bool b) {waiting = b;}
	void MovePhase(int p) {phase = p; batch = 0; sub_batch = 0;}
	void NextPhase() {phase++; batch = 0; sub_batch = 0;}
	void NextBatch() {batch++; sub_batch = 0;}
	void NextSubBatch() {sub_batch++;}
	
public:
	typedef DemandSolver CLASSNAME;
	DemandSolver();
	
	void Start() {if (!running) {running = true; stopped = false; Thread::Start(THISBACK(Process));}}
	void Stop() {running = false; while (!stopped) Sleep(1);}
	
	static DemandSolver& Get(MetaEntity& a);
	static void ClearTasks();
	static void RestartTasks();
	
	Callback2<int,int> WhenProgress;
	
};


END_TEXTLIB_NAMESPACE


#endif
