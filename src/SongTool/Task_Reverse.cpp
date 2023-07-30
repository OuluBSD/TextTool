#include "SongTool.h"


void Task::Process_ReverseImpact() {
	// Gather references for easy access
	const TaskMgr& mgr = *this->mgr;
	Database& db = Database::Single();
	Attributes& g = db.attrs;
	ASSERT(p.mode == 0);
	int gc = db.attrs.scorings.GetCount();
	Song& song0 = *p.song;
	
	
	// Get all Break classes as SnapContext pointers (Break inherits SnapContext)
	// These represents target objects in song-structure.
	Vector<SnapContext*> ctxs0;
	song0.GetContextLevel(0, ctxs0);
	
	// Clear result vector (with locks for gui)
	song0.lock.EnterWrite();
	song0.rev_impact.impact_results.Clear();
	song0.lock.LeaveWrite();
	
	// Check if we have special development case, in which only one proper song is added (early dev)
	// Then, allow itself to be used as resource.
	int valid_song_count = 0;
	for (Artist& art : db.artists)
		for (Release& rel : art.releases)
			for (Song& song1 : rel.songs)
				if (&song1 != &song0 && song1.parts.GetCount())
					valid_song_count++;
	bool accept_dbg_same_song = valid_song_count == 0;
	
	// Find solutions for target SnapContext pointers (Break classes)
	for (SnapContext* ctx0 : ctxs0) {
		
		// Don't do useless work: skip if impact score is ready
		bool ready = true;
		for(int i = 0; i < 2; i++) {
			if (ctx0->snap[i].impact_score.IsEmpty()) {
				ready = false;
				break;
			}
		}
		if (!ready)
			continue;
		
		
		// Search for best matching SnapContext in other songs
		const SnapContext* best_ctx = 0;
		double best_corr_av = -DBL_MAX;
		for (Artist& art : db.artists) {
			for (Release& rel : art.releases) {
				for (Song& song1 : rel.songs) {
					// Accept same song in a special case
					if (!accept_dbg_same_song && &song1 == &song0)
						continue;
					
					// Get Break classes of the other song
					Vector<SnapContext*> ctxs1;
					song1.GetContextLevel(0, ctxs1);
					
					// Loop Break classes
					for (SnapContext* ctx1 : ctxs1) {
						bool fail = false;
						double corr_av = 0;
						
						// Loop major genders
						for(int i = 0; i < GENDER_COUNT; i++) {
							const Vector<int>& score0 = ctx0->snap[i].impact_score;
							const Vector<int>& score1 = ctx1->snap[i].impact_score;
							
							// Check that score-vector matches the count of score-types in database
							if (score0.GetCount() != gc || score1.GetCount() != gc) {
								fail = true;
								break;
							}
							
							// Calculate correlation coefficient between score-vectors
							const int* sc0 = score0.Begin();
							const int* sc1 = score1.Begin();
							double sum_X = 0, sum_Y = 0, sum_XY = 0;
							double squareSum_X = 0, squareSum_Y = 0;
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
							double mul = (gc * squareSum_Y - sum_Y * sum_Y);
							double sq = sqrt((gc * squareSum_X - sum_X * sum_X) * mul);
							double corr = (gc * sum_XY - sum_X * sum_Y) / sq;
							
							// Use average correlation of genders as heuristic value
							corr_av += corr * (1.0 / GENDER_COUNT);
						}
						
						// Update the best value
						if (corr_av > best_corr_av) {
							best_corr_av = corr_av;
							best_ctx = ctx1;
							Ptrs& p = ctx1->snap[0];
						}
					}
				}
			}
		}
		
		// Use best match (if any)
		if (best_ctx) {
			// Write essential results (impact scores in Breaks)
			
			// Loop major genders for snapshots in the context
			for(int i = 0; i < GENDER_COUNT; i++) {
				PatternSnap& snap0 = ctx0->snap[MALE_REVERSED + i];
				const PatternSnap& snap1 = best_ctx->snap[MALE + i];
				snap0.impact = snap1.impact;
				snap0.impact_score <<= snap1.impact_score;
			}
			
			// Write non-essential results
			
			// Get location of Break class for target and source
			String loc = ctx0->snap[0].GetBreakInSongString();
			String best_loc = best_ctx->snap[0].GetBreakInDatabaseString();
			
			// Write results to the song object
			song0.lock.EnterWrite();
			Tuple2<String,double>& result = song0.rev_impact.impact_results.Add(loc);
			result.a = best_loc;
			result.b = best_corr_av;
			song0.lock.LeaveWrite();
		}
	}
}

void Task::Process_ReverseCommonMask() {
	// Gather stack references for easy access
	const TaskMgr& mgr = *this->mgr;
	Database& db = Database::Single();
	Attributes& g = db.attrs;
	ASSERT(p.mode >= 0);
	int mode = p.mode;
	ASSERT(mode == MALE);
	ReverseTask& task = *this->task;
	ASSERT(task.ctx);
	SnapContext& ctx = *task.ctx;
	GeneticOptimizer& optimizer = task.optimizer;
	
	// Update group/item to score shortcut vector
	db.attrscores.UpdateGroupsToScoring();
	
	// Get attribute-groups for high performance access
	const Attributes::Group* gp = g.groups.Begin();
	int group_count = g.groups.GetCount();
	
	// Get scorings for fast access
	int gc = g.scorings.GetCount(); // TODO rename gc
	ASSERT(ctx.snap[0].partscore.GetCount() == gc);
	ASSERT(ctx.snap[1].partscore.GetCount() == gc);
	const int* comp[2];
	comp[0] = ctx.snap[0].partscore.Begin();
	comp[1] = ctx.snap[1].partscore.Begin();
	
	// Get all attribute groups and items in the database to single vector
	Vector<SnapAttrStr> attrs;
	g.GetAll(attrs);
	
	// Remove attributes, which doesn't have scores yet
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
	
	// The task has been prepared
	task.active = true;
	
	// Get fast access pointer
	const SnapAttrStr* mas = attrs.Begin();
	
	// Temp vector for trial's score
	Vector<double> score;
	score.SetCount(gc, 0);
	double* sc = score.Begin();
	
	// Create fast fixed sorter
	FixedTopValueSorter<common_mask_max_values> sorter;
	
	// Set probability for attribute to not be used
	double mismatch_prob = max(0.5, 1.0 - (double)common_mask_max_values / ma);
	
	// If we are in running this task first time (and not continuing previous)
	if (!optimizer.GetRound()) {
		// Initialize optimizer
		optimizer.SetMaxGenerations(common_mask_gens);
		optimizer.Init(ma, ma * common_mask_gen_multiplier); // dimension, population
		optimizer.MinMax(0, 1); // range of values in the trial vector
		
		// Values for keeping track of the progress percentage
		task.actual = 0;
		task.total = optimizer.GetMaxRounds();
		
		// Reserve memory (to avoid realloc)
		task.result_values.Reserve(1 << 16);
	}
	
	// Main loop for optimizer (check for program shutdown)
	while (task.actual < task.total && !optimizer.IsEnd() && mgr.running) {
		// Get trial solution
		optimizer.Start();
		const Vector<double>& trial = optimizer.GetTrialSolution();
		for(int j = 0; j < gc; j++)
			sc[j] = 0;
		
		
		// Collect probabilities for attributes (and sort them, and discard lowest)
		sorter.Reset();
		for(int i = 0; i < ma; i++) {
			double p = trial[i];
			if (p < mismatch_prob)
				continue;
			
			sorter.Add(i, p);
		}
		
		
		// Calculate score of the trial solution
		int mc = min(common_mask_max_values, sorter.count);
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
		
		// Calculate result for the trial (=energy)
		double av_energy = 0;
		for (int mode = 0; mode < GENDER_COUNT; mode++) {
			double energy = 0;
			if (0) {
				for(int i = 0; i < gc; i++) {
					double a = sc[i];
					double b = comp[mode][i];
					double diff = fabs(a - b);
					energy -= diff;
				}
				double penalty = (common_mask_max_values - mc) * 0.1;
				energy -= penalty;
			}
			else if (0) {
				for(int i = 0; i < gc; i++) {
					double a = sc[i];
					double b = comp[mode][i];
					double diff = fabs(a - b);
					bool sgn_mismatch = (a > 0) == (b > 0);
					bool large_value = fabs(a) > fabs(b);
					double mul = (sgn_mismatch ? 2 : 1) * (large_value ? 2 : 1);
					energy -= diff * mul;
				}
			}
			else {
				double sum_X = 0, sum_Y = 0, sum_XY = 0;
				double squareSum_X = 0, squareSum_Y = 0;
				for(int i = 0; i < gc; i++) {
					double a = sc[i];
					double b = comp[mode][i];
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
				double mul = (gc * squareSum_Y - sum_Y * sum_Y);
				double sq = sqrt((gc * squareSum_X - sum_X * sum_X) * mul);
				double corr = (gc * sum_XY - sum_X * sum_Y) / sq;
				
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
			av_energy += energy * (1.0 / GENDER_COUNT);
		}
		
		// Check if this is the best result so far
		bool new_best = av_energy > optimizer.GetBestEnergy();
		
		// Collect result values coarsely for gui
		if ((task.actual % 100) == 0 || new_best) {
			task.result_values.Add(av_energy);
			task.values_max = max(av_energy, task.values_max);
			task.values_min = min(av_energy, task.values_min);
		}
		
		// Collect best result
		if (new_best) {
			LOG("Task #" << IntStr64((int64)this) << " best energy: " << av_energy);
			task.lock.EnterWrite();
			TaskResult& res = task.results.Add();
			res.optimizer_score = av_energy;
			res.id = task.actual;
			task.lock.LeaveWrite();
		}
		
		// Return trial's result to the optimizer too
		optimizer.Stop(av_energy);
		
		// Keep track of progress
		task.actual++;
		
		// Check for internal errors
		if (av_energy == 0)
			break;
	}
	
	// Exit fast, if program is shutting down
	if (!mgr.running) {
		task.active = false;
		task.Store();
		SetError(t_("Interrupted"));
		return;
	}
	
	
	// Use the best result
	{
		task.lock.EnterWrite();
		
		// All results are in the same vector, because these are common values
		task.result_attrs.SetCount(1);
		auto& result_attrs = task.result_attrs[0];
		result_attrs.Clear();
		
		// Loop for genders
		for (int mode = 0; mode < GENDER_COUNT; mode++) {
			
			// Write to 'reversed' objects instead of source value objects
			PatternSnap& snap = ctx.snap[MALE_REVERSED + mode];
			snap.mask.Clear();
			
			// Get the best trial solution from the optimizer
			const Vector<double>& best = optimizer.GetBestSolution();
			
			// Use same sorter as in trial solution algorithm
			
			// Add values to the sorter
			sorter.Reset();
			for(int i = 0; i < ma; i++) {
				double t = best[i];
				if (t < mismatch_prob)
					continue;
				
				sorter.Add(i, t);
			}
			
			// Get best values from the sorter
			int mc = min(common_mask_max_values, sorter.count);
			for(int i = 0; i < mc; i++) {
				int attr_i = sorter.key[i];
				const SnapAttrStr& sa = mas[attr_i];
				
				// Add result for the gui (management)
				result_attrs.Add(sa);
				
				// Add result for the database (and pipeline)
				snap.mask.FindAdd(sa);
			}
		}
		task.lock.LeaveWrite();
	}
	
	// Release some useless memory
	if (optimizer.IsEnd())
		optimizer.ClearPopulation();
	
	// Set as inactive and cache results, so duplicate work can be avoided.
	task.active = false;
	task.Store();
}

void Task::Process_ReverseSeparateMask() {
	// Gather stack references for easy access
	const TaskMgr& mgr = *this->mgr;
	Database& db = Database::Single();
	Attributes& g = db.attrs;
	ASSERT(p.mode >= 0);
	int mode = p.mode;
	ReverseTask& task = *this->task;
	ASSERT(task.ctx);
	SnapContext& ctx = *task.ctx;
	GeneticOptimizer& optimizer = task.optimizer;
	
	// Update group/item to score shortcut vector
	db.attrscores.UpdateGroupsToScoring();
	
	// Get attribute-groups for high performance access
	const Attributes::Group* gp = g.groups.Begin();
	int group_count = g.groups.GetCount();
	
	// Get scorings for fast access
	int gc = g.scorings.GetCount();
	ASSERT(ctx.snap[0].partscore.GetCount() == gc);
	ASSERT(ctx.snap[1].partscore.GetCount() == gc);
	const int* comp[2];
	comp[0] = ctx.snap[0].partscore.Begin();
	comp[1] = ctx.snap[1].partscore.Begin();
	
	// Get all attribute groups and items in the database to single vector
	Vector<SnapAttrStr> attrs;
	g.GetAll(attrs);
	
	// Remove attributes, which doesn't have scores yet
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
	const SnapAttrStr* mas = attrs.Begin();
	
	// The task has been prepared
	task.active = true;
	
	// Temp vector for trial's score (for all genders)
	Vector<double> score[GENDER_COUNT];
	double* sc[GENDER_COUNT];
	for(int i = 0; i < GENDER_COUNT; i++) {
		score[i].SetCount(gc,0);
		sc[i] = score[i].Begin();
	}
	
	// Create fast fixed sorter
	FixedTopValueSorter<separate_mask_max_values> sorter[GENDER_COUNT];
	
	// Set probability for attribute to not be used
	double mismatch_prob = max(0.5, 1.0 - (double)separate_mask_max_values / ma);
	
	// If we are in running this task first time (and not continuing previous)
	if (!optimizer.GetRound()) {
		// Initialize optimizer
		optimizer.SetMaxGenerations(separate_mask_gens);
		optimizer.Init(ma, ma * separate_mask_gen_multiplier); // dimension, population
		optimizer.MinMax(0, 1); // range of values in the trial vector
		
		// Values for keeping track of the progress percentage
		task.actual = 0;
		task.total = optimizer.GetMaxRounds();
		
		// Reserve memory (to avoid slow realloc)
		task.result_values.Reserve(1 << 16);
	}
	
	// Main loop for optimizer (check for program shutdown)
	while (task.actual < task.total && !optimizer.IsEnd() && mgr.running) {
		// Get trial solution
		optimizer.Start();
		const Vector<double>& trial = optimizer.GetTrialSolution();
		
		// Reset score vector
		for(int i = 0; i < GENDER_COUNT; i++)
			for(int j = 0; j < gc; j++)
				sc[i][j] = 0;
		
		
		// Collect probabilities for attributes (and sort them, and discard lowest)
		sorter[0].Reset();
		sorter[1].Reset();
		for(int i = 0; i < ma; i++) {
			static_assert(GENDER_COUNT == 2, "only 2 genders implemented here");
			double p = trial[i];
			if (+p >= mismatch_prob)
				sorter[0].Add(i, p);
			if (1-p >= mismatch_prob) // use inversed probability for opposite gender
				sorter[1].Add(i, 1-p);
		}
		
		
		// Calculate score of the trial solution (for all genders)
		for (int mode = 0; mode < GENDER_COUNT; mode++) {
			int mc = min(separate_mask_max_values, sorter[mode].count);
			for(int i = 0; i < mc; i++) {
				int attr_i = sorter[mode].key[i];
				const SnapAttrStr& sa = mas[attr_i];
				int score = db.attrscores.attr_to_score[sa.group_i][sa.item_i];
				ASSERT(score >= 0);
				const AttrScoreGroup& ag = db.attrscores.groups[score];
				ASSERT(ag.scores.GetCount() == gc);
				const int* fsc = ag.scores.Begin();
				for(int j = 0; j < gc; j++)
					sc[mode][j] += fsc[j];
			}
		}
		
		// Calculate result for all genders (and use average)
		double av_energy = 0;
		for (int mode = 0; mode < GENDER_COUNT; mode++) {
			double energy = 0;
			if (0) {
				int mc = min(separate_mask_max_values, sorter[mode].count);
				for(int i = 0; i < gc; i++) {
					double a = sc[mode][i];
					double b = comp[mode][i];
					double diff = fabs(a - b);
					energy -= diff;
				}
				double penalty = (separate_mask_max_values - mc) * 0.1;
				energy -= penalty;
			}
			else if (0) {
				for(int i = 0; i < gc; i++) {
					double a = sc[mode][i];
					double b = comp[mode][i];
					double diff = fabs(a - b);
					bool sgn_mismatch = (a > 0) == (b > 0);
					bool large_value = fabs(a) > fabs(b);
					double mul = (sgn_mismatch ? 2 : 1) * (large_value ? 2 : 1);
					energy -= diff * mul;
				}
			}
			else {
				double sum_X = 0, sum_Y = 0, sum_XY = 0;
				double squareSum_X = 0, squareSum_Y = 0;
				for(int i = 0; i < gc; i++) {
					double a = sc[mode][i];
					double b = comp[mode][i];
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
				double mul = (gc * squareSum_Y - sum_Y * sum_Y);
				double sq = sqrt((gc * squareSum_X - sum_X * sum_X) * mul);
				double corr = (gc * sum_XY - sum_X * sum_Y) / sq;
				
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
			av_energy += energy * (1.0 / GENDER_COUNT);
		}
		
		// Check if this is the best result so far
		bool new_best = av_energy > optimizer.GetBestEnergy();
		
		// Collect result values coarsely for gui
		if ((task.actual % 100) == 0 || new_best) {
			task.result_values.Add(av_energy);
			task.values_max = max(av_energy, task.values_max);
			task.values_min = min(av_energy, task.values_min);
		}
		
		// Collect best result
		if (new_best) {
			LOG("Task #" << IntStr64((int64)this) << " best energy: " << av_energy);
			task.lock.EnterWrite();
			TaskResult& res = task.results.Add();
			res.optimizer_score = av_energy;
			res.id = task.actual;
			task.lock.LeaveWrite();
		}
		
		// Return trial's result to the optimizer too
		optimizer.Stop(av_energy);
		
		// Keep track of progress
		task.actual++;
		
		// Check for internal errors
		if (av_energy == 0)
			break;
	}
	
	// Exit fast, if program is shutting down
	if (!mgr.running) {
		task.active = false;
		task.Store();
		SetError(t_("Interrupted"));
		return;
	}
	
	
	// Use the best result
	{
		task.lock.EnterWrite();
		
		// Clear values (loop for genders)
		for (int mode = 0; mode < GENDER_COUNT; mode++) {
			task.result_attrs[mode].Clear();
			PatternSnap& snap = ctx.snap[MALE_REVERSED + mode];
			// DON'T CLEAR MASK HERE BECAUSE OF ALREADY WRITTEN COMMON ATTRS: snap.mask.Clear();
			sorter[mode].Reset();
			for(int j = 0; j < gc; j++)
				sc[mode][j] = 0;
		}
		
		// Get the best trial solution from the optimizer
		const Vector<double>& best = optimizer.GetBestSolution();
		
		// Use same sorter as in trial solution algorithm
		
		// Add values to the sorter
		for(int i = 0; i < ma; i++) {
			double t = best[i];
			if (+t >= mismatch_prob)
				sorter[0].Add(i, t);
			if (1-t >= mismatch_prob)
				sorter[1].Add(i, 1-t);
		}
		
		// Get best values from the sorter (for all genders separately)
		for (int mode = 0; mode < GENDER_COUNT; mode++) {
			int mc = min(common_mask_max_values, sorter[mode].count);
			PatternSnap& snap = ctx.snap[MALE_REVERSED + mode];
			for(int i = 0; i < mc; i++) {
				int attr_i = sorter[mode].key[i];
				const SnapAttrStr& sa = mas[attr_i];
				
				// Add result for the gui (management)
				task.result_attrs[mode].Add(sa);
				
				// Add result for the database (and pipeline)
				snap.mask.FindAdd(sa);
			}
			ASSERT(!snap.mask.IsEmpty());
		}
		task.lock.LeaveWrite();
	}
	
	// Release some useless memory
	if (optimizer.IsEnd())
		optimizer.ClearPopulation();
	
	// Set as inactive and cache results, so duplicate work can be avoided.
	task.active = false;
	task.Store();
}

void Task::Process_ReversePattern() {
	// Gather stack references for easy access
	const TaskMgr& mgr = *this->mgr;
	Database& db = Database::Single();
	Attributes& g = db.attrs;
	ASSERT(p.mode >= 0);
	int mode = p.mode;
	ReverseTask& task = *this->task;
	if (task.mask_attrs.IsEmpty()) {
		SetError("no snap mask attributes");
		return;
	}
	GeneticOptimizer& optimizer = task.optimizer;
	ASSERT(task.scores.GetCount() == GENDER_COMMON_WEIGHTED_COUNT);
	ASSERT(task.ctx);
	SnapContext& ctx = *task.ctx;
	
	// Update group/item to score shortcut vector
	db.attrscores.UpdateGroupsToScoring();
	
	// Get attribute-groups for high performance access
	const Attributes::Group* gp = g.groups.Begin();
	int group_count = g.groups.GetCount();
	
	// Get scorings for fast access
	int gc = g.scorings.GetCount();
	const double* comp[GENDER_COMMON_WEIGHTED_COUNT];
	const SnapAttrStr* all_mas = task.mask_attrs.GetKeys().Begin();
	for(int i = 0; i < GENDER_COMMON_WEIGHTED_COUNT; i++) {
		ASSERT(task.scores[i].GetCount() == gc);
		comp[i] = task.scores[i].Begin();
	}
	int ma = task.mask_attrs.GetCount();
	
	// The task has been prepared
	task.active = true;
	
	// Temp vector for trial's score (for common + all genders)
	Vector<double> score[GENDER_COMMON_WEIGHTED_COUNT];
	double* sc[GENDER_COMMON_WEIGHTED_COUNT];
	for(int i = 0; i < GENDER_COMMON_WEIGHTED_COUNT; i++) {
		score[i].SetCount(gc);
		sc[i] = score[i].Begin();
	}
	
	// Find ranges for common + genders
	int offset[GENDER_COMMON_COUNT];
	int length[GENDER_COMMON_COUNT];
	//const SnapAttrStr* mas[GENDER_COMMON_COUNT];
	{
		memset(offset, 0, sizeof(offset));
		memset(length, 0, sizeof(length));
		int mode = -1, prev_mode = -1;
		for(int i = 0; i < task.mask_attrs.GetCount(); i++) {
			int cur = task.mask_attrs[i];
			int mode = cur-1; static_assert(GENDER_COUNT == 2, "check this");
			length[mode]++;
			if (mode != prev_mode && prev_mode >= 0) {
				offset[mode] = offset[mode-1] + length[mode-1];
			}
			prev_mode = mode;
		}
		for(int i = 0; i < GENDER_COMMON_COUNT; i++) {
			//mas[i] = all_mas + offset[i];
			if (i > 0) {
				ASSERT(offset[i] > offset[i-1]);
			}
			if (!length[i]) {
				DUMPM(task.mask_attrs);
				DUMPC(task.mask_attrs.GetKeys());
				LOG("error: length 0 for mode: " << GetCommonModeString(i));
				ASSERT(0);
			}
		}
	}
	
	// Create fast fixed sorter
	FixedTopValueSorter<snap_max_per_mode> sorter[GENDER_COMMON_COUNT];
		
	// If we are in running this task first time (and not continuing previous)
	if (!optimizer.GetRound()) {
		// Initialize optimizer
		optimizer.SetMaxGenerations(snap_gens);
		optimizer.Init(ma, ma * snap_gen_multiplier); // dimension, population
		optimizer.MinMax(0, 1); // range of values in the trial vector
		
		// Values for keeping track of the progress percentage
		task.actual = 0;
		task.total = optimizer.GetMaxRounds();
		
		// Reserve memory (to avoid slow realloc)
		task.result_values.Reserve(1 << 16);
	}
	
	// Main loop for optimizer (check for program shutdown)
	while (task.actual < task.total && !optimizer.IsEnd() && mgr.running) {
		// Get trial solution
		optimizer.Start();
		const Vector<double>& trial = optimizer.GetTrialSolution();
		
		// Collect probabilities for attributes
		// NOTE: sc[WEIGHTED==3] doesn't need to be zeroed (see CalculateWeightedGenderDifference)
		for(int mode = 0; mode < GENDER_COMMON_COUNT; mode++) {
			for(int j = 0; j < gc; j++)
				sc[mode][j] = 0;
			sorter[mode].Reset();
			int begin = offset[mode];
			int l = length[mode];
			int end = begin + l;
			
			// Set probability for attribute to not be used
			double mismatch_prob = max(0.5, 1.0 - (double)snap_max_per_mode / length[mode]);
			
			ASSERT(l > 0);
			for(int j = begin; j < end; j++) {
				double p = trial[j];
				if (p < mismatch_prob)
					continue;
				sorter[mode].Add(j, p);
			}
			
			
			// Calculate score of the trial solution
			int max_values = snap_max_per_mode; //mode == COMMON ? snap_max_values / 2 : snap_max_values;
			int mc = min(max_values, sorter[mode].count);
			for(int j = 0; j < mc; j++) {
				int attr_i = sorter[mode].key[j];
				ASSERT(attr_i >= 0 && attr_i < ma);
				const SnapAttrStr& sa = all_mas[attr_i];
				int score = db.attrscores.attr_to_score[sa.group_i][sa.item_i];
				ASSERT(score >= 0);
				const AttrScoreGroup& ag = db.attrscores.groups[score];
				ASSERT(ag.scores.GetCount() == gc);
				const int* fsc = ag.scores.Begin();
				for(int k = 0; k < gc; k++)
					sc[mode][k] += fsc[k];
			}
		}
		
		// Calculate weighted difference
		CalculateWeightedGenderDifference(score[WEIGHTED], score[MALE], score[FEMALE]);
		
		// Calculate energy
		double av_energy = 0;
		for (int mode = 0; mode < GENDER_COMMON_WEIGHTED_COUNT; mode++) {
			double energy = 0;
			/*if (0) {
				int max_values = mode == 0 ? snap_max_values / 2 : snap_max_values;
				int mc = min(max_values, sorter[mode].count);
				for(int i = 0; i < gc; i++) {
					double a = sc[mode][i];
					double b = comp[mode][i];
					double diff = fabs(a - b);
					energy -= diff;
				}
				double penalty = (snap_max_values - mc) * 0.1;
				energy -= penalty;
			}
			else*/ if (0) {
				for(int i = 0; i < gc; i++) {
					double a = sc[mode][i];
					double b = comp[mode][i];
					double diff = fabs(a - b);
					bool sgn_mismatch = (a > 0) == (b > 0);
					bool large_value = fabs(a) > fabs(b);
					double mul = (sgn_mismatch ? 2 : 1) * (large_value ? 2 : 1);
					energy -= diff * mul;
				}
			}
			else {
				double sum_X = 0, sum_Y = 0, sum_XY = 0;
				double squareSum_X = 0, squareSum_Y = 0;
				for(int i = 0; i < gc; i++) {
					double a = sc[mode][i];
					double b = comp[mode][i];
					//if (!a) continue;
					
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
				double mul = (gc * squareSum_Y - sum_Y * sum_Y);
				double sq = sqrt((gc * squareSum_X - sum_X * sum_X) * mul);
				double corr = (float)(gc * sum_XY - sum_X * sum_Y) / sq;
				
				if (!IsFin(corr)) {
					/*DUMP(sum_X);
					DUMP(sum_Y);
					DUMP(sum_XY);
					DUMP(squareSum_X);
					DUMP(squareSum_Y);
					DUMP(sq);
					DUMP(mul);
					ASSERT(0);*/
					corr = -1;
				}
				energy = corr;
			}
			av_energy += energy * (1.0 / (GENDER_COMMON_WEIGHTED_COUNT));
		}
		
		// Check if this is the best result so far
		bool new_best = av_energy > optimizer.GetBestEnergy();
		
		// Collect result values coarsely for gui
		if ((task.actual % 100) == 0 || new_best) {
			task.result_values.Add(av_energy);
			task.values_max = max(av_energy, task.values_max);
			task.values_min = min(av_energy, task.values_min);
		}
		
		// Collect best result
		if (new_best) {
			LOG("Task #" << IntStr64((int64)this) << " best energy: " << av_energy);
			task.lock.EnterWrite();
			TaskResult& res = task.results.Add();
			res.optimizer_score = av_energy;
			res.id = task.actual;
			task.lock.LeaveWrite();
		}
		
		// Return trial's result to the optimizer too
		optimizer.Stop(av_energy);
		
		// Keep track of progress
		task.actual++;
		
		// Check for internal errors
		if (av_energy == 0)
			break;
	}
	
	// Exit fast, if program is shutting down
	if (!mgr.running) {
		task.active = false;
		task.Store();
		SetError(t_("Interrupted"));
		return;
	}
	
	
	// Use the best result
	{
		task.lock.EnterWrite();
		
		// Clear values (loop for genders)
		task.result_attrs.SetCount(GENDER_COUNT);
		for (int mode = 0; mode < GENDER_COUNT; mode++) {
			task.result_attrs[mode].Clear();
			PatternSnap& snap = ctx.snap[MALE_REVERSED + mode];
			snap.attributes.Clear();
		}
		
		// Clear values (loop for common + separate genders)
		for (int mode = 0; mode < GENDER_COMMON_COUNT; mode++) {
			sorter[mode].Reset();
			for(int j = 0; j < gc; j++)
				sc[mode][j] = 0;
		}
		
		// Get the best trial solution from the optimizer
		const Vector<double>& best = optimizer.GetBestSolution();
		
		// Use same sorter as in trial solution algorithm
		
		// Add values to the sorter
		for(int mode = 0; mode < GENDER_COMMON_COUNT; mode++) {
			int begin = offset[mode];
			int l = length[mode];
			int end = begin + l;
			ASSERT(l > 0);
			
			// Set probability for attribute to not be used
			double mismatch_prob = max(0.5, 1.0 - (double)snap_max_per_mode / length[mode]);
			
			for(int j = begin; j < end; j++) {
				double p = best[j];
				if (p < mismatch_prob)
					continue;
				sorter[mode].Add(j, p);
			}
		}
		
		// Separate loop is needed, because all sorters are being read
		task.result_attrs.SetCount(GENDER_COMMON_COUNT);
		for(int mode = 0; mode < GENDER_COMMON_COUNT; mode++) {
			int max_values = snap_max_per_mode; //mode == COMMON ? snap_max_values / 2 : snap_max_values;
			int mc = min(max_values, sorter[mode].count);
			
			task.result_attrs[mode].Clear();
			
			if (!mc) {
				SetError("got no attributes for " + GetCommonModeString(mode));
				return;
			}
			
			// Add common attributes to all snaps
			if (mode == COMMON) {
				for (int mode0 = 0; mode0 < GENDER_COUNT; mode0++) {
					PatternSnap& snap = ctx.snap[MALE_REVERSED + mode0];
					for(int i = 0; i < mc; i++) {
						int attr_i = sorter[mode].key[i];
						const SnapAttrStr& sa = all_mas[attr_i];
						
						// Add result for the gui (management)
						task.result_attrs[mode].Add(sa);
						
						// Add result for the database (and pipeline)
						snap.attributes.FindAdd(sa);
					}
				}
			}
			// Add separate attributes to single genders
			else {
				PatternSnap& snap = ctx.snap[MALE_REVERSED + mode];
				for(int i = 0; i < mc; i++) {
					int attr_i = sorter[mode].key[i];
					const SnapAttrStr& sa = all_mas[attr_i];
					
					// Add result for the gui (management)
					task.result_attrs[mode].Add(sa);
					
					// Add result for the database (and pipeline)
					snap.attributes.FindAdd(sa);
				}
			}
		}
		task.lock.LeaveWrite();
	}
	
	// merge common values to owners in snaps
	for(int mode = 0; mode < GENDER_COUNT; mode++)
		ctx.snap[MALE_REVERSED + mode].part->MergeOwner();
	
	if (optimizer.IsEnd())
		optimizer.ClearPopulation();
	
	task.active = false;
	task.Store();
}
