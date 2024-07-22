#ifndef _TextDataCtrl_TokenPhrases_h_
#define _TextDataCtrl_TokenPhrases_h_


BEGIN_TEXTLIB_NAMESPACE


class TokenPhrases : public ToolAppCtrl {
	Splitter hsplit;
	ArrayCtrl texts;
	
public:
	typedef TokenPhrases CLASSNAME;
	TokenPhrases();
	
	void Data() override;
	void ToolMenu(Bar& bar) override;
	void Do(int fn);
	
	
};


END_TEXTLIB_NAMESPACE


#endif
