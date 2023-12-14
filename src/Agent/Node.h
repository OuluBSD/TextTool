#ifndef _Agent_Node_h_
#define _Agent_Node_h_




class Node
{
public:
	typedef const Node ConstNode;
	
	typedef typename Array<Node>::Iterator Iterator;
	typedef typename Array<Node>::ConstIterator ConstIterator;
	
	
private:
	Node* parent;
	Array<Node> sub;
	
	Vector<Node*> links;
	
	Node& SetParent(Node& n) {parent = &n; return *this;}
	
public:
	
	void Serialize(Stream& s) {
		s % sub;
		if (s.IsLoading()) {
			for(int i = 0; i < sub.GetCount(); i++)
				sub[i].parent = this;
			parent = 0;
		}
	}
	
	hash_t GetHashValue() const {
		CombineHash ch;
		ch << GetCount() << 1;
		for(int i = 0; i < sub.GetCount(); i++)
			ch << sub[i].GetHashValue() << (i+2);
		return ch;
	}
	
	Node() : parent(0) {}
	
	Node(const Node& n) : parent(0)  {
		*this = n;
	}
	
	Node& operator = (const Node& n) {
		sub <<= n.sub;
		for(int i = 0; i < sub.GetCount(); i++)
			sub[i].parent = this;
		return *this;
	}
	
	virtual Node* GetNode() {return this;}
	
	int GetDepth() {Node* ptr = this; for(int i = 0;; i++) {ptr = ptr->parent; if (!ptr) return i;}}
	Node* GetParent() const {return parent;}
	void RefreshParent() {
		for(int i = 0; i < sub.GetCount(); i++)
			sub[i].SetParent(*this).RefreshParent();
	}
	Vector<int> GetPosition() const {
		Vector<int> out;
		const Node* ptr = parent;
		const Node* chk = this;
		while (ptr) {
			int count = ptr->GetCount();
			int pos = -1;
			for(int i = 0; i < count; i++) {
				const Node* iptr = &(*ptr)[i];
				if (iptr == chk) {
					pos = i; break;
				}
			}
			ASSERT(pos >= 0);
			out.Insert(0, pos);
			chk = ptr;
			ptr = ptr->parent;
		}
		return out;
	}
	
	Node& Seek(const Vector<int>& pos) {
		Node* ptr = this;
		for(int i = 0; i < pos.GetCount(); i++) {
			ptr = &ptr->At(pos[i]);
		}
		return *ptr;
	}
	
	Node& operator[] (int i) {return sub[i];}
	ConstNode& operator[] (int i) const {return sub[i];}
	Node& At(int i) {return sub[i];}
	ConstNode& At(int i) const {return sub[i];}
	
	int GetCount() const {return sub.GetCount();}
	
	Node& AtTotal(int i) {if (i < sub.GetCount()) return sub[i]; return *links[i-sub.GetCount()];}
	ConstNode& AtTotal(int i) const {if (i < sub.GetCount()) return sub[i]; return *links[i-sub.GetCount()];}
	
	int GetTotalCount() const {return sub.GetCount() + links.GetCount();}
	
	void Remove(int i) {sub.Remove(i);}
	void RemoveChild(Node* child) {
		for(int i = 0; i < sub.GetCount(); i++) {
			if (&sub[i] == child) {
				sub.Remove(i);
				return;
			}
		}
	}
	Node* DetachChild(Node* child) {
		int i = 0;
		for(Node& n : sub) {
			if (&n == child)
				return sub.Detach(i);
			i++;
		}
		return NULL;
	}
	Node& Add() {return sub.Add().SetParent(*this);}
	Node& Add(const Node& n) {return sub.Add(n).SetParent(*this);}
	Node& Add(Node* n) {return sub.Add(n).SetParent(*this);}
	Node& Insert(int pos) {return sub.Insert(pos).SetParent(*this);}
	Node& Insert(int pos, const Node& n) {return sub.Insert(pos, new Node(n)).SetParent(*this);}
	void SetCount(int n) {sub.SetCount(n);}
	void Clear() {sub.Clear();}
	void MoveTo(Node& node) {
		if (parent)
			node.Add(parent->DetachChild(this));
		else
			node.Add(*this);
	}
	
	void AddLink(Node& n) {
		for(int i = 0; i < links.GetCount(); i++) {
			if ((int64)&links[i] == (int64)&n)
				return;
		}
		links.Add(&n);
	}
	
	String AsString(int indent=0) {
		String s;
		for(int i = 0; i < indent; i++)
			s += "-";
		s += ": <TODO>\n";
		for(int i = 0; i < sub.GetCount(); i++) {
			s += sub[i].AsString(indent+1);
		}
		return s;
	}
	
	
	
	
	class IteratorDeep {
		friend class Node;
		Iterator begin;
		Vector<Node*> cur;
		Vector<int> pos;
	protected:
		
		IteratorDeep(Node* cur) {
			this->cur.Add(cur);
			pos.Add(0);
		}
		
		IteratorDeep(const Node* cur) {
			this->cur.Add((Node*)cur);
			pos.Add(0);
		}
		
	public:
		IteratorDeep(const IteratorDeep& it) {
			*this = it;
		}
		IteratorDeep& operator = (const IteratorDeep& it) {
			begin = it.begin;
			cur <<= it.cur;
			pos <<= it.pos;
			return *this;
		}
		int GetDepth() {return pos.GetCount();}
		int GetPos() {return pos[pos.GetCount()-1];}
		int64 GetCurrentAddr() {return (int64)cur[cur.GetCount() - 1];}
		
		bool IsEnd() const {return pos.GetCount() == 1 && pos[0] == 1;}
		bool operator == (IteratorDeep& it) {return GetCurrentAddr() == it.GetCurrentAddr();}
		bool operator != (IteratorDeep& it) {return !(*this == it);}
		void operator ++(int i) {
			#define LASTPOS pos[pos.GetCount()-1]
			#define LASTCUR cur[cur.GetCount()-1]
			#define SECLASTCUR cur[cur.GetCount()-2]
			#define ADDPOS pos[pos.GetCount()-1]++
			#define ADDCUR LASTCUR = &SECLASTCUR->At(LASTPOS)
			#define REMLASTPOS pos.Remove(pos.GetCount()-1)
			#define REMLASTCUR cur.Remove(cur.GetCount()-1)
			//String s; for(int i = 0; i < pos.GetCount(); i++) s << " " << pos[i]; LOG("+++ " << s);
			
			if (pos.GetCount() == 1 && pos[0] < 0) {pos[0]++; return;}
			if (pos.GetCount() == 1 && pos[0] == 1) return; // at end
			
			if (LASTCUR->GetCount()) {
				pos.Add(0);
				cur.Add(&LASTCUR->At(0));
			}
			else if (pos.GetCount() == 1) {
				pos[0] = 1; // at end
			}
			else {
				while (1) {
					if (LASTPOS + 1 < SECLASTCUR->GetCount()) {
						ADDPOS;
						ADDCUR;
						break;
					} else {
						REMLASTPOS;
						REMLASTCUR;
						if (pos.GetCount() <= 1) {
							pos.SetCount(1);
							pos[0] = 1;
							break;
						}
					}
				}
			}
		}
		void IncNotDeep() {
			if (LASTCUR->GetCount()) {
				while (1) {
					if (cur.GetCount() >= 2 && LASTPOS + 1 < SECLASTCUR->GetCount()) {
						ADDPOS;
						ADDCUR;
						break;
					} else {
						REMLASTPOS;
						REMLASTCUR;
						if (pos.GetCount() <= 1) {
							pos.SetCount(1);
							pos[0] = 1;
							break;
						}
					}
				}
			}
			else operator++(1);
		}
		void DecToParent() {
			pos.Remove(pos.GetCount()-1);
			cur.Remove(cur.GetCount()-1);
		}
		
		operator Node*() {
			if (pos.GetCount() && pos[0] == 1) return 0;
			return LASTCUR;
		}
		
		Node* operator->() {
			if (pos.GetCount() && pos[0] == 1) return 0;
			return LASTCUR;
		}
		
		Node& operator*() {
			return *LASTCUR;
		}
		
		const Node& operator*() const {return *LASTCUR;}
		
		Node* Higher() {
			if (cur.GetCount() <= 1) return 0;
			return cur[cur.GetCount()-2];
		}
	};
	
	
	IteratorDeep		BeginDeep() { return IteratorDeep(this);}
	const IteratorDeep	BeginDeep() const { return IteratorDeep(this);}
	Iterator			Begin() { return sub.Begin(); }
	Iterator			End() { return sub.End(); }
		
};



template <class T>
void GenerateTree(T& root, int total, int branching_low, int branching_high, Callback1<T*> set_value) {
	root.Clear();
	Vector<T*> next_level;
	
	next_level.Add(&root);
	
	int branching_range = branching_high - branching_low + 1;
	
	int count = 0;
	while (count < total) {
		Vector<T*> this_level;
		this_level <<= next_level;
		next_level.Clear();
		for(int i = 0; i < this_level.GetCount(); i++) {
			T& n = *this_level[i];
			int remaining = total-count;
			if (remaining <= 0) break;
			int sub_node_count = branching_low + Random(branching_range);
			if (sub_node_count > remaining) sub_node_count = remaining;
			n.SetCount(sub_node_count);
			count += sub_node_count;
			for(int j = 0; j < sub_node_count; j++) {
				T* ptr = &n[j];
				next_level.Add(ptr);
				set_value(ptr);
			}
		}
	}
}



#endif
