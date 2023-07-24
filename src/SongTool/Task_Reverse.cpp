#include "SongTool.h"


void AI_Task::Process_ReverseImpact() {
	const TaskMgr& mgr = *this->mgr;
	Database& db = Database::Single();
	Attributes& g = db.attrs;
	ASSERT(p.mode >= 0);
	int mode = p.mode;
	
	int gc = db.attrs.scorings.GetCount();
	Song& song0 = *p.song;
	
	//song.RealizeImpacts();
	
	Vector<SnapContext*> ctxs0;
	song0.GetContextLevel(0, ctxs0);
	
	for (SnapContext* ctx0 : ctxs0) {
		bool ready = true;
		for(int i = 0; i < 2; i++) {
			if (/*ctx0->snap[i].impact.IsEmpty() ||*/ ctx0->snap[i].impact_score.IsEmpty()) {
				ready = false;
				break;
			}
		}
		if (!ready)
			continue;
		
		const SnapContext* best_ctx = 0;
		double best_corr_av = -DBL_MAX;
		
		for (Artist& art : db.artists) {
			for (Release& rel : art.releases) {
				for (Song& song1 : rel.songs) {
					//if (&song1 == &song0)
					//	continue;
					
					Vector<SnapContext*> ctxs1;
					song1.GetContextLevel(0, ctxs1);
					
					for (SnapContext* ctx1 : ctxs1) {
						bool fail = false;
						double corr_av = 0;
						for(int i = 0; i < 2; i++) {
							const Vector<int>& score0 = ctx0->snap[i].impact_score;
							const Vector<int>& score1 = ctx1->snap[i].impact_score;
							if (score0.GetCount() != gc || score1.GetCount() != gc) {
								fail = true;
								break;
							}
							
							const int* sc0 = score0.Begin();
							const int* sc1 = score1.Begin();
							int sum_X = 0, sum_Y = 0, sum_XY = 0;
							int squareSum_X = 0, squareSum_Y = 0;
							for(int i = 0; i < gc; i++) {
								double a = sc0[i];
								double b = sc1[i];
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
							float mul = (gc * squareSum_Y - sum_Y * sum_Y);
							float sq = sqrt((gc * squareSum_X - sum_X * sum_X) * mul);
							float corr = (float)(gc * sum_XY - sum_X * sum_Y) / sq;
							corr_av += corr * 0.5;
						}
						
						if (corr_av > best_corr_av) {
							best_corr_av = corr_av;
							best_ctx = ctx1;
						}
					}
				}
			}
		}
		
		if (best_ctx) {
			for(int i = 0; i < 2; i++) {
				PatternSnap& snap0 = ctx0->snap[MALE_REVERSED + i];
				const PatternSnap& snap1 = best_ctx->snap[MALE + i];
				snap0.impact = snap1.impact;
				snap0.impact_score <<= snap1.impact_score;
			}
		}
	}
}

void AI_Task::Process_ReverseMask() {
	const TaskMgr& mgr = *this->mgr;
	Database& db = Database::Single();
	Attributes& g = db.attrs;
	ASSERT(p.mode >= 0);
	int mode = p.mode;
	
	db.attrscores.RealizeTemp();
	
	const Attributes::Group* gp = g.groups.Begin();
	int group_count = g.groups.GetCount();
	int gc = g.scorings.GetCount();
	//int ac = db.attrscores.groups.GetCount();
	ReverseTask& task = *this->task;
	ASSERT(task.snap);
	ASSERT(task.rev_snap);
	PatternSnap& snap = *task.rev_snap;
	const double* comp = task.scores.Begin();
	
	Vector<SnapAttrStr> attrs;
	g.GetAll(attrs);
	
	for(int i = 0; i < attrs.GetCount(); i++) {
		const SnapAttrStr& sa = attrs[i];
		int& score = db.attrscores.attr_to_score[sa.group_i][sa.item_i];
		if (score < 0)
			attrs.Remove(i--);
	}
	int ma = attrs.GetCount();
	if (!ma) {
		SetError("internal error");
		return;
	}
	ASSERT(!task.scores.IsEmpty());
	ASSERT(!task.txt.IsEmpty());
	ASSERT(task.snap);
	ASSERT(task.rev_snap);
	const SnapAttrStr* mas = attrs.Begin();
		
	
	task.active = true;
	
	Vector<double> score;
	score.SetCount(gc);
	double* sc = score.Begin();
	
	GeneticOptimizer& optimizer = task.optimizer;
	FixedTopValueSorter<max_mask_values> sorter;
	double mismatch_prob = max(0.5, 1.0 - (double)max_mask_values / ma);
	
	if (!optimizer.GetRound()) {
		optimizer.SetMaxGenerations(gens);
		optimizer.Init(ma, ma * gen_multiplier);
		optimizer.MinMax(0, 1);
		
		// Process
		task.actual = 0;
		task.total = optimizer.GetMaxRounds();
		task.result_values.Reserve(1 << 16);
	}
	
	while (task.actual < task.total && !optimizer.IsEnd() && mgr.running) {
		// Get trial solution
		optimizer.Start();
		const Vector<double>& trial = optimizer.GetTrialSolution();
		for(int j = 0; j < gc; j++)
			sc[j] = 0;
		
		
		// Collect probabilities for attributes
		sorter.Reset();
		for(int i = 0; i < ma; i++) {
			double p = trial[i];
			if (p < mismatch_prob)
				continue;
			
			sorter.Add(i, p);
		}
		
		
		// Calculate score of the trial solution
		int mc = min(max_mask_values, sorter.count);
		for(int i = 0; i < mc; i++) {
			int attr_i = sorter.key[i];
			const SnapAttrStr& sa = mas[attr_i];
			int score = db.attrscores.attr_to_score[sa.group_i][sa.item_i];
			ASSERT(score >= 0);
			const AttrScoreGroup& ag = db.attrscores.groups[score];
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
				energy -= diff;
			}
			double penalty = (max_mask_values - mc) * 0.1;
			energy -= penalty;
		}
		else if (0) {
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
			float mul = (gc * squareSum_Y - sum_Y * sum_Y);
			float sq = sqrt((gc * squareSum_X - sum_X * sum_X) * mul);
			float corr = (float)(gc * sum_XY - sum_X * sum_Y) / sq;
			
			if (!IsFin(corr)) {
				DUMP(sum_X);
				DUMP(sum_Y);
				DUMP(sum_XY);
				DUMP(squareSum_X);
				DUMP(squareSum_Y);
				DUMP(sq);
				DUMP(mul);
				ASSERT(0);
			}
			energy = corr;
		}
		
		/*double penalty = max(0, enabled_count - 5) * 0.01;
		energy -= penalty;*/
		
		//DUMP(energy);
		//DUMP(optimizer.GetBestEnergy());
		bool new_best = energy > optimizer.GetBestEnergy();
		
		if ((task.actual % 100) == 0 || new_best) {
			task.result_values.Add(energy);
			task.values_max = max(energy, task.values_max);
			task.values_min = min(energy, task.values_min);
		}
		
		if (new_best) {
			LOG("Task #" << task.id << " best energy: " << energy);
			task.lock.EnterWrite();
			TaskResult& res = task.results.Add();
			res.optimizer_score = energy;
			res.id = task.actual;
			task.lock.LeaveWrite();
		}
		
		optimizer.Stop(energy);
		
		task.actual++;
		
		if (energy == 0)
			break;
	}
	
	if (!mgr.running) {
		SetError(t_("Interrupted"));
		return;
	}
	
	
	// Use the best result
	{
		task.lock.EnterWrite();
		task.result_attrs.Clear();
		snap.mask.Clear();
		
		const Vector<double>& best = optimizer.GetBestSolution();
		for(int j = 0; j < gc; j++)
			sc[j] = 0;
		
		sorter.Reset();
		for(int i = 0; i < ma; i++) {
			double t = best[i];
			if (t < mismatch_prob)
				continue;
			
			sorter.Add(i, t);
		}
		
		int mc = min(max_mask_values, sorter.count);
		for(int i = 0; i < mc; i++) {
			int attr_i = sorter.key[i];
			const SnapAttrStr& sa = mas[attr_i];
			task.result_attrs.Add(sa);
			snap.mask.FindAdd(sa);
		}
		task.lock.LeaveWrite();
	}
	
	task.Store();
}

void AI_Task::Process_ReversePattern() {
	const TaskMgr& mgr = *this->mgr;
	Database& db = Database::Single();
	Attributes& g = db.attrs;
	ASSERT(p.mode >= 0);
	int mode = p.mode;
	
	SetError("test"); return;
	
	db.attrscores.RealizeTemp();
	
	const Attributes::Group* gp = g.groups.Begin();
	int group_count = g.groups.GetCount();
	int gc = g.scorings.GetCount();
	//int ac = db.attrscores.groups.GetCount();
	ReverseTask& task = *this->task;
	if (task.mask_attrs.IsEmpty()) {
		SetError("empty mask attrs");
		return;
	}
	ASSERT(!task.mask_attrs.IsEmpty());
	ASSERT(!task.scores.IsEmpty());
	ASSERT(!task.txt.IsEmpty());
	ASSERT(task.snap);
	ASSERT(task.rev_snap);
	const double* comp = task.scores.Begin();
	PatternSnap& snap = *task.rev_snap;
	int ma = task.mask_attrs.GetCount();
	ASSERT(ma > 0);
	task.active = true;
	const SnapAttrStr* mas = task.mask_attrs.GetKeys().Begin();
	
	Vector<double> score;
	score.SetCount(gc);
	double* sc = score.Begin();
	
	GeneticOptimizer& optimizer = task.optimizer;
	FixedTopValueSorter<max_snap_values> sorter;
	double mismatch_prob = max(0.5, 1.0 - (double)max_snap_values / ma);
	
	if (!optimizer.GetRound()) {
		optimizer.SetMaxGenerations(gens);
		optimizer.Init(ma, ma * gen_multiplier);
		optimizer.MinMax(0, 1);
		
		// Process
		task.actual = 0;
		task.total = optimizer.GetMaxRounds();
		task.result_values.Reserve(1 << 16);
	}
	
	while (task.actual < task.total && !optimizer.IsEnd() && mgr.running) {
		// Get trial solution
		optimizer.Start();
		const Vector<double>& trial = optimizer.GetTrialSolution();
		for(int j = 0; j < gc; j++)
			sc[j] = 0;
		
		
		// Collect probabilities for attributes
		sorter.Reset();
		for(int i = 0; i < ma; i++) {
			double p = trial[i];
			if (p < mismatch_prob)
				continue;
			
			sorter.Add(i, p);
		}
		
		
		// Calculate score of the trial solution
		int mc = min(max_snap_values, sorter.count);
		for(int i = 0; i < mc; i++) {
			int attr_i = sorter.key[i];
			const SnapAttrStr& sa = mas[attr_i];
			int score = db.attrscores.attr_to_score[sa.group_i][sa.item_i];
			ASSERT(score >= 0);
			const AttrScoreGroup& ag = db.attrscores.groups[score];
			ASSERT(ag.scores.GetCount() == gc);
			const int* fsc = ag.scores.Begin();
			for(int j = 0; j < gc; j++)
				sc[j] += fsc[j];
		}
		
		// Calculate energy
		double energy = 0;
		if (1) {
			for(int i = 0; i < gc; i++) {
				double a = sc[i];
				double b = comp[i];
				double diff = fabs(a - b);
				energy -= diff;
			}
			double penalty = (max_snap_values - mc) * 0.1;
			energy -= penalty;
		}
		else if (0) {
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
			float mul = (gc * squareSum_Y - sum_Y * sum_Y);
			float sq = sqrt((gc * squareSum_X - sum_X * sum_X) * mul);
			float corr = (float)(gc * sum_XY - sum_X * sum_Y) / sq;
			
			if (!IsFin(corr)) {
				DUMP(sum_X);
				DUMP(sum_Y);
				DUMP(sum_XY);
				DUMP(squareSum_X);
				DUMP(squareSum_Y);
				DUMP(sq);
				DUMP(mul);
				ASSERT(0);
			}
			energy = corr;
		}
		
		/*double penalty = max(0, enabled_count - 5) * 0.01;
		energy -= penalty;*/
		
		//DUMP(energy);
		//DUMP(optimizer.GetBestEnergy());
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
		
		if (energy == 0)
			break;
	}
	
	if (!mgr.running) {
		SetError(t_("Interrupted"));
		return;
	}
	
	
	// Use the best result
	{
		task.lock.EnterWrite();
		task.result_attrs.Clear();
		snap.attributes.Clear();
		
		const Vector<double>& best = optimizer.GetBestSolution();
		for(int j = 0; j < gc; j++)
			sc[j] = 0;
		
		sorter.Reset();
		for(int i = 0; i < ma; i++) {
			double t = best[i];
			if (t < mismatch_prob)
				continue;
			
			sorter.Add(i, t);
		}
		
		int mc = min(max_snap_values, sorter.count);
		for(int i = 0; i < mc; i++) {
			int attr_i = sorter.key[i];
			const SnapAttrStr& sa = mas[attr_i];
			task.result_attrs.Add(sa);
			snap.FindAddAttr(sa);
		}
		task.lock.LeaveWrite();
	}
	
	// merge common values to owners in snaps
	snap.line->MergeOwner();
	
	task.Store();
}
