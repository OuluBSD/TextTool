#ifndef _SongTool_MaskScoringCtrl_h_
#define _SongTool_MaskScoringCtrl_h_


class MaskScoringCtrl : public Ctrl {
	Plotter plotter[GENDER_COUNT];
	ArrayCtrl list[GENDER_COUNT];
	Splitter vsplit[GENDER_COUNT], mainsplit;
	
	void ListMenu(Bar& bar);
public:
	typedef MaskScoringCtrl CLASSNAME;
	MaskScoringCtrl();
	
	void Data();
	void DataListAll() {for (int mode = 0; mode < GENDER_COUNT; mode++) DataList(mode);}
	void DataList(int mode);
	void ListValueChanged(int mode, int pos, int scoring);
	
	static const int group_begin = 4;
	
};


#endif
