#ifndef _SongLib_LyricsSolver_h_
#define _SongLib_LyricsSolver_h_


namespace SongLib {


class LyricsSolver {
	enum {
		LS_BEGIN,
		LS_COLOR,
		LS_ATTR,
		LS_ACTION,
		LS_FILTER,
		LS_PRIMARY,
		LS_SECONDARY_WORD_CLASS,
		LS_SECONDARY,
		
		LS_COUNT
	};
	Time time_started, time_stopped;
	int phase = LS_BEGIN;
	int batch = 0, sub_batch = 0, batch_count = 0, per_batch = 0;
	int ds_i = 0;
	TaskMgr* pipe = 0;
	Artist* artist = 0;
	Release* release = 0;
	Song* song = 0;
	
	bool waiting = false;
	bool running = false, stopped = true;
	bool skip_ready = true;
	
	void RealizePipe();
	void Process();
	void ProcessColor();
	void ProcessAttr();
	void ProcessAction();
	void OnProcessColor(String result);
	void OnProcessAttr(String result);
	void PostProgress() {WhenProgress(phase, LS_COUNT);}
	void SetNotRunning() {running = false;}
	void SetWaiting(bool b) {waiting = b;}
	void NextPhase() {phase++; batch = 0;}
	void NextBatch() {batch++; sub_batch = 0;}
	void NextSubBatch() {sub_batch++;}
	
public:
	typedef LyricsSolver CLASSNAME;
	LyricsSolver();
	
	void Start() {if (!running) {running = true; stopped = false; Thread::Start(THISBACK(Process));}}
	void Stop() {running = false; while (!stopped) Sleep(1);}
	
	static LyricsSolver& Get(Artist& a, Release& r, Song& s);
	
	Callback2<int,int> WhenProgress;
	
};


}

#endif
