#ifndef _SimplifiedCtrl_VisualIdea_h_
#define _SimplifiedCtrl_VisualIdea_h_


class VisualIdeaCtrl : public SongToolCtrl {
	Splitter hsplit, vsplit;
	ArrayCtrl parts;
	DocEdit part_characters, part_story;
	
public:
	typedef VisualIdeaCtrl CLASSNAME;
	VisualIdeaCtrl();
	
	void DisableAll();
	void EnableAll();
	void Data() override;
	void ToolMenu(Bar& bar) override;
	void GetPartVisualIdea(int part_i, bool start_next);
	void OnPartVisualIdea(String res, Song* song, int i, bool start_next);
	void GetPartVisualIdeaCharacters(int part_i, bool start_next);
	void OnPartVisualIdeaCharacters(String res, Song* song, int i, bool start_next);
	void DataSong();
	void DataPart();
	void GetFirstStory(bool start_next);
	
};


#endif
