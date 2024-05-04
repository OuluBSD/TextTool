#ifndef _TextCtrl_SocialContentTemplates_h_
#define _TextCtrl_SocialContentTemplates_h_


BEGIN_TEXTLIB_NAMESPACE


class SocialContentTemplateCtrl : public ToolAppCtrl {
	Splitter hsplit, vsplit, menusplit;
	ArrayCtrl platforms, entries, campaigns, campaign_tmpls, campaign_params, tmpl_params	;
	WithSocialGeneratedEntry<Ctrl> entry;
	SocialTimelineCtrl timeline;
	TabCtrl tabs;
	
public:
	typedef SocialContentTemplateCtrl CLASSNAME;
	SocialContentTemplateCtrl();
	
	void Data() override;
	void DataPlatform();
	void DataEntry();
	void Clear();
	void ClearEntry();
	void OnValueChange();
	void AddEntry();
	void RemoveEntry();
	void ToolMenu(Bar& bar) override;
	void EntryListMenu(Bar& bar);
	
	
};


END_TEXTLIB_NAMESPACE


#endif
