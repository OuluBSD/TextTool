#include "SongTool.h"

ReverseCtrl::ReverseCtrl() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << mainsplit << vsplit;
	hsplit.SetPos(6666);
	
	vsplit.Vert() << lyrics << translated;
	
	snaplist.AddColumn(t_("Position"));
	
	mainsplit.Vert() << tasklist << resultlist << task;
	
	tasklist.AddIndex();
	tasklist.AddColumn(t_("Name"));
	tasklist.AddColumn(t_("Active"));
	tasklist.AddColumn(t_("Progress"));
	tasklist.AddColumn(t_("Attributes"));
	tasklist.ColumnWidths("4 1 2 6");
	tasklist <<= THISBACK(DataWorker);
	
	resultlist.AddIndex();
	resultlist.AddColumn(t_("Number"));
	resultlist.AddColumn(t_("Optimizer score"));
	resultlist.AddColumn(t_("Result"));
	
}

void ReverseCtrl::Data() {
	Database& db = Database::Single();
	Attributes& g = db.attrs;
	Ptrs& p = db.ctx[MALE];
	
	if (g.groups.IsEmpty()) return;
	
	this->snaplist.Clear();
	if (!p.song)
		return;
	Song& song = *p.song;
	
	song.lock.EnterRead();
	song.RealizeTaskSnaps();
	
	for(int i = 0; i < song.rev_tasks.GetCount(); i++) {
		ReverseTask& t = song.rev_tasks[i];
		
		t.lock.EnterRead();
		if (!t.snap || !t.snap->part) {t.lock.LeaveRead(); continue;}
		
		
		String name =
			g.Translate(t.snap->part->name) + ": " +
			IntStr(t.snap->id) /*+ "/" +
			IntStr(t.snap->len)*/
			;
		
		int perc = 100 * t.actual / t.total;
		tasklist.Set(i, 0, i);
		tasklist.Set(i, 1, name);
		tasklist.Set(i, 2, t.active ? t_("Active") : "");
		tasklist.Set(i, 3, perc);
		
		String s;
		for(int j = 0; j < t.result_attrs.GetCount(); j++) {
			const SnapAttrStr& sa = t.result_attrs[j];
			if (!s.IsEmpty()) s << ", ";
			Attributes::Group& gg = g.groups[sa.group_i];
			String value = gg.values[sa.item_i];
			s << Capitalize(g.Translate(gg.description)) << ": " << Capitalize(g.Translate(value));
		}
		tasklist.Set(i, 4, s);
		t.lock.LeaveRead();
	}
	song.lock.LeaveRead();
	
	String key = "gen.lyrics";
	String key_translated = "gen.lyrics." + GetCurrentLanguageString().Left(5);
	lyrics.SetData(song.snap[MALE_REVERSED].data.Get(key, "").ToWString());
	
	/*String translated = song.data.Get(key_translated, "");
	WString wtranslated = ToUnicode(translated, CHARSET_UTF8);
	this->translated.SetData(wtranslated);*/
	this->translated.SetData(song.snap[MALE_REVERSED].data.Get(key_translated, ""));
	
	if (tasklist.GetCount() && !tasklist.IsCursor())
		tasklist.SetCursor(0);
	
	if (tasklist.IsCursor())
		DataWorker();
}

void ReverseCtrl::DataWorker() {
	Database& db = Database::Single();
	Ptrs& p = db.ctx[MALE];
	if (!p.song)
		return;
	Song& song = *p.song;
	
	if (!tasklist.IsCursor())
		return;
	
	int cursor = tasklist.GetCursor();
	int id = tasklist.Get(cursor, 0);
	
	song.lock.EnterRead();
	if (id >= song.rev_tasks.GetCount()) {song.lock.LeaveRead(); return;}
	
	ReverseTask& t = song.rev_tasks[id];
	this->task.plotter.Set(t);
	this->task.plotter.Refresh();
	t.lock.EnterRead();
	int c = t.results.GetCount();
	if (c != resultlist.GetCount()) {
		for(int i = 0; i < c; i++) {
			TaskResult& r = t.results[i];
			resultlist.Set(c-1-i, 0, r.id);
			resultlist.Set(c-1-i, 1, r.id);
			resultlist.Set(c-1-i, 2, r.optimizer_score);
		}
		resultlist.SetCount(c);
	}
	task.snap_prog.Set(t.actual, t.total);
	t.lock.LeaveRead();
	
	// Total progess over all tasks
	int total_total = 0;
	int total_actual = 0;
	for (ReverseTask& t : song.rev_tasks) {
		total_total += t.total;
		total_actual += t.actual;
	}
	task.total_prog.Set(total_actual, total_total);
	
	song.lock.LeaveRead();
}


/*
void ReverseCtrl::UpdateTaskStatus(ReverseTask* task) {
	int perc = 100 * task->actual / task->total;
	
	tasklist.Set(task->id, 2, task->active ? t_("Active") : "");
	tasklist.Set(task->id, 3, perc);
	
	if (tasklist.IsCursor()) {
		int cursor = tasklist.GetCursor();
		int cursor_id = tasklist.Get(cursor, 0);
		if (task->id == cursor_id) {
			this->task.snap_prog.Set(task->actual, task->total);
		}
	}
}
*/



TaskCtrl::TaskCtrl() {
	Add(plotter.HSizePos().VSizePos(0,30));
	Add(snap_prog.HSizePos().BottomPos(15,15));
	Add(total_prog.HSizePos().BottomPos(0,15));
	
	snap_prog.Set(0,1);
	total_prog.Set(0,1);
	
}






TaskPlotter::TaskPlotter() {
	clr = LtBlue();
	line_clr = LtRed();
}

void TaskPlotter::Paint(Draw& d) {
	Size sz = GetSize();
	
	d.DrawRect(sz, White());
	
	if (!rtask || rtask->values_max <= rtask->values_min)
		return;
	
	ReverseTask& task = *rtask;
	double h = task.values_max;
	double l = task.values_min;
	double diff = h - l;
	double cx = (double)sz.cx / task.result_values.GetCount();
	int w = 3;
	int w_2 = w / 2;
	
	const double* it = task.result_values.Begin();
	const double* end = task.result_values.End();
	double xf = 0;
	while (it != end) {
		double f = (*it - l) / diff;
		int y = (1.0 - f) * sz.cy - w_2;
		int x = (int)xf - w_2;
		d.DrawRect(RectC(x, y, w, w), clr);
		it++;
		xf += cx;
	}
	
	
}

