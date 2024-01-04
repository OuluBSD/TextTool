#ifndef _SongDataCtrl_TokenPhrases_h_
#define _SongDataCtrl_TokenPhrases_h_


class TokenPhrases : public SongToolCtrl {
	Splitter vsplit, hsplit;
	ArrayCtrl datasets, texts;
	
public:
	typedef TokenPhrases CLASSNAME;
	TokenPhrases();
	
	void Data() override;
	void DataDataset();
	void ToolMenu(Bar& bar) override;
	void GetUnknownPairs();
	
	
};


#endif
