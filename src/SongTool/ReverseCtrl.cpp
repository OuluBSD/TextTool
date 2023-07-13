#include "SongTool.h"

ReverseCtrl::ReverseCtrl() {
	Add(mainsplit.SizePos());
	
	//load_active_patternscore <<= THISBACK(LoadActive);
	//stop <<= THISBACK(Stop);
	
	//stop.Disable();
	
	mainsplit.Vert() << tasklist << resultlist << task;
	
	
	tasklist.AddIndex();
	tasklist.AddColumn(t_("Name"));
	tasklist.AddColumn(t_("Active"));
	tasklist.AddColumn(t_("Progress"));
	tasklist.AddColumn(t_("Attributes"));
	tasklist.ColumnWidths("4 1 2 6");
	
	resultlist.AddIndex();
	resultlist.AddColumn(t_("Number"));
	resultlist.AddColumn(t_("Optimizer score"));
	resultlist.AddColumn(t_("Result"));
	
}

void ReverseCtrl::Data() {
	Database& db = Database::Single();
	Attributes& g = db.attrs;
	
	LOG("TODO");
	#if 0
	for(int i = 0; i < tasks.GetCount(); i++) {
		ReverseTask& t = tasks[i];
		
		String name =
			g.Translate(t.snap->part->name) + ": " +
			IntStr(t.snap->pos) + "/" +
			IntStr(t.snap->len)
			;
		
		int perc = 100 * t.actual / t.total;
		tasklist.Set(i, 0, i);
		tasklist.Set(i, 1, name);
		tasklist.Set(i, 2, t.active ? t_("Active") : "");
		tasklist.Set(i, 3, perc);
		
		String s;
		for(int j = 0; j < t.attrs.GetCount(); j++) {
			const SnapAttr& sa = t.attrs[j];
			if (!s.IsEmpty()) s << ", ";
			Attributes::Group& gg = g.groups[sa.group];
			String value = gg.values[sa.item];
			s << Capitalize(g.Translate(gg.description)) << ": " << Capitalize(g.Translate(value));
		}
		tasklist.Set(i, 4, s);
	}
	#endif
	
	if (tasklist.GetCount() && !tasklist.IsCursor())
		tasklist.SetCursor(0);
	
	if (tasklist.IsCursor())
		DataWorker();
}

void ReverseCtrl::DataWorker() {
	if (!tasklist.IsCursor())
		return;
	
	int cursor = tasklist.GetCursor();
	int id = tasklist.Get(cursor, 0);
	
	LOG("TODO");
	#if 0
	ReverseTask& t = tasks[id];
	this->task.plotter.Set(t);
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
	for (ReverseTask& t : tasks) {
		total_total += t.total;
		total_actual += t.actual;
	}
	task.total_prog.Set(total_actual, total_total);
	#endif
	
	
	
}


#if 0
void ReverseCtrl::LoadActive() {
	Database& db = Database::Single();
	Attributes& g = db.attrs;
	int gc = g.scorings.GetCount();
	int ac = db.attrscores.groups.GetCount();
	
	if (!db.active_song)
		return;
	
	Song& ps = *db.active_song;
	
	LOG("TODO");
	#if 0
	is_running = true;
	is_stopped = false;
	
	tasks.Clear();
	if (co.IsEmpty()) co.Create();
	co->SetPoolSize(max(1, CPU_Cores() - 1));
	
	Vector<PatternSnap*> level_snaps;
	p.GetSnapsLevel(0, level_snaps);
	
	int total = ac * gen_multiplier * gens;
	
	for(int i = 0; i < level_snaps.GetCount(); i++) {
		PatternSnap& snap = *level_snaps[i];
		ReverseTask& t = tasks.Add();
		t.snap = &snap;
		t.id = i;
		t.total = total;
		
		// Connect PatternSnap to PartScore values (groups.snap_position)
		// Get score-vector
		const PartScore& partscore = ps.unique_parts[snap.part_id];
		Part& part = p.unique_parts[snap.part_id];
		t.scores.SetCount(gc);
		for(int j = 0; j < gc; j++)
			t.scores[j] = partscore.values[j][snap.pos];
		
		*co & THISBACK1(TaskWorker, &t);
	}
	
	OnStart();
	#endif
	
	Data();
}
#endif

void ReverseCtrl::TaskWorker(ReverseTask* task_) {
	LOG("TODO");
	#if 0
	Database& db = Database::Single();
	Attributes& g = db.attrs;
	int gc = g.scorings.GetCount();
	int ac = db.attrscores.groups.GetCount();
	ReverseTask& task = *task_;
	Vector<double> score;
	score.SetCount(gc);
	double* sc = score.Begin();
	const double* comp = task.scores.Begin();
	ASSERT(task.scores.GetCount() == gc);
	task.scores.SetCount(gc, 0);
	task.active = true;
	
	GeneticOptimizer optimizer;
	optimizer.SetMaxGenerations(gens);
	optimizer.Init(ac, ac * gen_multiplier);
	optimizer.MinMax(0, 1);
	double mismatch_prob = 1.0 - 5.0 / ac;
	
	// Process
	FixedTopValueSorter<max_values> sorter;
	task.actual = 0;
	task.total = optimizer.GetMaxRounds();
	task.result_values.Reserve(1 << 16);
	while (task.actual < task.total && !optimizer.IsEnd() && is_running) {
		optimizer.Start();
		
		// Calculate score of the trial solution
		const Vector<double>& trial = optimizer.GetTrialSolution();
		for(int j = 0; j < gc; j++)
			sc[j] = 0;
		
		sorter.Reset();
		for(int i = 0; i < ac; i++) {
			double t = trial[i];
			if (t < mismatch_prob)
				continue;
			
			sorter.Add(i, t);
		}
		
		int mc = min(max_values, sorter.count);
		for(int i = 0; i < mc; i++) {
			int group_i = sorter.key[i];
			
			const AttrScoreGroup& ag = db.attrscores.groups[group_i];
			ASSERT(ag.scores.GetCount() == gc);
			const int* fsc = ag.scores.Begin();
			for(int j = 0; j < gc; j++)
				sc[j] += fsc[j];
		}
		
		// Calculate energy
		double energy = 0;
		if (0) {
			for(int i = 0; i < gc; i++) {
				double a = sc[i];
				double b = comp[i];
				double diff = fabs(a - b);
				bool sgn_mismatch = (a > 0) == (b > 0);
				bool large_value = fabs(a) > fabs(b);
				double mul = (sgn_mismatch ? 2 : 1) * (large_value ? 2 : 1);
				energy -= diff * mul;
			}
		}
		else {
			int sum_X = 0, sum_Y = 0, sum_XY = 0;
			int squareSum_X = 0, squareSum_Y = 0;
			for(int i = 0; i < gc; i++) {
				double a = sc[i];
				double b = comp[i];
				if (!a) continue;
				
				// sum of elements of array X.
		        sum_X = sum_X + a;
		 
		        // sum of elements of array Y.
		        sum_Y = sum_Y + b;
		 
		        // sum of X[i] * Y[i].
		        sum_XY = sum_XY + a * b;
		 
		        // sum of square of array elements.
		        squareSum_X = squareSum_X + a * a;
		        squareSum_Y = squareSum_Y + b * b;
			}
			// use formula for calculating correlation coefficient.
			float corr = (float)(gc * sum_XY - sum_X * sum_Y)
			      / sqrt((gc * squareSum_X - sum_X * sum_X)
			          * (gc * squareSum_Y - sum_Y * sum_Y));
			
			energy = corr;
		}
		
		/*double penalty = max(0, enabled_count - 5) * 0.01;
		energy -= penalty;*/
		
		bool new_best = energy > optimizer.GetBestEnergy();
		
		if ((task.actual % 100) == 0 || new_best) {
			task.result_values.Add(energy);
			task.values_max = max(energy, task.values_max);
			task.values_min = min(energy, task.values_min);
		}
		
		if (new_best) {
			//LOG("Task #" << task.id << " best energy: " << energy);
			task.lock.EnterWrite();
			TaskResult& res = task.results.Add();
			res.optimizer_score = energy;
			res.id = task.actual;
			task.lock.LeaveWrite();
		}
		
		optimizer.Stop(energy);
		
		task.actual++;
		
		if ((task.actual % 10000) == 0)
			PostCallback(THISBACK1(UpdateTaskStatus, task_));
	}
	
	
	
	// Use the best result
	{
		task.lock.EnterWrite();
		task.attrs.Clear();
		task.snap->attributes.Clear();
		
		const Vector<double>& best = optimizer.GetBestSolution();
		for(int j = 0; j < gc; j++)
			sc[j] = 0;
		
		sorter.Reset();
		for(int i = 0; i < ac; i++) {
			double t = best[i];
			if (t < mismatch_prob)
				continue;
			
			sorter.Add(i, t);
		}
		
		int mc = min(max_values, sorter.count);
		for(int i = 0; i < mc; i++) {
			int group_i = sorter.key[i];
			const AttrScoreGroup& ag = db.attrscores.groups[group_i];
			if (ag.attrs.IsEmpty())
				continue;
			
			int c = ag.attrs.GetCount();
			const SnapAttr& sa = ag.attrs[Random(c)];
			task.attrs.Add(sa);
			
			SnapAttrStr sas;
			sas.Load(sa);
			task.snap->attributes.FindAdd(sas);
		}
		task.lock.LeaveWrite();
	}
	
	// merge common values to owners in snaps
	task.snap->MergeOwner();
	
	
	task.active = false;
	
	PostCallback(THISBACK1(UpdateTaskStatus, task_));
	
	
	// Check if all ready
	LOG("TODO");
	#if 0
	bool all_ready = true;
	int active_count = 0;
	for(ReverseTask& task : tasks) {
		if (task.actual < task.total) {
			all_ready = false;
		}
		if (task.active)
			active_count++;
	}
	if (all_ready || (!is_running && !active_count)) {
		PostCallback(THISBACK(OnStop));
	}
	#endif
	#endif
}

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

