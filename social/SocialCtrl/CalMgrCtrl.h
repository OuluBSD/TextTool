#ifndef _SongTool_CalMgrCtrl_h_
#define _SongTool_CalMgrCtrl_h_


class ArtistCalendarCtrl : public Ctrl {
	Color bg;
	Date scroll_date;
	Date cursor_date;
	int month_offset, month_len;
	
public:
	typedef ArtistCalendarCtrl CLASSNAME;
	ArtistCalendarCtrl();
	
	void Paint(Draw& d);
	
	void SetScroll(Date d);
	
	
public:
	
	
};


class CalMgrCtrl : public ToolAppCtrl {
	Splitter split;
	TabCtrl tabs;
	ArtistCalendarCtrl cal;
	ArrayCtrl events;
	
public:
	typedef CalMgrCtrl CLASSNAME;
	CalMgrCtrl();
	
	void Data() override;
	void ToolMenu(Bar& bar) override;
	void ImportAutomatically();
	
};

#endif
