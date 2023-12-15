#include "Agent.h"

namespace SongAgent {

int AStar::FindNode(const Vector<NodePtr*>& vec, const Node* ptr) {
	int i = 0;
	for (const NodePtr* p : vec) {
		if (p->ptr == ptr)
			return i;
		i++;
	}
	return -1;
}




AStar::AStar(Machine& mach) : max_worst(0), limit(0), mach(mach) {}

void AStar::operator=(const AStar& as) {
	max_worst = as.max_worst;
	do_search = as.do_search;
	limit = as.limit;
	closed_set <<= as.closed_set;
	open_set <<= as.open_set;
}

void AStar::TrimWorst(int limit, int count) {
	rm_limit = limit;
	max_worst = count;
	ASSERT(count >= 0);
}

Vector<Node*> AStar::GetBestKnownPath() {
	if (smallest_id < 0)
		return Vector<Node*>();
	
	NodePtr* t_ptr = open_set[smallest_id];
	Node& t = *t_ptr->ptr;
	return ReconstructPath(t, closed_set, open_set);
}

Vector<Node*> AStar::ReconstructPath(Node& current, Vector<NodePtr*>& closed_set, Vector<NodePtr*>& open_set) {
	Vector<Node*> path;
	Node* ptr = &current;
	while (1) {
		path.Add(ptr);
		int i = FindNode(open_set, ptr);
		if (i == -1) {
			i = FindNode(closed_set, ptr);
			if (i == -1) break;
			else {
				const NodePtr* cf = closed_set[i]->came_from;
				ptr = cf ? cf->ptr : 0;
			}
		}
		else ptr = open_set[i]->came_from->ptr;
		if (!ptr) break;
	}
	Vector<Node*> out;
	out.SetCount(path.GetCount());
	for(int i = 0; i < path.GetCount(); i++) {
		out[path.GetCount()-1-i] = path[i];
	}
	return out;
}

Vector<Node*> AStar::ReconstructPath(Node& current) {
	return ReconstructPath(current, closed_set, open_set);
}

Vector<Node*> AStar::GetBest() {
	NodePtr* n = 0;
	double best_score = DBL_MAX;
	for (const NodePtr* p : closed_set) {
		double score = p->g_score + p->f_score;
		if (score < best_score) {
			best_score = score;
			n = const_cast<NodePtr*>(p);
		}
	}
	if (n)
		return ReconstructPath(*n->ptr, closed_set, open_set);
	
	return Vector<Node*>();
}

void AStar::Init(Node& src) {
	do_search = true;
	
	closed_set.Clear();
	open_set.Clear();
	
	// For the first node, that value is completely heuristic.
	NodePtr& np = nodes.Add();
	np.ptr = &src;
	np.g_score = 0;
	np.f_score = this->Estimate(src);
	open_set.Add(&np);
	
}

Vector<Node*> AStar::ContinueSearch(Node& src) {
	do_search = true;
	
	NodePtr* copy = 0;
	int i = 0;
	for (NodePtr* np : open_set) {
		if (np->ptr == &src) {
			copy = np;
			open_set.Remove(i);
			break;
		}
		++i;
	}
	if (!copy || !copy->ptr)
		return Vector<Node*>();
	
	closed_set.Append(open_set);
	open_set.Clear();
	
	open_set.Add(copy);
	
	return SearchMain();
}

Vector<Node*> AStar::SearchMain() {
	Vector<double> worst_f_score;
	Vector<int> worst_id;
	
	// while open_set is not empty
	for(; open_set.GetCount() && do_search;) {
		
		if (limit) {
			limit--;
			if (limit <= 0)
				do_search = 0;
		}
		
		bool rm = open_set.GetCount() > rm_limit;
		if (rm) {
			int max_worst = this->max_worst > 0 ? max_worst : open_set.GetCount()-rm_limit;
			worst_f_score.SetCount(max_worst);
			worst_id.SetCount(max_worst);
			
			for(int i = 0; i < max_worst; i++) {
				worst_f_score[i] = -DBL_MAX;
				worst_id[i] = -1;
			}

			for(int i = 0; i < open_set.GetCount(); i++) {
				const NodePtr& nptr = *open_set[i];
				double f_score = nptr.f_score;
				
				for(int j = 0; j < max_worst; j++) {
					if (f_score > worst_f_score[j]) {
						for(int k = max_worst-1; k > j; k--) {
							worst_f_score[k] = worst_f_score[k-1];
							worst_id[k] = worst_id[k-1];
						}
						worst_f_score[j] = f_score;
						worst_id[j] = i;
						break;
					}
				}
			}
			
			int count = 0;
			Vector<int> rm_list;
			for(int i = 0; i < max_worst; i++) {
				int id = worst_id[i];
				if (id == -1) break;
				closed_set.Add(open_set[id]);
				rm_list.Add(id);
			}
			
			if (rm_list.GetCount()) {
				if (rm_list.GetCount() > 1) {
					Sort(rm_list, StdLess<int>());
				}
				open_set.Remove(rm_list);
			}
		}
		
		
		double smallest_f_score = DBL_MAX;
		smallest_id = -1;
		
		for(int i = 0; i < open_set.GetCount(); i++) {
			const NodePtr& nptr = *open_set[i];
			double f_score = nptr.f_score;
			if (f_score < smallest_f_score) {
				smallest_f_score = f_score;
				smallest_id = i;
			}
		}
		if (smallest_id < 0)
			break;
		
		
		Node* prevs[5];
		NodePtr* t_ptr = open_set[smallest_id];
		Node& t = *t_ptr->ptr;
		const NodePtr* prev = t_ptr->came_from;
		for(int i = 0; i < 4; i++) {
			if (prev) {
				prevs[i] = prev->ptr;
				prev = prev->came_from;
			}
			else prevs[i] = 0;
		}
		prevs[4] = 0;
		double current_g_score = t_ptr->g_score;
		
		if (mach.TerminalTest(t, prevs))
			return ReconstructPath(t, closed_set, open_set);
		
		if (!do_search)
			break;
		
		open_set.Remove(smallest_id);
		smallest_id = -1;
		closed_set.Add(t_ptr);
		
		
		for(int j = 0; j < t.GetTotalCount(); j++) {
			Node& sub = t.AtTotal(j);
			if (FindNode(closed_set, &sub) != -1)
				continue; // Ignore the neighbor which is already evaluated.
			// The distance from start to a neighbor
			double sub_g_score = current_g_score + this->Distance(t, sub);
			double sub_f_score = sub_g_score + this->Estimate(sub);
			int k = FindNode(open_set, &sub);
			if (k == -1) {
				// Discover a new node
				k = open_set.GetCount();
				NodePtr& subptr = nodes.Add();
				subptr.ptr = &sub;
				subptr.came_from = t_ptr;
				subptr.f_score = sub_f_score;
				subptr.g_score = sub_g_score;
				ASSERT(subptr.ptr);
				open_set.Add(&subptr);
			}
			else if (sub_g_score >= current_g_score)
				continue; // This is not a better path.
			else {
				NodePtr& subptr = *open_set[k];
				ASSERT(subptr.ptr == &sub);
				ASSERT(subptr.came_from == t_ptr);
				ASSERT(subptr.ptr);
				subptr.f_score = sub_f_score;
				subptr.g_score = sub_g_score;
				open_set.Remove(k);
			}
		}
		
		
	}
	return Vector<Node*>();
}


}
