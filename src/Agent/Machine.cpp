#include "Agent.h"

namespace SongAgent {


Machine::Machine() {
	
	
}

bool Machine::Run(const Vector<Node*>& prog) {
	
	TODO
	
	return true;
}

bool Machine::TerminalTest(Node& n, Node** prev) {
	
	TODO
	/*
	if (n.GetCount()) return !n.GetCount();
	int goal = 0;
	if (n.estimate_to_goal <= goal) return true;
	int sub_node_count = 2 + Random(1);
	for(int i = 0; i < sub_node_count; i++) {
		Node& sub = n.Add();
		double length = 5 + Random(10);
		// Accumulate total route length
		if (sub.GetParent()) {
			sub.length				 = length;
			sub.length_to_node		 = n.length_to_node + length;
			sub.estimate_to_goal	 = n.estimate_to_goal - length;
			if (sub.estimate_to_goal < goal) sub.estimate_to_goal = goal;
		}
	}
	return !n.GetCount();
	*/
	
	return true;
}

}
