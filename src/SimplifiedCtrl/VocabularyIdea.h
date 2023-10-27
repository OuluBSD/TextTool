#ifndef _SimplifiedCtrl_VocabularyIdea_h_
#define _SimplifiedCtrl_VocabularyIdea_h_


class VocabularyIdeaCtrl : public SongToolCtrl {
	Splitter hsplit;
	ArrayCtrl list, local, vocab;
	
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
	
	Vector<String> GetLineVisualIdea(int row);
	Vector<String> GetLineCharactersIdea(int row);
	Vector<String> GetLineDialogueIdea1(int row);
	Vector<String> GetLineDialogueIdea2(int row);
	Vector<Vector<String>> GetLinePhraseIdea(int row, bool colors);
	Vector<String> GetPartData(int row, String key);
	Vector<Color> GetLineColorIdea(int row);
	
	Vector<String> FindLineData(int row, Vector<String> lines);
	
};


#endif
