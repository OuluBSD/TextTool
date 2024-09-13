#ifndef _ToolCtrl_FrontPage_h_
#define _ToolCtrl_FrontPage_h_


BEGIN_TEXTLIB_NAMESPACE


class FrontPage : public WithFrontPage<Ctrl> {
	
	
public:
	typedef FrontPage CLASSNAME;
	FrontPage();
	
	void Data();
	void ToolMenu(Bar& bar) {}
	
};


END_TEXTLIB_NAMESPACE


#endif
