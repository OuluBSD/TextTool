#ifndef _SocialCtrl_PressCampaign_h_
#define _SocialCtrl_PressCampaign_h_


BEGIN_SOCIALLIB_NAMESPACE


class PressCampaignCtrl : public ToolAppCtrl {
	
	
public:
	typedef PressCampaignCtrl CLASSNAME;
	PressCampaignCtrl();
	
	void Data() override;
	void ToolMenu(Bar& bar) override;
	
};


END_SOCIALLIB_NAMESPACE


#endif
