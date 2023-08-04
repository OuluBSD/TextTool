#ifndef _SongTool_StoryCtrl_h_
#define _SongTool_StoryCtrl_h_


class StoryCtrl : public Ctrl {
	Splitter vsplit;
	MArr<ArrayCtrl> list;
	
public:
	typedef StoryCtrl CLASSNAME;
	StoryCtrl();
	
	void SelectLine(const SnapArg& match);
	void Data();
	
	
};


#endif
