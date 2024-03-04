#ifndef _ToolCtrl_FrontPage_h_
#define _ToolCtrl_FrontPage_h_


class FrontPage : public WithFrontPage<Ctrl> {
	
	
public:
	typedef FrontPage CLASSNAME;
	FrontPage();
	
	void Data();
	void ToolMenu(Bar& bar) {}
	
};

#endif
