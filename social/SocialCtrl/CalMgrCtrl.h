#ifndef _SocialTool_CalMgrCtrl_h_
#define _SocialTool_CalMgrCtrl_h_


BEGIN_SOCIALLIB_NAMESPACE


class CompanyCalendarCtrl : public Ctrl {
	Color bg;
	Date scroll_date;
	Date cursor_date;
	int month_offset, month_len;
	
public:
	typedef CompanyCalendarCtrl CLASSNAME;
	CompanyCalendarCtrl();
	
	void Paint(Draw& d);
	
	void SetScroll(Date d);
	
	
public:
	
	
};


class CalMgrCtrl : public ToolAppCtrl {
	Splitter split;
	TabCtrl tabs;
	CompanyCalendarCtrl cal;
	ArrayCtrl events;
	
public:
	typedef CalMgrCtrl CLASSNAME;
	CalMgrCtrl();
	
	void Data() override;
	void ToolMenu(Bar& bar) override;
	void ImportAutomatically();
	
};


END_SOCIALLIB_NAMESPACE


#endif
