#ifndef _TextCtrl_PlatformCtrl_h_
#define _TextCtrl_PlatformCtrl_h_


BEGIN_TEXTLIB_NAMESPACE


class PlatformCtrl : public ToolAppCtrl {
	Splitter hsplit, vsplit, bottom;
	TabCtrl plat_tabs, epk_tabs;
	ArrayCtrl roles, platforms, epk_text_fields, epk_photo_types;
	WithSocialPlatform<Ctrl> plat;
	
public:
	typedef PlatformCtrl CLASSNAME;
	PlatformCtrl();
	
	void Data() override;
	void DataPlatform();
	void ToolMenu(Bar& bar) override;
	void PlatformMenu(Bar& bar);
	void Do(int fn);
	void SetSorting(int col);
	
};


END_TEXTLIB_NAMESPACE


#endif
