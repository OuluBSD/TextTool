#include "SongTool.h"


ReverseImpactCtrl::ReverseImpactCtrl() {
	Add(split.SizePos());
	
	split.Vert() << list << plotter;
	
	list.AddColumn(t_("Break"));
	list.AddColumn(t_("Source"));
	list.AddColumn(t_("Score"));
	
}

void ReverseImpactCtrl::Data() {
	Database& db = Database::Single();
	if (!db.ctx.ed.song) return;
	Song& song = *db.ctx.ed.song;
	Pipe& pipe = *song.pipe;
	
	pipe.lock.EnterRead();
	
	if (pipe.rev_impact >= 0) {
		ReverseTask& t = pipe.rev_tasks[pipe.rev_impact];
		int c = t.impact_results.GetCount();
		for(int i = 0; i < c; i++) {
			String key = t.impact_results.GetKey(i);
			const Tuple2<String, double>& res = t.impact_results[i];
			const String& src = res.a;
			double val = res.b;
			list.Set(i, 0, key);
			list.Set(i, 1, src);
			list.Set(i, 2, val);
		}
		list.SetCount(c);
	}
	else list.Clear();
	
	pipe.lock.LeaveRead();
	
	plotter.Refresh();
}





ReverseImpactPlotter::ReverseImpactPlotter() {
	
}

void ReverseImpactPlotter::Paint(Draw& d) {
	Size sz = GetSize();
	d.DrawRect(sz, White());
	
	Database& db = Database::Single();
	if (!db.ctx.ed.song) return;
	Song& song = *db.ctx.ed.song;
	Pipe& pipe = *song.pipe;
	
	
	pipe.lock.EnterRead();
	
	if (pipe.rev_impact < 0) {pipe.lock.LeaveRead(); return;}
	ReverseTask& t = pipe.rev_tasks[pipe.rev_impact];
	int c = t.impact_results.GetCount();
	if (c < 2) {pipe.lock.LeaveRead(); return;}
	
	double cx = (double)sz.cx / (c-1);
	
	double high = 1.0;
	double low = -1.0;
	double diff = high - low;
	int w = 3;
	int w_2 = w / 2;
	Vector<Point> pts;
	for(int i = 0; i < c; i++) {
		double val = t.impact_results[i].b;
		double f = (val - low) / diff;
		int y = (int)(-sz.cy + f * sz.cy - w_2);
		int x = (int)(i * cx - w_2);
		if (polyline)
			pts << Point(x,y);
		else
			d.DrawRect(x, y, w, w, Blue());
	}
	if (polyline)
		d.DrawPolyline(pts, w, Blue());
	
	pipe.lock.LeaveRead();
	
}
