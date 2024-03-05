#ifndef _SocialTool_CampaignIdeas_h_
#define _SocialTool_CampaignIdeas_h_


BEGIN_SOCIALLIB_NAMESPACE


class CampaignIdeas : public ToolAppCtrl {
	Splitter hsplit;
	ArrayCtrl list;
	WithCampaignIdeas<Ctrl> idea;
	
	
public:
	typedef CampaignIdeas CLASSNAME;
	CampaignIdeas();
	
	void Data();
	void Clear();
	void IdeaData();
	void OnListMenu(Bar& bar);
	void AddIdea();
	void RemoveIdea();
	void OnValueChange();
	
};


END_SOCIALLIB_NAMESPACE


#endif
