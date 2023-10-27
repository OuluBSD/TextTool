#ifndef _SimplifiedCtrl_WordSaladIdea_h_
#define _SimplifiedCtrl_WordSaladIdea_h_


class WordSaladIdeaCtrl : public LineListCtrl {
	Splitter hsplit;
	ArrayCtrl vocab, sentences;
	DocEdit part_dialogue0, part_dialogue1;
	
public:
	typedef WordSaladIdeaCtrl CLASSNAME;
	WordSaladIdeaCtrl();
	
	void DisableAll();
	void EnableAll();
	void Data() override;
	void ToolMenu(Bar& bar) override;
	void OnList();
	void DataVocabulary();
	void DataSentences();
	void GetLineSentence(int row, bool start_next);
	void OnLineSentence(String result, Song* song, int list_i, bool start_next);
	
};


#endif
