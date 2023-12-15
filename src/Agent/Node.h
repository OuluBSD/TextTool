#ifndef _Agent_Node_h_
#define _Agent_Node_h_


namespace SongAgent {

class WorldState;
class ActionPlanner;

class Node
{
public:
	typedef const Node ConstNode;
	typedef typename Array<Node>::Iterator Iterator;
	typedef typename Array<Node>::ConstIterator ConstIterator;
	
protected:
	friend bool TerminalTest(Node& n, Node** prev);
	double length;
	double length_to_node;
	double estimate_to_goal;
	
public:
	String ToString() const {return DblStr(length) + ", " + DblStr(length_to_node) + ", " + DblStr(estimate_to_goal);}
	double GetUtility() const {return length_to_node;}
	double GetEstimate() const {return estimate_to_goal;}
	double GetDistance(Node& n) {return n.length;}
	void SetEstimate(double d) {estimate_to_goal = d;}
	
private:
	Node* parent;
	Array<Node> sub;
	Vector<Node*> links;
	
	Node& SetParent(Node& n) {parent = &n; return *this;}
	
	
public:
	Node();
	
	void Serialize(Stream& s);
	hash_t GetHashValue() const;
	
	Node(const Node& n) : parent(0)  {
		*this = n;
	}
	
	Node& operator = (const Node& n) {
		sub <<= n.sub;
		for(int i = 0; i < sub.GetCount(); i++)
			sub[i].parent = this;
		return *this;
	}
	
	
	int GetDepth();
	Node* GetParent() const {return parent;}
	void RefreshParent();
	Vector<int> GetPosition() const;
	Node& Seek(const Vector<int>& pos);
	Node& operator[] (int i) {return sub[i];}
	const Node& operator[] (int i) const {return sub[i];}
	Node& At(int i) {return sub[i];}
	const Node& At(int i) const {return sub[i];}
	int GetCount() const {return sub.GetCount();}
	Node& AtTotal(int i);
	const Node& AtTotal(int i) const;
	int GetTotalCount() const;
	
	void Remove(int i) {sub.Remove(i);}
	void RemoveChild(Node* child);
	Node* DetachChild(Node* child);
	
	Node& Add() {return sub.Add().SetParent(*this);}
	Node& Add(const Node& n) {return sub.Add(n).SetParent(*this);}
	Node& Add(Node* n) {return sub.Add(n).SetParent(*this);}
	Node& Insert(int pos) {return sub.Insert(pos).SetParent(*this);}
	Node& Insert(int pos, const Node& n) {return sub.Insert(pos, new Node(n)).SetParent(*this);}
	Node& Insert(int pos, Node* n) {return sub.Insert(pos, n).SetParent(*this);}
	void SetCount(int n) {sub.SetCount(n);}
	void Clear() {sub.Clear();}
	void MoveTo(Node& node) {
		if (parent)
			node.Add(parent->DetachChild(this));
		else
			node.Add(*this);
	}
	
	void AddLink(Node& n);
	String AsString(int indent=0);
	
	
	
	
	class IteratorDeep {
		friend class Node;
		Array<Node>::Iterator begin;
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
	
	
	IteratorDeep			BeginDeep() { return IteratorDeep(this);}
	const IteratorDeep		BeginDeep() const { return IteratorDeep(this);}
	Array<Node>::Iterator	Begin() { return sub.Begin(); }
	Array<Node>::Iterator	End() { return sub.End(); }
	
};





}

#endif
