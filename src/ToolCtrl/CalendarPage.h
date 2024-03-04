#ifndef _ToolCtrl_CalendarPage_h_
#define _ToolCtrl_CalendarPage_h_


class CalendarPage : public WithCalendar<Ctrl> {
	
	
public:
	typedef CalendarPage CLASSNAME;
	CalendarPage();
	
	void Data();
	void ToolMenu(Bar& bar) {}
	
	
};


#endif
