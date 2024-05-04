#ifndef _TextCtrl_SocialHeader_h_
#define _TextCtrl_SocialHeader_h_


BEGIN_TEXTLIB_NAMESPACE


class SocialHeaderCtrl : public ToolAppCtrl {
	Splitter hsplit, vsplit;
	ArrayCtrl platforms, entries;
	WithSocialHeader<Ctrl> entry;
	SocialTimelineCtrl timeline;
	
public:
	typedef SocialHeaderCtrl CLASSNAME;
	SocialHeaderCtrl();
	
	void Data() override;
	void DataPlatform();
	void ToolMenu(Bar& bar) override;
	void EntryListMenu(Bar& bar);
	
	
};


END_TEXTLIB_NAMESPACE

#endif
