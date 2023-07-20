#ifndef _SongTool_StoryCtrl_h_
#define _SongTool_StoryCtrl_h_


class StoryCtrl : public Ctrl {
	Splitter vsplit;
	ArrayCtrl list[2];
	
public:
	typedef StoryCtrl CLASSNAME;
	StoryCtrl();
	
	void SelectLine(int match);
	void Data();
	
	
};


#endif
