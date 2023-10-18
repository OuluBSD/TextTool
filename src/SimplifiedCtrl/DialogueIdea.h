#ifndef _SimplifiedCtrl_DialogueIdea_h_
#define _SimplifiedCtrl_DialogueIdea_h_


class DialogueIdeaCtrl : public SongToolCtrl {
	Splitter hsplit, vsplit;
	ArrayCtrl parts;
	DocEdit part_dialogue0, part_dialogue1;
	
public:
	typedef DialogueIdeaCtrl CLASSNAME;
	DialogueIdeaCtrl();
	
	void DisableAll();
	void EnableAll();
	void Data() override;
	void ToolMenu(Bar& bar) override;
	void GetPartDialogueIdea(int part_i, int prio_i, bool start_next);
	void OnPartDialogueIdea(String res, Song* song, int i, int prio_i, bool start_next);
	void DataSong();
	void DataPart();
	void GetFirstStory(bool start_next);
	void OnDialogueStyle(StaticPart* part, int prio_i, DropList* dl);
	void GetStyleSuggestions(int part_i, bool start_next);
	void OnPartDialogueIdeaStyleSuggestions(String res, Song* song, int i, bool start_next);
	
};


#endif
