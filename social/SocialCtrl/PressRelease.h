#ifndef _SocialCtrl_PressCampaign_h_
#define _SocialCtrl_PressCampaign_h_


class PressCampaignCtrl : public ToolAppCtrl {
	
	
public:
	typedef PressCampaignCtrl CLASSNAME;
	PressCampaignCtrl();
	
	void Data() override;
	void ToolMenu(Bar& bar) override;
	
};


#endif
