#ifndef _SocialCtrl_Marketing_h_
#define _SocialCtrl_Marketing_h_


BEGIN_SOCIALLIB_NAMESPACE


class MarketingCtrl : public ToolAppCtrl {
	
	
public:
	typedef MarketingCtrl CLASSNAME;
	MarketingCtrl();
	
	void Data() override;
	void ToolMenu(Bar& bar) override;
	
};


END_SOCIALLIB_NAMESPACE


#endif
