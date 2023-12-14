#ifndef _Agent_Solver_h_
#define _Agent_Solver_h_


class Solver {
	ActionMachine mach;
	bool running = false;
	bool stopped = true;
	
	
protected:
	
	void Process();
	
public:
	typedef Solver CLASSNAME;
	Solver();
	
	void Load(const RhymeContainer& rc);
	void Start() {Stop(); running = true; stopped = false; Thread::Start(THISBACK(Process));}
	void Stop() {running = false; while (!stopped) Sleep(10);}
	
};


#endif
