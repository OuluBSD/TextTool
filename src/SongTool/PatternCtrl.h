#ifndef _SongTool_PatternCtrl_h_
#define _SongTool_PatternCtrl_h_


class PatternCtrl : public Ctrl {
	Splitter hsplit, vsplit;
	TreeCtrl tree;
	Label txt[GENDER_COUNT];
	One<ArrayCtrl> list[GENDER_COUNT];
	Ctrl ctrl[GENDER_COUNT];
	AttrCtrl attr[GENDER_COUNT];
	
public:
	VectorMap<int, SnapContext*> tree_snaps;
	VectorMap<int, Part*> tree_parts;
	
	Vector<PatternSnap*> level_snaps[GENDER_COUNT];
	Index<int> group_types[GENDER_COUNT];
	VectorMap<int, Vector<int>> group_items[GENDER_COUNT];
	
public:
	typedef PatternCtrl CLASSNAME;
	PatternCtrl();
	
	void Data();
	void DataPatternTree();
	void OnTreeSel();
	void OnListSel(int mode);
	void DataPatternSnap(int mode);
	void DataPatternSnapAll() {for(int mode = 0; mode < GENDER_COUNT; mode++) DataPatternSnap(mode);}
	void DataList(int mode);
	void DataListAll() {for(int mode = 0; mode < GENDER_COUNT; mode++) DataList(mode);}
	void MergeOwner();
	void FocusTree(int mode);
	void FocusList(int mode);
	void FocusListAll() {for(int mode = 0; mode < GENDER_COUNT; mode++) FocusList(mode);}
	void SelectLine(int match);
	
	template <class T>
	void DataPatternTreeNode(Part& part, T& snap, int parent) {
		tree_snaps.Add(parent, &snap);
		tree_parts.Add(parent, &part);
		
		for (auto& sub : snap.GetSub()) {
			String s = IntStr(sub.snap[0].id);
			int id = tree.Add(parent, AppImg::Snap(), s);
			DataPatternTreeNode(part, sub, id);
		}
	}

	bool use_rev_snap = false;
	
};


#endif
