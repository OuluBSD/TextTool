#ifndef _SongTool_ImpactScoringCtrl_h_
#define _SongTool_ImpactScoringCtrl_h_


class ImpactScoringCtrl : public Ctrl {
	Plotter plotter[GENDER_COUNT];
	ArrayCtrl list[GENDER_COUNT];
	Splitter vsplit[GENDER_COUNT], mainsplit;
	
	void ListMenu(Bar& bar);
public:
	typedef ImpactScoringCtrl CLASSNAME;
	ImpactScoringCtrl();
	
	void Data();
	void DataListAll() {for (int mode = 0; mode < GENDER_COUNT; mode++) DataList(mode);}
	void DataList(int mode);
	void ListValueChanged(int mode, int pos, int scoring);
	
	static const int group_begin = 4;
	
};


#endif
