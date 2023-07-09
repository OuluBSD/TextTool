#ifndef _SongTool_StoryCtrl_h_
#define _SongTool_StoryCtrl_h_


class StoryCtrl : public WithStory<Ctrl> {
	
	
public:
	typedef StoryCtrl CLASSNAME;
	StoryCtrl();
	
	void Data();
	void SaveStory();
	
	
};

#endif
