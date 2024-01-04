#ifndef _SongDataCtrl_Tokens_h_
#define _SongDataCtrl_Tokens_h_


class TokensPage : public SongToolCtrl {
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


#endif
