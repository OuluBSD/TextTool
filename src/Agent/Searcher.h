#ifndef _Agent_Searcher_h_
#define _Agent_Searcher_h_

namespace SongAgent {


class Machine;

class Searcher {
	
public:
	Searcher() {}
	
	inline double Utility(Node& n) {return n.GetUtility();}
	inline double Estimate(Node& n) {return n.GetEstimate();}
	inline double Distance(Node& n, Node& dest) {return n.GetDistance(dest);}
	
	
};


    
class AStar : public Searcher {
	
	struct NodePtr {
		Node* ptr;
		NodePtr() : ptr(0), g_score(DBL_MAX), f_score(DBL_MAX), came_from(0) {}
		
		hash_t GetHashValue() const {return UPP::GetHashValue((size_t)ptr);}
		bool operator == (const NodePtr& np) const {return np.ptr == ptr;}
		// For each node, the cost of getting from the start node to that node.
		double g_score;
		// For each node, the total cost of getting from the start node to the goal
		// by passing by that node. That value is partly known, partly heuristic.
		double f_score;
		// For each node, which node it can most efficiently be reached from.
		// If a node can be reached from many nodes, came_from will eventually contain the
		// most efficient previous step.
		const NodePtr* came_from;
	};
	
	int max_worst;
	bool do_search = false;
	int limit;
	int rm_limit = 1000;
	int smallest_id = -1;
	
	// The set of nodes already evaluated.
	Array<NodePtr> nodes;
	Vector<NodePtr*> closed_set;
	
	// The set of currently discovered nodes still to be evaluated.
	// Initially, only the start node is known.
	Vector<NodePtr*> open_set;
	
	
	static int FindNode(const Vector<NodePtr*>& vec, const Node* ptr);
	
	Machine& mach;
	
public:
	AStar(Machine& mach);
	
	void operator=(const AStar& as);
	void SetLimit(int i) {limit = i;}
	void Stop() {do_search = false;}
	
	void TrimWorst(int limit, int count);
	
	void Init(Node& src);
	
	Vector<Node*> GetBestKnownPath();
	Vector<Node*> ReconstructPath(Node& current, Vector<NodePtr*>& closed_set, Vector<NodePtr*>& open_set);
	Vector<Node*> ReconstructPath(Node& current);
	Vector<Node*> GetBest();
	Vector<Node*> ContinueSearch(Node& src);
	Vector<Node*> SearchMain();
	
};


}

#endif
