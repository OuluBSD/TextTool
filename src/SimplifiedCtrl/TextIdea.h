#ifndef _SimplifiedCtrl_TextIdea_h_
#define _SimplifiedCtrl_TextIdea_h_


class TextIdeaCtrl : public LineListCtrl {
	Splitter hsplit;
	ArrayCtrl texts;
	
public:
	typedef TextIdeaCtrl CLASSNAME;
	TextIdeaCtrl();
	
	void DisableAll();
	void EnableAll();
	void Data() override;
	void DataPart();
	void ToolMenu(Bar& bar) override;
	void GetPartTexts(int row, bool start_next);
	void OnPartTexts(String result, Song* song, int list_i, bool start_next);
	
};


#endif
