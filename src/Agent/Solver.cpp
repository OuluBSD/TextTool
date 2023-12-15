#include "Agent.h"

namespace SongAgent {


Solver::Solver() : searcher(mach) {
	
}

bool Solver::Load(const RhymeContainer& rc) {
	//const Vector<RhymeContainer::Line>& lines = rc.Get();
	this->rc = rc;
	
	root.SetEstimate(1000);
	searcher.TrimWorst(100,0);
	
	searcher.Init(root);
	
	return true;
}

void Solver::Process() {
	auto ret = searcher.SearchMain();
	
	/*while (running && !Thread::IsShutdownThreads()) {
		
		TODO
		
		Sleep(10);
	}*/
	
	running = false;
	stopped = true;
}

void Solver::Start() {
	Stop();
	running = true;
	stopped = false;
	Thread::Start(THISBACK(Process));
}

void Solver::Stop() {
	searcher.Stop();
	running = false;
	while (!stopped) Sleep(10);
}




}
