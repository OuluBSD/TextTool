#ifndef _TextDataCtrl_TokenPhrases_h_
#define _TextDataCtrl_TokenPhrases_h_


BEGIN_TEXTLIB_NAMESPACE


class TokenPhrases : public ToolAppCtrl {
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


END_TEXTLIB_NAMESPACE


#endif
