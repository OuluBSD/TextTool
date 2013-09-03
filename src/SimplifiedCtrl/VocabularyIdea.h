#ifndef _SimplifiedCtrl_VocabularyIdea_h_
#define _SimplifiedCtrl_VocabularyIdea_h_


class VocabularyIdeaCtrl : public LineListCtrl {
	Splitter hsplit;
	ArrayCtrl local, vocab;
	
	int phrase_limit = 20;
	
public:
	typedef VocabularyIdeaCtrl CLASSNAME;
	VocabularyIdeaCtrl();
	
	void DisableAll();
	void EnableAll();
	void Data() override;
	void ToolMenu(Bar& bar) override;
	void OnList();
	void DataLocal();
	void DataResult();
	
	void GetLineIdea(int row, bool start_next);
	void OnIdeaResult(String result, Song* song, int list_i, bool start_next);
	
};


#endif
