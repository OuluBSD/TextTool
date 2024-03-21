#ifndef _TextDataCtrl_TextDataWordnetScoring_h_
#define _TextDataCtrl_TextDataWordnetScoring_h_


#if 0

class TextDataWordnetScoring : public ToolAppCtrl {
	Splitter vsplit, hsplit, vsplit1;
	ArrayCtrl datasets, attrs, colors, wordnets, clr_wordnets;
	Mutex lock;
	
	Vector<Wordnet*> tmp_wordnets;
	Vector<ColorWordnet*> tmp_clr_wordnets;
	
	bool disabled = false;
	bool running = false;
	int per_batch = 40;
	bool batch = false;
public:
	typedef TextDataWordnetScoring CLASSNAME;
	TextDataWordnetScoring();
	
	void EnableAll();
	void DisableAll();
	void Data() override;
	void DataMain();
	void DataDataset();
	void DataAttribute();
	void DataColor();
	void ToolMenu(Bar& bar) override;
	
	void ToggleGettingWordnetScores(int score_mode);
	void GetWordnetScores(int batch_i, int score_mode);
	void OnWordnetScores(String res, int batch_i, int score_mode);
	
	void ToggleGettingColorWordnetScores(int score_mode);
	void GetColorWordnetScores(int batch_i, int score_mode);
	void OnColorWordnetScores(String res, int batch_i, int score_mode);
	
};


#endif
#endif

