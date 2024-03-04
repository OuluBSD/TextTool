#ifndef _SongCtrl_Marketing_h_
#define _SongCtrl_Marketing_h_


class MarketingCtrl : public ToolAppCtrl {
	
	
public:
	typedef MarketingCtrl CLASSNAME;
	MarketingCtrl();
	
	void Data() override;
	void ToolMenu(Bar& bar) override;
	
};


#endif
