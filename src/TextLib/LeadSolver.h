#ifndef _TextLib_LeadSolver_h_
#define _TextLib_LeadSolver_h_


BEGIN_TEXTLIB_NAMESPACE


/*
Notes:
	- parse list of leads from A&R websites
	- analyze leads
	- analyze reference songs (by user giving info)
	- the product
		- prune bad options
		- rate good options
	
	- create multiple entries for suno
	- generate checklist for asserting generated suno songs
	
*/

class LeadSolver {
	enum {
		LS_BEGIN,
		LS_DOWNLOAD_WEBSITES,
		LS_PARSE_WEBSITES,
		
		LS_COUNT
	};
	Time time_started, time_stopped;
	int phase = LS_BEGIN;
	int batch = 0, sub_batch = 0, batch_count = 0, per_batch = 0;
	MetaEntity* entity = 0;
	LeadEntityAnalysis* sa = 0;
		
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
	
	void ClearDB();
	void ProcessDownloadWebsites(bool parse);
	void ParseWebsite(int batch, String content);
	String ProcessDownloadWebsiteUrl(String url);
	
	static String GetLeadCacheDir();
	
public:
	typedef LeadSolver CLASSNAME;
	LeadSolver();
	
	void Start() {if (!running) {running = true; stopped = false; Thread::Start(THISBACK(Process));}}
	void Stop() {running = false; while (!stopped) Sleep(1);}
	
	static LeadSolver& Get(MetaEntity& a);
	static void ClearTasks();
	static void RestartTasks();
	
	Callback2<int,int> WhenProgress;
	
};


END_TEXTLIB_NAMESPACE


#endif
