#ifndef _TextLib_SolverBase_h_
#define _TextLib_SolverBase_h_

BEGIN_TEXTLIB_NAMESPACE


class SolverBase {
	
protected:
	Time time_started, time_stopped;
	int phase = 0, batch = 0, sub_batch = 0, batch_count = 0, per_batch = 0;
	Owner* owner = 0;
	Profile* profile = 0;
	
	bool waiting = false;
	bool running = false, stopped = true;
	bool skip_ready = true;
	
	void Process();
	
	void PostProgress() {WhenProgress(phase, GetPhaseCount());}
	void SetNotRunning() {running = false;}
	void SetWaiting(bool b) {waiting = b;}
	void MovePhase(int p) {phase = p; batch = 0; sub_batch = 0;}
	void NextPhase() {phase++; batch = 0; sub_batch = 0;}
	void NextBatch() {batch++; sub_batch = 0;}
	void NextSubBatch() {sub_batch++;}
	
public:
	typedef SolverBase CLASSNAME;
	SolverBase();
	
	void Start() {if (!running) {running = true; stopped = false; Thread::Start(THISBACK(Process));}}
	void Stop() {running = false; while (!stopped) Sleep(1);}
	
	virtual int GetPhaseCount() const = 0;
	virtual void DoPhase() = 0;
	virtual void OnBatchError() {NextBatch();}
	
	Callback2<int,int> WhenProgress;
	
};


END_TEXTLIB_NAMESPACE

#endif
