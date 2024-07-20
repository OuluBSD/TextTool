#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


SolverBase::SolverBase() {
	
}

void SolverBase::Process() {
	generation = 0;
	phase = 0;
	batch = 0;
	sub_batch = 0;
	
	TimeStop update_ts;
	
	while (running && !Thread::IsShutdownThreads()) {
		if (waiting) {
			Sleep(10);
			continue;
		}
		
		int phase_count = GetPhaseCount();
		
		if (generation == 0 && phase == 0 && batch == 0 && sub_batch == 0) {
			time_started = GetSysTime();
		}
		
		if (phase >= 0 && phase < phase_count) {
			EnterAppMode(appmode);
			DoPhase();
			LeaveAppMode();
		}
		else {
			generation++;
			phase = 0;
			batch = 0;
			sub_batch = 0;
			
			if (generation >= generation_count) {
				time_stopped = GetSysTime();
				WhenReady();
				break;
			}
		}
		
		if (update_ts.Seconds() >= 0.1) {
			PostProgress();
			update_ts.Reset();
		}
		
		Sleep(1);
	}
	
	running = false;
	stopped = true;
}

void SolverBase::StopAll() {
	LOG("SolverBase::StopAll: TODO");
}

TextDatabase& SolverBase::GetDatabase() const {
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	return MetaDatabase::Single().db[appmode];
}

void SolverBase::PostProgress() {
	static const int LEVELS = 3;
	int a[LEVELS], t[LEVELS];
	a[0] = phase;
	t[0] = GetPhaseCount();
	a[1] = batch;
	t[1] = GetBatchCount();
	a[2] = sub_batch;
	t[2] = GetSubBatchCount();
	int actual = ((a[0]) * t[1] + a[1]) * t[2] + a[2];
	int total = t[0] * t[1] * t[2];
	ASSERT(actual >= 0);
	ASSERT(total > 0);
	ASSERT(actual <= total);
	WhenProgress(actual, total);
}


END_TEXTLIB_NAMESPACE