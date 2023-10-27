#ifndef _SimplifiedCtrl_WordSaladIdea_h_
#define _SimplifiedCtrl_WordSaladIdea_h_


class WordSaladIdeaCtrl : public SongToolCtrl {
	Splitter hsplit;
	ArrayCtrl list, vocab, sentences;
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
	
	Vector<Vector<String>> GetVocabulary(int row);
	Vector<Vector<Color>> GetColors(int row);
	Vector<Color> GetListenerColors(int row);
	Vector<Vector<Color>> GetPreviousListenerColors(int row);
	void GetPossibleParts(Song& s, StaticPart& part, Index<String>& possible_previous, Index<String>& possible_next);
	
};


#endif
