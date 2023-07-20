#ifndef _SongTool_ImpactCtrl_h_
#define _SongTool_ImpactCtrl_h_


class ImpactCtrl : public Ctrl {
	Splitter vsplit;
	ArrayCtrl list[GENDER_COUNT];
	
public:
	typedef ImpactCtrl CLASSNAME;
	ImpactCtrl();
	
	void Data();
	void SelectLine(int match);
	
	
};


#endif
