#ifndef _SongTool_ImpactCtrl_h_
#define _SongTool_ImpactCtrl_h_


class ImpactCtrl : public Ctrl {
	Splitter vsplit;
	PArr<ArrayCtrl> list;
	
public:
	typedef ImpactCtrl CLASSNAME;
	ImpactCtrl();
	
	void Data();
	void SelectLine(const SnapArg& match);
	
	
};


#endif
