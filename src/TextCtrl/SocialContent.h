#ifndef _TextCtrl_SocialContent_h_
#define _TextCtrl_SocialContent_h_


BEGIN_TEXTLIB_NAMESPACE


class SocialContent : public ToolAppCtrl {
	Splitter hsplit, vsplit, menusplit, threadsplit;
	ArrayCtrl platforms, threads, entries;
	WithSocialEntry<Ctrl> entry;
	SocialTimelineCtrl timeline;
	
public:
	typedef SocialContent CLASSNAME;
	SocialContent();
	
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
	void Do(int fn);
	
};


END_TEXTLIB_NAMESPACE


#endif
