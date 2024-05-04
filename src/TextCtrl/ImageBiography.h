#ifndef _TextCtrl_ImageBiography_h_
#define _TextCtrl_ImageBiography_h_


BEGIN_TEXTLIB_NAMESPACE


class ImageBiographyCtrl : public ToolAppCtrl {
	/*Splitter hsplit, vsplit;
	ArrayCtrl platforms, entries;
	WithSocialHeader<Ctrl> entry;
	SocialTimelineCtrl timeline;*/
	
public:
	typedef ImageBiographyCtrl CLASSNAME;
	ImageBiographyCtrl();
	
	void Data() override;
	void DataPlatform();
	void ToolMenu(Bar& bar) override;
	void EntryListMenu(Bar& bar);
	
	
};


END_TEXTLIB_NAMESPACE


#endif
