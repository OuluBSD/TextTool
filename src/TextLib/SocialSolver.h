#ifndef _TextLib_SocialSolver_h_
#define _TextLib_SocialSolver_h_


BEGIN_TEXTLIB_NAMESPACE


class SocialSolver {
	enum {
		SS_BEGIN,
		
		SS_AUDIENCE_PROFILE_CATEGORIES,
		SS_SUMMARIZE,
		SS_AUDIENCE_REACTS_SUMMARY,
		SS_PLATFORM_AUDIENCE_TYPE,
		
		SS_COUNT
	};
	Time time_started, time_stopped;
	int phase = SS_BEGIN;
	int batch = 0, sub_batch = 0, batch_count = 0, per_batch = 0;
	Owner* owner = 0;
	
	bool waiting = false;
	bool running = false, stopped = true;
	bool skip_ready = true;
	
	Vector<BiographyProfileAnalysis*> ptrs;
	Vector<const RoleProfile*> prof_ptrs;
	Vector<String> role_descs;
	
	void Process();
	void ProcessAudienceProfileCategories();
	void ProcessSummarize();
	void ProcessAudienceReactsSummary();
	void OnProcessAudienceProfileCategories(String res);
	void OnProcessSummarize(String res);
	void OnProcessAudienceReactsSummary(String res);
	
	void PostProgress() {WhenProgress(phase, SS_COUNT);}
	void SetNotRunning() {running = false;}
	void SetWaiting(bool b) {waiting = b;}
	void MovePhase(int p) {phase = p; batch = 0; sub_batch = 0;}
	void NextPhase() {phase++; batch = 0; sub_batch = 0;}
	void NextBatch() {batch++; sub_batch = 0;}
	void NextSubBatch() {sub_batch++;}
	
public:
	typedef SocialSolver CLASSNAME;
	SocialSolver();
	
	void Start() {if (!running) {running = true; stopped = false; Thread::Start(THISBACK(Process));}}
	void Stop() {running = false; while (!stopped) Sleep(1);}
	
	static SocialSolver& Get(Owner& a);
	static void ClearTasks();
	static void RestartTasks();
	
	Callback2<int,int> WhenProgress;
	
};


END_TEXTLIB_NAMESPACE


#endif
