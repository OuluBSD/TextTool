#ifndef _TextDataCtrl_Tokens_h_
#define _TextDataCtrl_Tokens_h_


BEGIN_TEXTLIB_NAMESPACE


class TokensPage : public ToolAppCtrl {
	Splitter vsplit, hsplit;
	ArrayCtrl datasets, tokens;
	
public:
	typedef TokensPage CLASSNAME;
	TokensPage();
	
	void Data() override;
	void DataDataset();
	void ToolMenu(Bar& bar) override;
	void ProcessTokens();
	
	
};


END_TEXTLIB_NAMESPACE


#endif
