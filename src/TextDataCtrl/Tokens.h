#ifndef _TextDataCtrl_Tokens_h_
#define _TextDataCtrl_Tokens_h_


BEGIN_TEXTLIB_NAMESPACE


class TokensPage : public ToolAppCtrl {
	Splitter hsplit;
	ArrayCtrl tokens;
	
public:
	typedef TokensPage CLASSNAME;
	TokensPage();
	
	void Data() override;
	void ToolMenu(Bar& bar) override;
	void Do(int fn);
	
	
};


END_TEXTLIB_NAMESPACE


#endif
