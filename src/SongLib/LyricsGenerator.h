#ifndef _SongLib_LyricsGenerator_h_
#define _SongLib_LyricsGenerator_h_


namespace SongLib {


class LyricsGenerator {
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
	Artist* artist = 0;
	Release* release = 0;
	Song* song = 0;
		
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
	typedef LyricsGenerator CLASSNAME;
	LyricsGenerator();
	
	void Start() {if (!running) {running = true; stopped = false; Thread::Start(THISBACK(Process));}}
	void Stop() {running = false; while (!stopped) Sleep(1);}
	
	static LyricsGenerator& Get(Artist& a, Release& r, Song& s);
	
	Callback2<int,int> WhenProgress;
	
};


}

#endif
