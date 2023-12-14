#include "Agent.h"


Solver::Solver() {
	
}

void Solver::Load(const RhymeContainer& rc) {
	
	TODO
	
}

void Solver::Process() {
	
	while (running && !Thread::IsShutdownThreads()) {
		
		
		
		Sleep(10);
	}
	
	running = false;
	stopped = true;
}
