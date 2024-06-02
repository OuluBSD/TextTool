#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


SolverBase::SolverBase() {
	
}

void SolverBase::Process() {
	phase = 0;
	batch = 0;
	sub_batch = 0;
	
	while (running && !Thread::IsShutdownThreads()) {
		if (waiting) {
			Sleep(10);
			continue;
		}
		
		int phase_count = GetPhaseCount();
		
		if (phase == 0 && batch == 0 && sub_batch == 0) {
			time_started = GetSysTime();
		}
		
		if (phase >= 0 && phase < phase_count) {
			DoPhase();
		}
		else {
			time_stopped = GetSysTime();
			phase = 0;
			break;
		}
		
		
		PostProgress();
		Sleep(1);
	}
	
	running = false;
	stopped = true;
}


END_TEXTLIB_NAMESPACE