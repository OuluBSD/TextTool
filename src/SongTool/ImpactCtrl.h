#ifndef _SongTool_ImpactCtrl_h_
#define _SongTool_ImpactCtrl_h_


class ImpactCtrl : public Ctrl {
	Splitter vsplit;
	ArrayCtrl list[GENDER_COUNT];
	
public:
	typedef ImpactCtrl CLASSNAME;
	ImpactCtrl();
	
	void SelectLine(int match);
	void Data();
	
	
};


#endif
