#ifndef _SimplifiedCtrl_StoryIdea_h_
#define _SimplifiedCtrl_StoryIdea_h_


class StoryIdeaCtrl : public SongToolCtrl {
	Splitter split;
	ArrayCtrl list;
	DocEdit story;
	
public:
	typedef StoryIdeaCtrl CLASSNAME;
	StoryIdeaCtrl();
	
	void DisableAll();
	void EnableAll();
	void Data() override;
	void DataStory();
	void ToolMenu(Bar& bar) override;
	void GetFirstStory(bool start_next);
	void GetStory(int i, bool start_next);
	void OnStory(String res, int i, Song* song, bool start_next);
	
};


#endif
