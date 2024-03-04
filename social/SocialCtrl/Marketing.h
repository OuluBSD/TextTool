#ifndef _SocialCtrl_Marketing_h_
#define _SocialCtrl_Marketing_h_


class MarketingCtrl : public ToolAppCtrl {
	
	
public:
	typedef MarketingCtrl CLASSNAME;
	MarketingCtrl();
	
	void Data() override;
	void ToolMenu(Bar& bar) override;
	
};


#endif
