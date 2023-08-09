#ifndef _SongTool_PatternCtrl_h_
#define _SongTool_PatternCtrl_h_


class PatternCtrl : public Ctrl {
	Label txt;
	One<ArrayCtrl> list;
	Ctrl ctrl;
	AttrCtrl attr;
	SnapArg a;
	
public:
	
	Vector<PatternSnap*> level_snaps;
	Index<int> group_types;
	VectorMap<int, Vector<int>> group_items;
	
public:
	typedef PatternCtrl CLASSNAME;
	PatternCtrl();
	
	void Data();
	void OnListSel();
	void DataPatternSnap();
	void DataList();
	void MergeOwner();
	void FocusList();
	void SelectLine(const SnapArg& match);
	void SetMode(const SnapArg& a);
	
	Callback WhenFocusTree;
	
};


class PatternList : public Ctrl {
	MArr<ArrayCtrl> data;
	MArr<DocEdit> lyrics;
	MArr<Splitter> vsplit;
	Splitter hsplit;
	
public:
	typedef PatternList CLASSNAME;
	PatternList();
	
	void Data();
	void SelectLine(const SnapArg& match);
	
	PipePtrs p;
	
};

class PatternView : public Ctrl {
	Splitter hsplit;
	TreeCtrl tree;
	TabCtrl tabs;
	PatternCtrl pattern[MODE_COUNT];
	PatternList list;
	
public:
	VectorMap<int, SnapContext*> tree_snaps;
	VectorMap<int, Part*> tree_parts;
	SnapArg a[1+MODE_COUNT];
	
public:
	typedef PatternView CLASSNAME;
	PatternView();
	
	void Data();
	void UseRev();
	void DataPatternTree();
	void OnTreeSel();
	void FocusTree();
	
	template <class T>
	void DataPatternTreeNode(Part& part, T& snap, int parent) {
		tree_snaps.Add(parent, &snap);
		tree_parts.Add(parent, &part);
		
		for (auto& sub : snap.GetSub()) {
			String s = IntStr(sub.Get0().id);
			int id = tree.Add(parent, AppImg::Snap(), s);
			DataPatternTreeNode(part, sub, id);
		}
	}
	
};


#endif
