#ifndef _SongTool_MaskScoringCtrl_h_
#define _SongTool_MaskScoringCtrl_h_


class MaskScoringCtrl : public Ctrl {
	PArr<Plotter> plotter;
	PArr<ArrayCtrl> list;
	PArr<Splitter> vsplit;
	Splitter mainsplit;
	
	void ListMenu(Bar& bar);
public:
	typedef MaskScoringCtrl CLASSNAME;
	MaskScoringCtrl();
	
	void Data();
	void DataListAll() {for(const SnapArg& a : SnapArgs()) DataList(a);}
	void DataList(const SnapArg& a);
	void ListValueChanged(const SnapArg& a, int pos, int scoring);
	
	static const int group_begin = 4;
	
};


#endif
