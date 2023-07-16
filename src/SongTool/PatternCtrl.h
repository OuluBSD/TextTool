#ifndef _SongTool_PatternCtrl_h_
#define _SongTool_PatternCtrl_h_


class PatternCtrl : public Ctrl {
	Label txt;
	Splitter hsplit, vsplit0, vsplit1;
	TreeCtrl tree;
	One<ArrayCtrl> list;
	Ctrl list_placeholder;
	AttrCtrl attr;
	
public:
	VectorMap<int, PatternSnap*> tree_snaps;
	VectorMap<int, Part*> tree_parts;
	Vector<PatternSnap*> level_snaps;
	Index<int> group_types;
	VectorMap<int, Vector<int>> group_items;
	
public:
	typedef PatternCtrl CLASSNAME;
	PatternCtrl();
	
	void Data();
	void DataPatternTree();
	void OnTreeSel();
	void OnListSel();
	void DataPatternSnap();
	void DataList();
	void MergeOwner();
	void FocusTree();
	void FocusList();
	
	template <class T>
	void DataPatternTreeNode(Part& part, T& snap, int parent) {
		tree_snaps.Add(parent, &snap);
		tree_parts.Add(parent, &part);
		
		for (auto& sub : snap.GetSub()) {
			String s = IntStr(sub.id);
			int id = tree.Add(parent, AppImg::Snap(), s);
			DataPatternTreeNode(part, sub, id);
		}
	}

	bool use_rev_snap = false;
	
};


#endif
