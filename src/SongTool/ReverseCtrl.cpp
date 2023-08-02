#include "SongTool.h"

ReverseCtrl::ReverseCtrl() {
	Add(mainsplit.SizePos());
	
	/*hsplit.Horz() << mainsplit << vsplit;
	hsplit.SetPos(6666);
	
	vsplit.Vert() << lyrics << translated;*/
	
	snaplist.AddColumn(t_("Position"));
	
	mainsplit.Vert() << tasklist << resultlist << task;
	
}

void ReverseCtrl::SetSource(int i) {
	src = i;
	
	tasklist.AddIndex();
	tasklist.AddColumn(t_("Name"));
	tasklist.AddColumn(t_("Active"));
	tasklist.AddColumn(t_("Progress"));
	if (src == 0) {
		tasklist.AddColumn(t_("Attributes"));
	}
	else if (src == 1) {
		tasklist.AddColumn(t_("Male Attributes"));
		tasklist.AddColumn(t_("Female Attributes"));
	}
	else if (src == 2) {
		tasklist.AddColumn(t_("Male Attributes"));
		tasklist.AddColumn(t_("Female Attributes"));
		tasklist.AddColumn(t_("Common Attributes"));
	}
	
	if (src == 0)
		tasklist.ColumnWidths("1 1 1 9");
	else if (src == 1)
		tasklist.ColumnWidths("2 1 1 9 9");
	else if (src == 2)
		tasklist.ColumnWidths("3 1 1 9 9 9");
	
	tasklist <<= THISBACK(DataWorker);
	
	resultlist.AddIndex();
	resultlist.AddColumn(t_("Number"));
	resultlist.AddColumn(t_("Optimizer score"));
	resultlist.AddColumn(t_("Result"));
	
}

void ReverseCtrl::Data() {
	Database& db = Database::Single();
	Attributes& g = db.attrs;
	Ptrs& p = db.ctx.p;
	
	if (g.groups.IsEmpty()) return;
	
	this->snaplist.Clear();
	if (!p.song)
		return;
	Song& song = *p.song;
	
	song.lock.EnterRead();
	
	song.RealizeTaskSnaps();
	
	Array<ReverseTask>& tasks = src == 0 ? song.rev_common_mask_tasks : (src == 1 ? song.rev_separate_mask_tasks : song.rev_pattern_tasks);
	
	int row = 0;
	for(int i = 0; i < tasks.GetCount(); i++) {
		ReverseTask& t = tasks[i];
		
		if (!t.ctx) {continue;}
		
		t.lock.EnterRead();
		
		PatternSnap& snap = t.ctx->snap[0];
		String name =
			g.Translate(snap.part->name) + ": " +
			IntStr(snap.id) /*+ "/" +
			IntStr(t.snap->len)*/
			;
		
		int perc = 100 * t.actual / t.total;
		tasklist.Set(row, 0, i);
		tasklist.Set(row, 1, name);
		tasklist.Set(row, 2, t.active ? t_("Active") : "");
		tasklist.Set(row, 3, perc);
		
		int mode_count = 0;
		switch (src) {
			case 0: mode_count = 1; break;
			case 1: mode_count = 2; break;
			case 2: mode_count = 3; break;
			default: break;
		}
		
		for (int mode = 0; mode < mode_count; mode++) {
			if (t.result_attrs.IsEmpty())
				continue;
			String s;
			for(int j = 0; j < t.result_attrs[mode].GetCount(); j++) {
				const SnapAttrStr& sa = t.result_attrs[mode][j];
				if (!s.IsEmpty()) s << ", ";
				Attributes::Group& gg = g.groups[sa.group_i];
				String value = gg.values[sa.item_i];
				s << Capitalize(g.Translate(gg.description)) << ": " << Capitalize(g.Translate(value));
			}
			tasklist.Set(row, 4 + mode, s);
		}
		t.lock.LeaveRead();
		
		row++;
	}
	tasklist.SetCount(row);
	song.lock.LeaveRead();
	
	/*String key = "gen.lyrics";
	String key_translated = "gen.lyrics." + GetCurrentLanguageString().Left(5);
	lyrics.SetData(song.snap[MALE_REVERSED].data.Get(key, "").ToWString());
	
	this->translated.SetData(song.snap[MALE_REVERSED].data.Get(key_translated, ""));*/
	
	if (tasklist.GetCount() && !tasklist.IsCursor())
		tasklist.SetCursor(0);
	
	if (tasklist.IsCursor())
		DataWorker();
}

void ReverseCtrl::DataWorker() {
	Database& db = Database::Single();
	Ptrs& p = db.ctx.p;
	if (!p.song)
		return;
	Song& song = *p.song;
	
	if (!tasklist.IsCursor())
		return;
	
	int cursor = tasklist.GetCursor();
	int id = tasklist.Get(cursor, 0);
	
	song.lock.EnterRead();
	
	Array<ReverseTask>& tasks = src == 0 ? song.rev_common_mask_tasks : (src == 1 ? song.rev_separate_mask_tasks : song.rev_pattern_tasks);
	if (id < 0 || id >= tasks.GetCount()) {song.lock.LeaveRead(); return;}
	
	ReverseTask& t = tasks[id];
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
	for (ReverseTask& t : song.rev_pattern_tasks) {
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
		int y = (int)((1.0 - f) * sz.cy - w_2);
		int x = (int)xf - w_2;
		d.DrawRect(RectC(x, y, w, w), clr);
		it++;
		xf += cx;
	}
	
	
}

