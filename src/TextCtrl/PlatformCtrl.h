#ifndef _TextCtrl_PlatformCtrl_h_
#define _TextCtrl_PlatformCtrl_h_


BEGIN_TEXTLIB_NAMESPACE


class PlatformCtrl : public ToolAppCtrl {
	Splitter hsplit, vsplit;
	ArrayCtrl platforms, _;
	WithSocialPlatform<Ctrl> plat;
	
public:
	typedef PlatformCtrl CLASSNAME;
	PlatformCtrl();
	
	void Data() override;
	void DataPlatform();
	void ToolMenu(Bar& bar) override;
	void Do(int fn);
	
};


END_TEXTLIB_NAMESPACE


#endif
