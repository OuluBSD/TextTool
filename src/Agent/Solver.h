#ifndef _Agent_Solver_h_
#define _Agent_Solver_h_

namespace SongAgent {


class Solver {
	AStar searcher;
	Machine mach;
	Node root;
	
	RhymeContainer rc;
	bool running = false;
	bool stopped = true;
	
	
protected:
	
	
public:
	typedef Solver CLASSNAME;
	Solver();
	
	bool Load(const RhymeContainer& rc);
	void Start();
	void Stop();
	void Process();
	
};


}

#endif
