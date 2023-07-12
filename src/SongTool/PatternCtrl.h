#ifndef _SongTool_PatternCtrl_h_
#define _SongTool_PatternCtrl_h_


class PatternCtrl : public Ctrl {
	Splitter hsplit, vsplit0, vsplit1;
	TreeCtrl tree;
	ArrayCtrl list;
	AttrCtrl attr;
	
	VectorMap<int, PatternSnap*> tree_snaps;
	Vector<PatternSnap*> level_snaps;
	
public:
	typedef PatternCtrl CLASSNAME;
	PatternCtrl();
	
	void Data();
	void DataPatternTree();
	void DataPatternTreeNode(PatternSnap& snap, int parent);
	void OnTreeSel();
	void OnListSel();
	void DataPatternSnap();
	void DataList();
	void MergeOwner();
	void FocusTree();
	void FocusList();
	
};


#endif
