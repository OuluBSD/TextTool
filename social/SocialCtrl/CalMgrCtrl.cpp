#include "SocialCtrl.h"


CalMgrCtrl::CalMgrCtrl() {
	Add(split.SizePos());
	
	split.Vert() << cal << tabs;
	
	tabs.Add(events, t_("Events"));
	
	split.SetPos(8000);
	
	
}

void CalMgrCtrl::Data() {
	
}

void CalMgrCtrl::ToolMenu(Bar& bar) {
	bar.Add(t_("Import automatically"), AppImg::import_(), THISBACK(ImportAutomatically))
	   .Help(t_("Import calendar events automatically from artist's catalogue"))
	   .Key(K_F5);
}

void CalMgrCtrl::ImportAutomatically() {
	
}














ArtistCalendarCtrl::ArtistCalendarCtrl() {
	bg = Color(255, 241, 241);
	scroll_date = GetSysTime();
	cursor_date = scroll_date;
	SetScroll(scroll_date);
}

void ArtistCalendarCtrl::Paint(Draw& d) {
	Size sz = GetSize();
	Date today = GetSysTime();
	
	Color line = GrayColor();
	Color today_bg = White();
	
	Font fnt = SansSerif(20);
	
	d.DrawRect(sz, bg);
	
	int visible_days = month_offset + month_len;
	int visible_weeks = visible_days / 7 + (visible_days % 7 ? 1 : 0);
	
	int row = sz.cy / visible_weeks;
	int col = sz.cx / 7;
	
	int day = -month_offset + 1;
	for(int i = 0; i < visible_weeks; i++) {
		int y = i * row;
		d.DrawLine(0, y, sz.cx, y, 1, line);
		y++;
		for(int j = 0; j < 7; j++) {
			Date date = scroll_date;
			date.day = day % month_len + 1;
			date.month = scroll_date.month + (day >= month_len ? 1 : 0);
			
			int x = j * col;
			d.DrawLine(x, 0, x, sz.cy, 1, line);
			x++;
			
			Rect r = RectC(x, y, col-1, row-1);
			if (day < 0 || day >= month_len) {
				
			}
			else {
				
				if (date == today) {
					d.DrawRect(r, White());
				}
				d.DrawText(x+7,y+7,IntStr(date.day), fnt, White());
				d.DrawText(x+5,y+5,IntStr(date.day), fnt, Black());
			}
			
			day++;
		}
	}
}

void ArtistCalendarCtrl::SetScroll(Date d) {
	Date cur = d;
	cur.day = 1;
	month_offset = DayOfWeek(cur);
	month_len = GetDaysOfMonth(cur.month, cur.year);
	scroll_date = d;
}

