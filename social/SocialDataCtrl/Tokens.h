#ifndef _SocialDataCtrl_Tokens_h_
#define _SocialDataCtrl_Tokens_h_


BEGIN_SOCIALLIB_NAMESPACE


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


END_SOCIALLIB_NAMESPACE


#endif
