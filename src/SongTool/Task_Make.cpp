#include "SongTool.h"

void Task::Process_MakeImportTasks() {
	const Database& db = Database::Single();
	Song& song = *p.song;
	const Artist& artist = *p.artist;
	ASSERT(p.artist);
	
	{
		song.DeepClearSnap();
		
		if (!ParseOriginalLyrics())
			return;
	}
	
	for (GroupContext ctx = CTX_BEGIN; ctx != CTX_COUNT; ((int&)ctx)++) {
		for(int i = 0; i < db.attrs.group_types.GetCount(); i++) {
			for(int j = 0; j < GENDER_COUNT; j++) {
				const Attributes::GroupType& group_type = db.attrs.group_types[i];
				
				// Skip different context
				if (group_type.group_ctx != ctx)
					continue;
				
				// Add task for type
				Task& t = ResultTask(TASK_PATTERNMASK);
				t.p = p;
				t.p.mode = j;
				t.p.group_ctx = ctx;
				t.args << group_type.name << artist.vocalist_visual;
			}
		}
	}
	{
		for(int j = 0; j < GENDER_COUNT; j++) {
			Task& t = ResultTask(TASK_STORYARC);
			t.p = p;
			t.p.mode = j;
			t.p.group_ctx = CTX_BEGIN;
		}
	}
	
	{
		for(int j = 0; j < GENDER_COUNT; j++) {
			Vector<PatternSnap*> snaps;
			song.GetSnapsLevel(j, 1, snaps);
			const SongHeader& h = song.headers[j];
			Index<String> unique_lines;
			for (PatternSnap* snap : snaps) {
				if (unique_lines.Find(snap->txt) >= 0)
					continue;
				unique_lines.Add(snap->txt);
				
				Task& t = ResultTask(TASK_IMPACT);
				t.p.CopyPtrs(*snap);
				t.p.snap = snap;
				t.p.mode = j;
				t.p.group_ctx = CTX_BEGIN;
				ASSERT(t.p.snap && t.p.line);
			}
		}
	}
	
	{
		for(int j = 0; j < GENDER_COUNT; j++) {
			Task& t = ResultTask(TASK_MAKE_IMPACT_SCORING_TASKS);
			t.p = p;
			t.p.mode = j;
			t.p.group_ctx = CTX_BEGIN;
		}
	}
	
	{
		for (int whole_song = 0; whole_song < 2; whole_song++) {
			for(int i = 0; i < db.attrs.analysis.GetCount(); i++) {
				for(int j = 0; j < GENDER_COUNT; j++) {
					String key = db.attrs.analysis.GetKey(i);
					
					// Add task for analysis type
					Task& t = ResultTask(TASK_ANALYSIS);
					t.p = p;
					t.p.mode = j;
					t.args << artist.vocalist_visual;
					t.args << key;
					t.whole_song = whole_song;
					t.p.group_ctx = CTX_BEGIN;
					
					const Vector<String>& v = db.attrs.analysis[i];
					for(int j = 0; j < v.GetCount(); j++)
						t.args << v[j];
				}
			}
		}
	}
}

bool Task::ParseOriginalLyrics() {
	Database& db = Database::Single();
	if (!p.song)
		return false;
	
	
	Song& song = *p.song;
	String structure_str;
	for(int mode = 0; mode < 2; mode++) {
		String c = song.headers[mode].content;
		c.Replace("\r", "");
		Vector<String> parts = Split(c, "\n\n");
		
		String str;
		for(int i = 0; i < parts.GetCount(); i++) {
			Vector<String> lines = Split(parts[i], "\n");
			int lc = lines.GetCount()-1;
			String part_title = TrimBoth(lines[0]);
			if (part_title.Right(1) == ":")
				part_title = part_title.Left(part_title.GetCount()-1);
			if (!str.IsEmpty())
				str << ",";
			str << part_title;
			if (lc) str << ":" << lc;
			for(int j = 1; j < lines.GetCount(); j++) {
				Vector<String> breaks = Split(lines[j], "[br]");
				str << ":" << breaks.GetCount();
			}
		}
		//DUMP(str);
		if (structure_str.IsEmpty())
			structure_str = str;
		else {
			if (structure_str != str) {
				int len = min(structure_str.GetCount(), str.GetCount());
				String mismatch;
				for(int j = 0; j < len; j++) {
					if (structure_str[j] != str[j]) {
						mismatch = str.Mid(j, 16);
						break;
					}
				}
				SetError("structure mismatch at '" + mismatch + "'");
				return false;
			}
		}
	}
	
	song.lock.EnterWrite();
	
	song.parts.Clear();
	song.structure.Clear();
	
	// Structure string
	song.structure_str = structure_str;
	song.structure.Clear();
	
	//DUMP(a.structure_str);
	song.ReloadStructure(); // p.unique_parts is filled here
	
	// Parse lyric parts and lines
	Vector<Vector<String>>& unique_lines = str_map;
	for(int i = 0; i < PTR_COUNT; i++)
		song.headers[i].unique_lines.Clear();
	
	
	for(int mode = 0; mode < GENDER_COUNT; mode++) {
		String c = song.headers[mode].content;
		c.Replace("\r", "");
		Vector<String> parts = Split(c, "\n\n");
		
		for(int i = 0; i < parts.GetCount(); i++) {
			Vector<String> lines = Split(parts[i], "\n");
			int lc = lines.GetCount()-1;
			if (lines.GetCount() >= 1) {
				String part_title = TrimBoth(lines[0]);
				if (part_title.Right(1) == ":")
					part_title = part_title.Left(part_title.GetCount()-1);
				
				if (lc) {
					Part& part = song.GetAddPart(part_title);
					Array<Line>& parsed_lines = part.lines;
					parsed_lines.SetCount(lines.GetCount()-1);
					String& txt = part.snap[mode].txt;
					
					// Add parsed lines to the Song class
					for(int j = 1; j < lines.GetCount(); j++) {
						String tl = TrimBoth(lines[j]);
						Line& l = parsed_lines[j-1];
						l.ParseLine(song, mode, tl);
						ASSERT(l.snap[mode].txt.GetCount());
						
						if (!txt.IsEmpty()) txt << "; ";
						txt << l.snap[mode].txt;
					}
					
					song.headers[mode].unique_lines.GetAdd(txt);
				}
			}
		}
	}
	
	song.FixPtrs();
	song.RealizeTaskSnaps(true);
	song.lock.LeaveWrite();
	
	return true;
}


void Task::Process_MakeAttrScores() {
	Database& db = Database::Single();
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
	song.GetMaskAttributes(mode, song_attrs); // get attrs from snapshots
	
	Index<int> unresolved_group_types;
	for (const SnapAttrStr& a : song_attrs.GetKeys()) {
		ASSERT(a.has_id);
		const Attributes::Group& gg = attrs.groups[a.group_i];
		ASSERT(gg.type_i >= 0);
		const Attributes::GroupType& group_type = attrs.group_types[gg.type_i];
		
		// Skip different context
		if (group_type.group_ctx != p.group_ctx)
			continue;
		
		// Skip groups, which doesn't match this task
		ASSERT(gg.type.GetCount());
		ASSERT(group_type.name.GetCount());
		if (gg.type.GetCount() != group_type.name.GetCount() ||
			ToLower(gg.type) != ToLower(group_type.name))
			continue;
		
		// Check from 'attribute to score' conversion vector, if all is solved
		bool is_unresolved = false;
		if (a.group_i >= as.attr_to_score.GetCount())
			is_unresolved = true;
		else {
			const Vector<int>& to_score_i = as.attr_to_score[a.group_i];
			// Check if attribute is without score
			if (a.item_i >= to_score_i.GetCount() || to_score_i[a.item_i] < 0) {
				// Not found
				is_unresolved = true;
			}
		}
		
		// If group have attributes without known scores (then add task to get them)
		if (is_unresolved) {
			unresolved_group_types.FindAdd(gg.type_i);
		}
	}
	
	if (unresolved_group_types.GetCount()) {
		//Task& chk = ResultTask(TASK_MAKE_ATTRSCORES_TASKS);
		//chk.p = this->p;
		
		for (int group_type_i : unresolved_group_types.GetKeys()) {
			const Attributes::GroupType& group_type = attrs.group_types[group_type_i];
			
			Task& t = ResultTask(TASK_ATTRSCORES);
			t.p = this->p;
			t.args << group_type.name;
			
			// Prevent stupid loops
			/*
			t.CreateInput();
			ASSERT(!t.failed);
			ASSERT(t.input.GetCount());for(int i = 0; i < m.tasks.GetCount()-1; i++) {
				if (m.tasks[i].input == t.input) {
					this->input = t.input;
					SetError("duplicate task is being made");
					return;
				}
			}*/
		}
		
		// Allow TASK_MAKE_ATTRSCORES_TASKS to be spawned once again
		allow_multi_spawn = true;
	}
	
	// If all were resolved
	else {
		// Add dummy task to flag that O_DB_ATTR_SCORES is available
		Task& tr = ResultTask(TASK_ATTRSCORES_READY);
		tr.p = this->p;
		
		Task& t = ResultTask(TASK_SONGSCORE);
		t.p = this->p;
		
		//Task& chk = ResultTask(TASK_MAKE_REVERSE_IMPACT_TASK);
		//chk.p = this->p;
	}
}

void Task::Process_MakePatternTasks() {
	Database& db = Database::Single();
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
	
	//Task& chk = ResultTask(TASK_MAKE_ATTRSCORES_TASKS);
	//chk.p = this->p;
	
	// Pattern tasks -> attribute score task maker
	{
		for(int i = 0; i < db.attrs.group_types.GetCount(); i++) {
			const Attributes::GroupType& group_type = db.attrs.group_types[i];
			
			// Skip different context
			if (group_type.group_ctx != p.group_ctx)
				continue;
			
			for(int j = 0; j < tasks; j++) {
				Task& t = ResultTask(TASK_PATTERN);
				t.p = this->p;
				t.args << group_type.name << IntStr(j * per_task) << IntStr((j + 1) * per_task);
			}
		}
	}
}

void Task::Process_MakeImpactScoringTasks() {
	Database& db = Database::Single();
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
	
	//Task& chk = ResultTask(TASK_MAKE_IMPACT_SCORING_TASKS);
	//chk.p = this->p;
	
	int per_task = 30;
	int tasks = 1 + (line_impacts.GetCount() + 1) / per_task;
	
	int impact_i = 0;
	for(int i = 0; i < tasks; i++) {
		Task& t = ResultTask(TASK_IMPACT_SCORING);
		t.p = this->p;
		for(int i = 0; i < per_task; i++) {
			if (impact_i >= line_impacts.GetCount())
				break;
			t.args << line_impacts[impact_i];
			impact_i++;
		}
	}
}

void Task::Process_MakeReverseImpactTask() {
	//Task& chk = ResultTask(TASK_MAKE_REVERSE_MASK_TASK);
	//chk.p = p;
	
	Task& t0 = ResultTask(TASK_REVERSE_IMPACT);
	t0.p = p;
}

void Task::Process_MakeReverseMaskTask() {
	Database& db = Database::Single();
	Attributes& g = db.attrs;
	Song& song = *p.song;
	int gc = g.scorings.GetCount();
	
	// Mode is joined to 0 to find common mask
	if (p.mode > 0) {
		return;
	}
	/*else {
		// Check that other modes are ready
		for (Task& t : m.tasks) {
			if (&t == this) continue;
			if (t.p.song == p.song && t.type == TASK_MAKE_REVERSE_MASK_TASK) {
				ASSERT(t.p.mode > 0);
				if (t.p.mode > 0 && !t.ready) {
					SetWaiting();
					return;
				}
			}
		}
	}*/
	
	song.lock.EnterWrite();
	song.rev_common_mask_tasks.Clear();
	song.rev_separate_mask_tasks.Clear();
	
	//Task& chk = ResultTask(TASK_MAKE_REVERSEPATTERN_TASK);
	//chk.p = p;
	
	for (Part& part : song.parts) {
		Task* prev = 0;
		// Tasks for common and separate parts
		for(int i = 0; i < 2; i++) {
			int rev_mode = p.mode == MALE ? MALE_REVERSED : FEMALE_REVERSED;
			int type = i == 0 ? TASK_REVERSE_COMMON_MASK : TASK_REVERSE_SEPARATE_MASK;
			
			Task& t = ResultTask(type);
			t.p = p;
			t.p.part = &part;
			
			Array<ReverseTask>& tasks = i == 0 ? song.rev_common_mask_tasks : song.rev_separate_mask_tasks;
			ReverseTask& rt = tasks.Add();
			rt.type = type;
			rt.result_attrs.SetCount(2);
			t.task = &rt;
			
			rt.ctx = &part;
			
			
			// Input arguments ( = hash affecting values)
			rt.txt = part.snap[i].txt;
			if (rt.txt.IsEmpty()) {
				SetError("snap text empty");
				t.failed = true;
				//song.lock.LeaveWrite();
				//return;
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
	
	// Try to load existing results
	for (ReverseTask& rt : song.rev_common_mask_tasks)
		rt.LoadHash(rt.GetHashValue());
	for (ReverseTask& rt : song.rev_separate_mask_tasks)
		rt.LoadHash(rt.GetHashValue());
	
}

void Task::Process_MakeReversePattern() {
	Database& db = Database::Single();
	Attributes& g = db.attrs;
	AttrScore& as = db.attrscores;
	if (!p.song) {
		SetError("no song pointer set");
		return;
	}
	Song& song = *p.song;
	
	
	as.UpdateGroupsToScoring();
	
	// Check that impact scores are ready
	/*for (Task& t : m.tasks) {
		if (t.type == TASK_MAKE_IMPACT_SCORING_TASKS && t.p.song == p.song) {
			if (!t.ready || t.failed) {
				SetWaiting();
				return;
			}
		}
	}*/
	
	//Task& chk = ResultTask(TASK_MAKE_LYRICS_TASK);
	//chk.p = p;
	int mode = p.mode;
	ASSERT(mode == 0); // all are being done in one mode
	//int rev_mode = p.mode == MALE ? MALE_REVERSED : FEMALE_REVERSED;
	
	int gc = g.scorings.GetCount();
	int ac = db.attrscores.groups.GetCount();
	int total = ac * snap_gen_multiplier * snap_gens;
	
	song.lock.EnterWrite();
	song.rev_pattern_tasks.Clear();
	int part_i = -1;
	for (Part& part : song.parts) {
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
		
		
		for(int i = 0; i < part.lines.GetCount(); i++) {
			Line& line = part.lines[i];
			for(int j = 0; j < line.breaks.GetCount(); j++) {
				Break& brk = line.breaks[j];
				
				//PatternSnap& snap = *snaps[i];
				//PatternSnap& rev_snap = *rev_snaps[i];
				ReverseTask& rt = song.rev_pattern_tasks.Add();
				Task& t = ResultTask(TASK_REVERSEPATTERN);
				t.p = this->p;
				t.task = &rt;
				//t.id = i;
				//t.total = total;
				rt.ctx = &brk;
				rt.type = TASK_REVERSEPATTERN;
				
				
				// Input arguments ( = hash affecting values)
				
				// Snapshot text (for hash purposes only)
				rt.txt = brk.snap[0].txt; // + "; " + brk.snap[1].txt;
				/*if (rt.txt.IsEmpty()) {
					SetError("snap text empty");
					t.failed = true;
					song.lock.LeaveWrite();
					m.lock.LeaveWrite();
					return;
				}*/
				
				
				// Add mask from genders
				// - get int from += 1 << i, highest sum means that it's common
				// - sort mask by value
				rt.mask_attrs.Clear();
				for(int i = 0; i < GENDER_COUNT; i++) {
					int code = 1 << i;
					const PatternSnap& snap = part.snap[MALE_REVERSED + i];
					ASSERT(!snap.mask.IsEmpty());
					for (const SnapAttrStr& sa : snap.mask.GetKeys()) {
						rt.mask_attrs.GetAdd(sa, 0) += code;
					}
				}
				ASSERT(!rt.mask_attrs.IsEmpty());
				SortByKey(rt.mask_attrs, SnapAttrStr()); // required for same hash value
				SortByValue(rt.mask_attrs, StdLess<int>());
				
				for(int j = 0; j < rt.mask_attrs.GetCount(); j++) {
					const SnapAttrStr& sa = rt.mask_attrs.GetKey(j);
					int score = db.attrscores.attr_to_score[sa.group_i][sa.item_i];
					if (score < 0) {
						//LOG("warning: attribute did not have the scoring: " << sa.group << ", " << sa.item);
						rt.mask_attrs.Remove(j--);
					}
				}
				// Check that previous code didn't remove all gender attributes
				for(int i = 0; i < GENDER_COUNT; i++) {
					int code = 1 << i;
					int count = 0;
					for(int j = 0; j < rt.mask_attrs.GetCount(); j++)
						if (rt.mask_attrs[j] == code)
							count++;
					ASSERT(count);
					if (!count) {
						SetError("all gender attributes were removed because of lacking attribute scores");
						song.lock.LeaveWrite();
						return;
					}
				}
				/*if (rt.mask_attrs.IsEmpty()) {
					SetError("no snap mask attributes");
					t.failed = true;
					song.lock.LeaveWrite();
					return;
				}*/
				
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
				SortByValue(snap_attrs, StdLess<int>());
				
				// Calculate scores for common and separate attribute vectors
				Vector<Vector<double>>& src_scores = rt.scores;
				src_scores.SetCount(GENDER_COMMON_WEIGHTED_COUNT);
				for(int i = 0; i < GENDER_COMMON_COUNT; i++) {
					Vector<double>& src_score = src_scores[i];
					src_score.SetCount(0);
					src_score.SetCount(gc, 0);
					int match = i+1; static_assert(GENDER_COUNT == 2, "check this");
					int no_score_count = 0;
					int match_count = 0;
					for(int j = 0; j < snap_attrs.GetCount(); j++) {
						if (snap_attrs[j] != match)
							continue;
						const SnapAttrStr& sa = snap_attrs.GetKey(j);
						ASSERT(sa.has_id);
						int score_i = as.attr_to_score[sa.group_i][sa.item_i];
						if (score_i >= 0) {
							const auto& score = as.groups[score_i].scores;
							ASSERT(score.GetCount() == gc);
							for(int k = 0; k < gc; k++)
								src_score[k] += score[k];
							match_count++;
						}
						else no_score_count++;
					}
					if (!match_count) {
						LOG("warning: no matches for mode: " << GetCommonModeString(i));
					}
					if (no_score_count) {
						LOG("warning: no score count: " << no_score_count);
					}
				}
				
				// Calculate last score: weighted separate
				{
					const Vector<double>& src_score0 = src_scores[MALE];
					const Vector<double>& src_score1 = src_scores[FEMALE];
					Vector<double>& dst_score = src_scores[WEIGHTED];
					dst_score.SetCount(gc);
					CalculateWeightedGenderDifference(dst_score, src_score0, src_score1);
				}
				
				// Try to load existing results
				rt.LoadHash(rt.GetHashValue());
				
			}
		}
	}
	song.lock.LeaveWrite();
}

void Task::Process_MakeLyricsTask() {
	Database& db = Database::Single();
	if (!p.song) {
		SetError("no song pointer set");
		return;
	}
	
	Song& song = *p.song;
	Vector<Task*> tasks;
	for (Part& part : song.parts) {
		Task& t = ResultTask(TASK_LYRICS);
		t.p = p;
		t.p.part = &part;
		t.args << "rev";
		tasks << &t;
	}
	
	if (GetCurrentLanguage() != LNG_('E', 'N', 'U', 'S')) {
		Task& t = ResultTask(TASK_LYRICS_TRANSLATE);
		t.p = p;
		t.args << "rev" << GetCurrentLanguageString();
	}
}
