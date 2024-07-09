#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


SolverBase::SolverBase() {
	
}

void SolverBase::Process() {
	generation = 0;
	phase = 0;
	batch = 0;
	sub_batch = 0;
	
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
			DoPhase();
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
		
		
		PostProgress();
		Sleep(1);
	}
	
	running = false;
	stopped = true;
}

void SolverBase::StopAll() {
	LOG("SolverBase::StopAll: TODO");
}

END_TEXTLIB_NAMESPACE