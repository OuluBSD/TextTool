#include "Agent.h"

namespace SongAgent {




Node::Node() {
	parent = 0;
	estimate_to_goal = DBL_MAX*0.99; // not the DBL_MAX for comparison functions
	length_to_node = 0;
	length = 0;
}

void Node::Serialize(Stream& s) {
	s % sub;
	if (s.IsLoading()) {
		for(int i = 0; i < sub.GetCount(); i++)
			sub[i].parent = this;
		parent = 0;
	}
}

hash_t Node::GetHashValue() const {
	CombineHash ch;
	ch << GetCount() << 1;
	for(int i = 0; i < sub.GetCount(); i++)
		ch << sub[i].GetHashValue() << (i+2);
	return ch;
}

int Node::GetDepth() {
	Node* ptr = this;
	for(int i = 0;; i++) {
		ptr = ptr->parent;
		if (!ptr)
			return i;
	}
}

void Node::RefreshParent() {
	for(int i = 0; i < sub.GetCount(); i++)
		sub[i].SetParent(*this).RefreshParent();
}

Vector<int> Node::GetPosition() const {
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

Node& Node::Seek(const Vector<int>& pos) {
	Node* ptr = this;
	for(int i = 0; i < pos.GetCount(); i++) {
		ptr = &ptr->At(pos[i]);
	}
	return *ptr;
}

Node& Node::AtTotal(int i) {
	if (i < sub.GetCount())
		return sub[i];
	return *links[i-sub.GetCount()];
}

const Node& Node::AtTotal(int i) const {
	if (i < sub.GetCount())
		return sub[i];
	return *links[i-sub.GetCount()];
}

int Node::GetTotalCount() const {
	return sub.GetCount() + links.GetCount();
}

void Node::RemoveChild(Node* child) {
	for(int i = 0; i < sub.GetCount(); i++) {
		if (&sub[i] == child) {
			sub.Remove(i);
			return;
		}
	}
}

Node* Node::DetachChild(Node* child) {
	int i = 0;
	for(Node& n : sub) {
		if (&n == child)
			return sub.Detach(i);
		i++;
	}
	return NULL;
}

void Node::AddLink(Node& n) {
	for(int i = 0; i < links.GetCount(); i++) {
		if ((int64)&links[i] == (int64)&n)
			return;
	}
	links.Add(&n);
}

String Node::AsString(int indent) {
	String s;
	for(int i = 0; i < indent; i++)
		s += "-";
	s += ": <TODO>\n";
	for(int i = 0; i < sub.GetCount(); i++) {
		s += sub[i].AsString(indent+1);
	}
	return s;
}


}
