#ifndef _SongTool_PatternCtrl_h_
#define _SongTool_PatternCtrl_h_


class PatternCtrl : public Ctrl {
	Splitter hsplit, vsplit;
	TreeCtrl tree;
	MArr<Label> txt;
	MArr<One<ArrayCtrl>> list;
	MArr<Ctrl> ctrl;
	MArr<AttrCtrl> attr;
	
public:
	VectorMap<int, SnapContext*> tree_snaps;
	VectorMap<int, Part*> tree_parts;
	
	MArr<Vector<PatternSnap*>> level_snaps;
	MArr<Index<int>> group_types;
	MArr<VectorMap<int, Vector<int>>> group_items;
	
public:
	typedef PatternCtrl CLASSNAME;
	PatternCtrl();
	
	void Data();
	void DataPatternTree();
	void OnTreeSel();
	void OnListSel(const SnapArg& a);
	void DataPatternSnap(const SnapArg& a);
	void DataPatternSnapAll() {for(const SnapArg& a : AllArgs()) DataPatternSnap(a);}
	void DataList(const SnapArg& a);
	void DataListAll() {for(const SnapArg& a : AllArgs()) DataList(a);}
	void MergeOwner();
	void FocusTree(const SnapArg& a);
	void FocusList(const SnapArg& a);
	void FocusListAll() {for(const SnapArg& a : AllArgs()) FocusList(a);}
	void SelectLine(const SnapArg& match);
	
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

	bool use_rev_snap = false;
	
};


#endif
