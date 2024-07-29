#ifndef _TextCtrl_SocialContentTemplates_h_
#define _TextCtrl_SocialContentTemplates_h_

#if 0
BEGIN_TEXTLIB_NAMESPACE


class SocialContentTemplateCtrl : public ToolAppCtrl {
	Splitter hsplit, vsplit, menusplit, threadsplit;
	ArrayCtrl platforms, threads, entries, campaign_tmpls, campaign_params, tmpl_params	;
	WithSocialGeneratedEntry<Ctrl> entry;
	TabCtrl tabs;
	
public:
	typedef SocialContentTemplateCtrl CLASSNAME;
	SocialContentTemplateCtrl();
	
	void Data() override;
	void DataPlatform();
	void DataThread();
	void DataEntry();
	void Clear();
	void ClearEntry();
	void OnValueChange();
	void AddEntry();
	void RemoveEntry();
	void AddThread();
	void RemoveThread();
	void ToolMenu(Bar& bar) override;
	void ThreadListMenu(Bar& bar);
	void EntryListMenu(Bar& bar);
	
	
};


END_TEXTLIB_NAMESPACE


#endif
#endif
