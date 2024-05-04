#ifndef _TextCtrl_SocialContent_h_
#define _TextCtrl_SocialContent_h_


BEGIN_TEXTLIB_NAMESPACE


class SocialContent : public ToolAppCtrl {
	Splitter hsplit, vsplit, menusplit;
	ArrayCtrl platforms, entries, campaigns;
	WithSocialEntry<Ctrl> entry;
	SocialTimelineCtrl timeline;
	
public:
	typedef SocialContent CLASSNAME;
	SocialContent();
	
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
