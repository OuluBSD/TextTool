#ifndef _SimplifiedCtrl_ColorIdea_h_
#define _SimplifiedCtrl_ColorIdea_h_


class ColorIdeaCtrl : public SongToolCtrl {
	ArrayCtrl list;
	
public:
	typedef ColorIdeaCtrl CLASSNAME;
	ColorIdeaCtrl();
	
	void DisableAll();
	void EnableAll();
	void Data() override;
	void ToolMenu(Bar& bar) override;
	
	void GetMain(int part_i, bool start_next);
	void GetEnvelope(int part_i, bool start_next);
	void GetListener(int part_i, bool start_next);
	void OnMain(String res, Song* song, int part_i, bool start_next);
	void OnEnvelope(String res, Song* song, int part_i, bool start_next);
	void OnListener(String res, Song* song, int part_i, bool start_next);
	int FindListIndex(int part, int line) const;
	void GetPossibleParts(Song& song, StaticPart& part, Index<String>& possible_previous, Index<String>& possible_next);
	
};


#endif
