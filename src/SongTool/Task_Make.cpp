#include "SongTool.h"

void AI_Task::Process_MakeAttrScores() {
	Database& db = Database::Single();
	TaskMgr& m = TaskMgr::Single();
	Attributes& attrs = db.attrs;
	AttrScore& as = db.attrscores;
	if (!p.song) {
		SetError("no song pointer set");
		return;
	}
	
	if (as.groups.IsEmpty())
		as.attr_to_score.Clear();
	
	Song& song = *this->p.song;
	ASSERT(p.mode >= 0);
	int mode = p.mode;
	
	if (!as.UpdateGroupsToScoring()) {
		SetError("AttrScore::UpdateGroupsToScoring failed");
		return;
	}
	
	Index<SnapAttrStr> song_attrs;
	song.GetAttributes(mode, song_attrs); // get attrs from snapshots
	
	Index<int> unresolved_group_types;
	for (const SnapAttrStr& a : song_attrs.GetKeys()) {
		ASSERT(a.has_id);
		const Attributes::Group& gg = attrs.groups[a.group_i];
		ASSERT(gg.type_i >= 0);
		const Attributes::GroupType& group_type = attrs.group_types[gg.type_i];
		
		
		// Skip groups, which doesn't match this task
		if (gg.type != group_type.name)
			continue;
		
		// Check from 'attribute to score' conversion vector, if all is solved
		if (a.group_i >= as.attr_to_score.GetCount())
			break;
		const Vector<int>& to_score_i = as.attr_to_score[a.group_i];
		bool is_unresolved = false;
		// Check if attribute is without score
		if (a.item_i >= to_score_i.GetCount() || to_score_i[a.item_i] < 0) {
			// Not found
			is_unresolved = true;
		}
		
		
		// If group have attributes without known scores (then add task to get them)
		if (is_unresolved) {
			unresolved_group_types.FindAdd(gg.type_i);
		}
	}
	
	m.lock.EnterWrite();
	
	if (unresolved_group_types.GetCount()) {
		int task_begin = m.tasks.GetCount();
		AI_Task& chk = m.tasks.Add();
		chk.type = TASK_MAKE_ATTRSCORES_TASKS;
		chk.p = this->p;
		
		for (int group_type_i : unresolved_group_types.GetKeys()) {
			const Attributes::GroupType& group_type = attrs.group_types[group_type_i];
			
			AI_Task& t = m.tasks.Add();
			m.total++;
			t.type = TASK_ATTRSCORES;
			t.p = this->p;
			t.args << group_type.name << group_type.ai_txt;
			t.CreateInput();
			ASSERT(!t.failed);
			ASSERT(t.input.GetCount());
			
			// Prevent stupid loops
			for(int i = 0; i < m.tasks.GetCount()-1; i++) {
				if (m.tasks[i].input == t.input) {
					this->input = t.input;
					SetError("duplicate task is being made");
					m.tasks.Remove(task_begin, m.tasks.GetCount()-task_begin);
					m.lock.LeaveWrite();
					return;
				}
			}
			
			chk.depends_on << &t;
		}
	}
	
	// If all were resolved
	else {
		AI_Task& t = m.tasks.Add();
		m.total++;
		t.type = TASK_SONGSCORE;
		t.p = this->p;
		t.CreateInput();
		
		AI_Task& chk = m.tasks.Add();
		m.total++;
		chk.type = TASK_MAKE_REVERSE_IMPACT_TASK;
		chk.p = this->p;
		chk.depends_on << &t;
	}
	
	m.lock.LeaveWrite();
}

void AI_Task::Process_MakePatternTasks() {
	Database& db = Database::Single();
	TaskMgr& m = TaskMgr::Single();
	int mode = p.mode;
	ASSERT(mode >= 0);
	Song& song = *p.song;
	SongHeader& header = song.headers[mode];
	
	int per_task = 30;
	int tasks = 1 + header.unique_lines.GetCount() / per_task;
	
	if (!p.song) {
		SetError("no song pointer set");
		return;
	}
	
	m.lock.EnterWrite();
	
	AI_Task& chk = m.tasks.Add();
	chk.type = TASK_MAKE_ATTRSCORES_TASKS;
	chk.p = this->p;
	
	// Pattern tasks -> attribute score task maker
	{
		for(int i = 0; i < db.attrs.group_types.GetCount(); i++) {
			const Attributes::GroupType& group_type = db.attrs.group_types[i];
			
			for(int j = 0; j < tasks; j++) {
				AI_Task& t = m.tasks.Add();
				m.total++;
				t.type = TASK_PATTERN;
				t.p = this->p;
				t.args << group_type.name << group_type.ai_txt << IntStr(j * per_task) << IntStr((j + 1) * per_task);
				t.CreateInput();
				chk.depends_on << &t;
			}
		}
	}
	
	m.lock.LeaveWrite();
}

void AI_Task::Process_MakeImpactScoringTasks() {
	Database& db = Database::Single();
	TaskMgr& m = TaskMgr::Single();
	int mode = p.mode;
	ASSERT(mode >= 0);
	Song& song = *p.song;
	SongHeader& header = song.headers[mode];
	
	if (!p.song) {
		SetError("no song pointer set");
		return;
	}
	

	Index<String> line_impacts;
	for(int i = 0; i < song.parts.GetCount(); i++) {
		Part& part = song.parts[i];
		for(int j = 0; j < part.lines.GetCount(); j++) {
			Line& line = part.lines[j];
			for(int k = 0; k < line.breaks.GetCount(); k++) {
				Break& brk = line.breaks[k];
				PatternSnap& snap = brk.snap[mode];
				if (snap.impact.GetCount() && snap.impact_score.IsEmpty()) {
					line_impacts.FindAdd(snap.impact);
				}
			}
		}
		
	}
	if (line_impacts.IsEmpty()) {
		return;
	}
	SortIndex(line_impacts, StdLess<String>());
	
	m.lock.EnterWrite();
	
	AI_Task& chk = m.tasks.Add();
	chk.type = TASK_MAKE_IMPACT_SCORING_TASKS;
	chk.p = this->p;
	
	int per_task = 30;
	int tasks = 1 + (line_impacts.GetCount() + 1) / per_task;
	
	int impact_i = 0;
	for(int i = 0; i < tasks; i++) {
		AI_Task& t = m.tasks.Add();
		m.total++;
		t.type = TASK_IMPACT_SCORING;
		t.p = this->p;
		for(int i = 0; i < per_task; i++) {
			if (impact_i >= line_impacts.GetCount())
				break;
			t.args << line_impacts[impact_i];
			impact_i++;
		}
		t.CreateInput();
		chk.depends_on << &t;
	}
	
	m.lock.LeaveWrite();
}

void AI_Task::Process_MakeReverseImpactTask() {
	TaskMgr& m = TaskMgr::Single();
	m.lock.EnterWrite();
	
	AI_Task& chk = m.tasks.Add();
	chk.type = TASK_MAKE_REVERSE_MASK_TASK;
	chk.p = p;
	
	AI_Task& t0 = m.tasks.Add();
	t0.type = TASK_REVERSE_IMPACT;
	t0.p = p;
	chk.depends_on << &t0;
	
	m.lock.LeaveWrite();
}

void AI_Task::Process_MakeReverseMaskTask() {
	Database& db = Database::Single();
	Attributes& g = db.attrs;
	TaskMgr& m = TaskMgr::Single();
	Song& song = *p.song;
	int gc = g.scorings.GetCount();
	
	// Mode is joined to 0 to find common mask
	if (p.mode > 0) {
		return;
	}
	else {
		// Check that other modes are ready
		for (AI_Task& t : m.tasks) {
			if (&t == this) continue;
			if (t.p.song == p.song && t.type == TASK_MAKE_REVERSE_MASK_TASK) {
				ASSERT(t.p.mode > 0);
				if (t.p.mode > 0 && !t.ready) {
					SetWaiting();
					return;
				}
			}
		}
	}
	
	m.lock.EnterWrite();
	
	song.lock.EnterWrite();
	song.rev_common_mask_tasks.Clear();
	song.rev_separate_mask_tasks.Clear();
	
	AI_Task& chk = m.tasks.Add();
	chk.type = TASK_MAKE_REVERSEPATTERN_TASK;
	chk.p = p;
	
	for (Part& part : song.parts) {
		AI_Task* prev = 0;
		for(int i = 0; i < 2; i++) {
			int rev_mode = p.mode == MALE ? MALE_REVERSED : FEMALE_REVERSED;
			
			AI_Task& t = m.tasks.Add();
			t.type = i == 0 ? TASK_REVERSE_COMMON_MASK : TASK_REVERSE_SEPARATE_MASK;
			t.p = p;
			t.p.part = &part;
			chk.depends_on << &t;
			if (prev)
				t.depends_on << prev;
			
			Array<ReverseTask>& tasks = i == 0 ? song.rev_common_mask_tasks : song.rev_separate_mask_tasks;
			ReverseTask& rt = tasks.Add();
			rt.type = t.type;
			rt.id = tasks.GetCount()-1;
			rt.result_attrs.SetCount(2);
			t.task = &rt;
			
			rt.ctx = &part;
			m.total++;
			
			
			// Input arguments ( = hash affecting values)
			rt.txt = part.snap[i].txt;
			if (rt.txt.IsEmpty()) {
				SetError("snap text empty");
				t.failed = true;
				song.lock.LeaveWrite();
				return;
			}
			rt.scores.SetCount(GENDER_COUNT);
			for(int i = 0; i < GENDER_COUNT; i++) {
				auto& score = rt.ctx->snap[i].partscore;
				ASSERT(score.GetCount() == gc);
				score.SetCount(gc, 0);
				rt.ctx->snap[MALE_REVERSED + i].partscore.SetCount(gc, 0);

				rt.scores[i].SetCount(score.GetCount());
				for(int j = 0; j < score.GetCount(); j++)
					rt.scores[i][j] = score[j];
			}
			
			prev = &t;
		}
	}
	
	song.lock.LeaveWrite();
	m.lock.LeaveWrite();
	
	// Try to load existing results
	for (ReverseTask& rt : song.rev_common_mask_tasks)
		rt.LoadHash(rt.GetHashValue());
	for (ReverseTask& rt : song.rev_separate_mask_tasks)
		rt.LoadHash(rt.GetHashValue());
	
}

void AI_Task::Process_MakeReversePattern() {
	Database& db = Database::Single();
	Attributes& g = db.attrs;
	AttrScore& as = db.attrscores;
	TaskMgr& m = TaskMgr::Single();
	if (!p.song) {
		SetError("no song pointer set");
		return;
	}
	Song& song = *p.song;
	
	// Check that impact scores are ready
	for (AI_Task& t : m.tasks) {
		if (t.type == TASK_MAKE_IMPACT_SCORING_TASKS && t.p.song == p.song) {
			if (!t.ready || t.failed) {
				SetWaiting();
				return;
			}
		}
	}
	
	m.lock.EnterWrite();
	
	AI_Task& chk = m.tasks.Add();
	chk.type = TASK_MAKE_LYRICS_TASK;
	chk.p = p;
	int mode = p.mode;
	ASSERT(mode == 0); // all are being done in one mode
	//int rev_mode = p.mode == MALE ? MALE_REVERSED : FEMALE_REVERSED;
	
	int gc = g.scorings.GetCount();
	int ac = db.attrscores.groups.GetCount();
	int total = ac * snap_gen_multiplier * snap_gens;
	
	song.lock.EnterWrite();
	song.rev_pattern_tasks.Clear();
	int part_i = -1;
	for (Part& p : song.parts) {
		part_i++;
		
		/*Vector<PatternSnap*> snaps, rev_snaps;
		revp.GetSnapsLevel(0, rev_snaps);
		p.GetSnapsLevel(0, snaps);
			
		if (snaps.GetCount() != rev_snaps.GetCount()) {
			SetError("snaps and rev-snaps count mismatch");
			song.lock.LeaveWrite();
			m.lock.LeaveWrite();
			return;
		}*/
		for(int i = 0; i < p.lines.GetCount(); i++) {
			Line& line = p.lines[i];
			for(int j = 0; j < line.breaks.GetCount(); j++) {
				Break& brk = line.breaks[j];
				
				//PatternSnap& snap = *snaps[i];
				//PatternSnap& rev_snap = *rev_snaps[i];
				ReverseTask& rt = song.rev_pattern_tasks.Add();
				AI_Task& t = m.tasks.Add();
				t.type = TASK_REVERSEPATTERN;
				t.p = this->p;
				t.task = &rt;
				//t.id = i;
				//t.total = total;
				rt.type = t.type;
				//rt.snap = &brk.snap[mode];
				//rt.rev_snap = &brk.snap[rev_mode];
				rt.ctx = &brk;
				chk.depends_on << &t;
				m.total++;
				
				
				// Input arguments ( = hash affecting values)
				
				// Snapshot text (for hash purposes only)
				rt.txt = brk.snap[0].txt; // + "; " + brk.snap[1].txt;
				if (rt.txt.IsEmpty()) {
					SetError("snap text empty");
					t.failed = true;
					song.lock.LeaveWrite();
					m.lock.LeaveWrite();
					return;
				}
				
				
				// Add mask from genders
				// - get int from += 1 << i, highest sum means that it's common
				// - sort mask by value
				rt.mask_attrs.Clear();
				for(int i = 0; i < GENDER_COUNT; i++) {
					int code = 1 << i;
					const PatternSnap& snap = rt.ctx->snap[i];
					for (const SnapAttrStr& sa : snap.mask.GetKeys()) {
						rt.mask_attrs.GetAdd(sa, 0) += code;
					}
				}
				SortByKey(rt.mask_attrs, SnapAttrStr()); // required for same hash value
				SortByValue(rt.mask_attrs, StdGreater<int>());
				
				for(int j = 0; j < rt.mask_attrs.GetCount(); j++) {
					const SnapAttrStr& sa = rt.mask_attrs.GetKey(j);
					int score = db.attrscores.attr_to_score[sa.group_i][sa.item_i];
					if (score < 0) {
						//LOG("warning: attribute did not have the scoring: " << sa.group << ", " << sa.item);
						rt.mask_attrs.Remove(j--);
					}
				}
				if (rt.mask_attrs.IsEmpty()) {
					SetError("no snap mask attributes");
					t.failed = true;
					song.lock.LeaveWrite();
					m.lock.LeaveWrite();
					return;
				}
				
				// Gender scores requires knowing common and separate attributes
				// Calculate them here, as there is no database structure for that
				VectorMap<SnapAttrStr,int>& snap_attrs = rt.snap_attrs;
				snap_attrs.Clear();
				for(int i = 0; i < GENDER_COUNT; i++) {
					int code = 1 << i;
					const PatternSnap& snap = rt.ctx->snap[i];
					for (const SnapAttrStr& sa : snap.attributes.GetKeys()) {
						snap_attrs.GetAdd(sa, 0) += code;
					}
				}
				SortByValue(snap_attrs, StdGreater<int>());
				
				// Calculate scores for common and separate attribute vectors
				Vector<Vector<double>>& src_scores = rt.scores;
				src_scores.SetCount(COMMON_GENDER_WEIGHTED_COUNT);
				for(int i = 0; i < COMMON_GENDER_COUNT; i++) {
					Vector<double>& src_score = src_scores[i];
					src_score.SetCount(0);
					src_score.SetCount(gc, 0);
					int match = i == 0 ? 3 : (1 << (i-1));
					int match_count = 0;
					for(int j = 0; j < snap_attrs.GetCount(); j++) {
						if (snap_attrs[j] != match)
							continue;
						const SnapAttrStr& sa = snap_attrs.GetKey(j);
						ASSERT(sa.has_id);
						int score_i = as.attr_to_score[sa.group_i][sa.item_i];
						const auto& score = as.groups[score_i].scores;
						ASSERT(score.GetCount() == gc);
						for(int k = 0; k < gc; k++)
							src_score[k] += score[k];
						match_count++;
					}
					ASSERT(match_count > 0);
				}
				
				// Calculate last score: weighted separate
				{
					const Vector<double>& src_score0 = src_scores[1]; // MALE
					const Vector<double>& src_score1 = src_scores[2]; // FEMALE
					Vector<double>& dst_score = src_scores[3];
					dst_score.SetCount(gc);
					CalculateWeightedGenderDifference(dst_score, src_score0, src_score1);
				}
				
				// Try to load existing results
				rt.LoadHash(rt.GetHashValue());
				
			}
		}
	}
	song.lock.LeaveWrite();
	m.lock.LeaveWrite();
}

void AI_Task::Process_MakeLyricsTask() {
	Database& db = Database::Single();
	TaskMgr& m = TaskMgr::Single();
	if (!p.song) {
		SetError("no song pointer set");
		return;
	}
	
	m.lock.EnterWrite();
	
	Song& song = *p.song;
	Vector<AI_Task*> tasks;
	for (Part& part : song.parts) {
		AI_Task& t = m.tasks.Add();
		t.type = TASK_LYRICS;
		t.p = p;
		t.p.part = &part;
		t.args << "rev";
		t.CreateInput();
		tasks << &t;
	}
	
	if (GetCurrentLanguage() != LNG_('E', 'N', 'U', 'S')) {
		AI_Task& t = m.tasks.Add();
		t.type = TASK_LYRICS_TRANSLATE;
		t.p = p;
		t.depends_on <<= tasks;
		t.args << "rev" << GetCurrentLanguageString();
	}
	
	m.lock.LeaveWrite();
}
