#ifndef _SongTool_AttrDataCtrl_h_
#define _SongTool_AttrDataCtrl_h_


class AttrDataCtrl : public Ctrl {
	Splitter split;
	ArrayCtrl groups, items;
	
public:
	typedef AttrDataCtrl CLASSNAME;
	AttrDataCtrl();
	
	void Data();
	void DataGroup();
	
	
};

#endif
