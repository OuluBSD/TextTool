#ifndef _SimplifiedCtrl_StoryIdea_h_
#define _SimplifiedCtrl_StoryIdea_h_


class StoryIdeaCtrl : public SongToolCtrl {
	Splitter hsplit0, vsplit, hsplit1;
	ArrayCtrl list, parts;
	DocEdit story, part_story;
	
public:
	typedef StoryIdeaCtrl CLASSNAME;
	StoryIdeaCtrl();
	
	void DisableAll();
	void EnableAll();
	void Data() override;
	void DataSong();
	void DataStory();
	void DataPart();
	void ToolMenu(Bar& bar) override;
	void GetFirstStory(bool start_next);
	void GetStory(int i, bool start_next);
	void GetPartStory(int part_i, bool start_next);
	void OnStory(String res, int i, Song* song, bool start_next);
	void OnPart(String res, Song* song, int i, bool start_next);
	
};


#endif
