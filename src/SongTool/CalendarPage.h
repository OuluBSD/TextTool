#ifndef _SongTool_CalendarPage_h_
#define _SongTool_CalendarPage_h_


class CalendarPage : public WithCalendar<Ctrl> {
	
	
public:
	typedef CalendarPage CLASSNAME;
	CalendarPage();
	
	void Data();
	void ToolMenu(Bar& bar) {}
	
	
};


#endif
