#ifndef _SocialCtrl_CampaignInfoCtrl_h_
#define _SocialCtrl_CampaignInfoCtrl_h_


class SocialEditor;

class CampaignInfoCtrl : public WithProductInfo<ToolAppCtrl> {
	
	
public:
	typedef CampaignInfoCtrl CLASSNAME;
	CampaignInfoCtrl();
	
	void Data();
	void Clear();
	void OnValueChange();
	
	SocialEditor* editor = 0;
	
};


#endif
