#ifndef _TextLib_SocialSolver_h_
#define _TextLib_SocialSolver_h_


BEGIN_TEXTLIB_NAMESPACE


class SocialSolver {
	enum {
		SS_BEGIN,
		
		SS_AUDIENCE_PROFILE_CATEGORIES,
		SS_SUMMARIZE,
		SS_AUDIENCE_REACTS_SUMMARY,
		SS_PACK_ROLE_REACTIONS,
		SS_PACK_PLATFORM_REACTIONS,
		SS_PLATFORM_DESCRIPTIONS,
		SS_PLATFORM_DESCRIPTION_REFINEMENTS,
		SS_PLATFORM_DESCRIPTION_TRANSLATED,
		
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
	
	struct Range : Moveable<Range> {
		int off = 0, len = 0;
		int input[2] = {0,0};
		String ToString() const {return Format("%d, %d: %d, %d", off, len, input[0], input[1]);}
	};
	Vector<Range> ranges;
	
	int CreateRange(int off, int len);
	void Process();
	void ProcessAudienceProfileCategories();
	void ProcessSummarize();
	void ProcessAudienceReactsSummary();
	void ProcessRoleReactions();
	void ProcessPlatformReactions();
	void ProcessPlatformDescriptions();
	void ProcessPlatformDescriptionRefinements();
	void ProcessPlatformDescriptionTranslated();
	void OnProcessAudienceProfileCategories(String res);
	void OnProcessSummarize(String res);
	void OnProcessAudienceReactsSummary(String res);
	void OnProcessRoleReactions(String res);
	void OnProcessPlatformReactions(String res);
	void OnProcessPlatformDescriptions(String res);
	void OnProcessPlatformDescriptionRefinements(String res);
	void OnProcessPlatformDescriptionTranslated(String res);
	
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
