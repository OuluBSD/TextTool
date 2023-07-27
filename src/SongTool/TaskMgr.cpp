#include "SongTool.h"



void TaskMgr::CreateDefaultTaskRules() {
	
	AddRule(TASK_IMPORT_AND_REVERSE, "import song and make reversed song")
		.Process(&Task::Process_MakeImportTasks)
			.Arg(V_PTR_SONG)
			.Arg(V_SONG_LYRICS)
			.Result(O_ORDER_IMPORT)
			.Result(O_ORDER_IMPORT_DETAILED)
			.Result(O_ORDER_REVERSE)
			.Result(O_SONG_UNIQLINES)
			.Result(O_TASKS)
		;
	
	AddRule(TASK_PATTERNMASK, "pattern masks of parts of song")
		.Require(O_ORDER_IMPORT)
		.Input(&Task::CreateInput_PatternMask)
			.Arg(V_PTR_SONG)
			.Arg(V_MODE, 0, GENDER_COUNT)
			.Arg(V_ARGS, 3, 3)
		.Process(&Task::Process_PatternMask)
			.Result(O_SONG_MASK)
			.Result(O_PART_MASK)
			.Result(O_DB_ATTRS)
		;
	
	AddRule(TASK_ANALYSIS, "analysis of parts of song")
		.Require(O_ORDER_IMPORT_DETAILED)
		.Input(&Task::CreateInput_Analysis)
			.Arg(V_PTR_SONG)
			.Arg(V_SONG_PARTS)
			.Arg(V_MODE, 0, GENDER_COUNT)
		.Process(&Task::Process_Analysis)
			.Arg(V_ARGS, 1, INT_MAX)
			.Result(O_SONG_ANALYSIS)
		;
	
	AddRule(TASK_STORYARC, "story arc of song")
		.Require(O_ORDER_IMPORT)
		.Input(&Task::CreateInput_StoryArc)
			.Arg(V_PTR_SONG)
			.Arg(V_SONG_PARTS)
			.Arg(V_MODE, 0, GENDER_COUNT)
			.Arg(V_LINE_TXT)
		.Process(&Task::Process_StoryArc)
			.Result(O_PART_DATA_STORYLINE)
			.Result(O_SONG_DATA_STORYLINE)
		;
	
	AddRule(TASK_IMPACT, "impacts")
		.Require(O_ORDER_IMPORT)
		.Input(&Task::CreateInput_Impact)
			.Arg(V_PTR_SONG)
			.Arg(V_PTR_LINE)
			.Arg(V_MODE, 0, GENDER_COUNT)
		.Process(&Task::Process_Impact)
			.Result(O_BREAK_IMPACT)
		;
	
	AddRule(TASK_MAKE_IMPACT_SCORING_TASKS, "make impact scoring tasks")
		.Spawnable()
		.Require(O_ORDER_IMPORT)
		.Require(O_BREAK_IMPACT)
		.Process(&Task::Process_MakeImpactScoringTasks)
			.Arg(V_PTR_SONG)
			.Arg(V_MODE, 0, GENDER_COUNT)
			.Result(O_TASKS)
		;
	
	AddRule(TASK_IMPACT_SCORING, "impact scoring")
		.Require(O_ORDER_IMPORT)
		.Require(O_BREAK_IMPACT)
		.Input(&Task::CreateInput_ImpactScoring)
			.Arg(V_PTR_SONG)
			.Arg(V_MODE, 0, GENDER_COUNT)
		.Process(&Task::Process_ImpactScoring)
			.Arg(V_ATTR_SCORING)
			.Result(O_BREAK_IMPACT_SCORES)
		;
	
	AddRule(TASK_MAKE_PATTERN_TASKS, "make pattern tasks")
		.Spawnable()
		.Require(O_ORDER_IMPORT)
		.Require(O_SONG_MASK)
		.Require(O_PART_MASK)
		.Process(&Task::Process_MakePatternTasks)
			.Arg(V_PTR_SONG)
			.Arg(V_MODE, 0, GENDER_COUNT)
			.Arg(V_PTR_SONG_UNIQUELINES)
			.Result(O_TASKS)
		;
	
	AddRule(TASK_PATTERN, "pattern")
		.Require(O_ORDER_IMPORT)
		.Require(O_SONG_MASK)
		.Require(O_PART_MASK)
		.Require(O_SONG_UNIQLINES)
		.Input(&Task::CreateInput_Pattern)
			.Arg(V_PTR_SONG)
			.Arg(V_MODE, 0, GENDER_COUNT)
		.Process(&Task::Process_Pattern)
			.Result(O_SONG_UNIQLINE_ATTRS)
			//.Result(O_SONG_SNAP)
			.Result(O_PART_SNAP)
			.Result(O_LINE_SNAP)
			.Result(O_BREAK_SNAP)
			.Result(O_DB_ATTRS)
		;
	
	AddRule(TASK_MAKE_ATTRSCORES_TASKS, "make attribute score tasks")
		.Spawnable()
		.Require(O_ORDER_IMPORT)
		.Require(O_DB_ATTRS)
		.Require(O_SONG_MASK)
		.Require(O_PART_MASK)
		.Require(O_SONG_UNIQLINE_ATTRS)
		.Process(&Task::Process_MakeAttrScores)
			.Arg(V_PTR_SONG)
			.Arg(V_MODE, 0, GENDER_COUNT)
			.Result(O_TASKS)
		;
	
	AddRule(TASK_ATTRSCORES, "attribute scores")
		.Require(O_ORDER_IMPORT)
		.Require(O_DB_ATTRS)
		.Require(O_SONG_MASK)
		.Require(O_PART_MASK)
		.Input(&Task::CreateInput_AttrScores)
			.Arg(V_PTR_SONG)
			.Arg(V_MODE, 0, GENDER_COUNT)
		.Process(&Task::Process_AttrScores)
			.Result(O_DB_ATTR_SCORES)
		;
	
	AddRule(TASK_SONGSCORE, "song score")
		.Require(O_ORDER_IMPORT)
		.Require(O_DB_ATTR_SCORES)
		.Process(&Task::Process_SongScores)
			.Arg(V_PTR_SONG)
			.Arg(V_MODE, 0, GENDER_COUNT)
			.Result(O_PART_MASK_SCORE)
			.Result(O_PART_SNAP_SCORE)
			.Result(O_LINE_SNAP_SCORE)
			.Result(O_BREAK_SNAP_SCORE)
		;
	
	AddRule(TASK_MAKE_REVERSE_IMPACT_TASK, "make reverse impact tasks")
		.Spawnable()
		.Require(O_ORDER_REVERSE)
		.Require(O_BREAK_IMPACT_SCORES)
		.Process(&Task::Process_MakeReverseImpactTask)
			.Arg(V_MODE, 0, 1)
		;
	
	AddRule(TASK_REVERSE_IMPACT, "reverse impact")
		.Require(O_ORDER_REVERSE)
		.Require(O_BREAK_IMPACT_SCORES)
		.Process(&Task::Process_ReverseImpact)
			.Arg(V_MODE, 0, 1)
			.Result(O_BREAK_REVERSED_IMPACT)
		;
	
	AddRule(TASK_MAKE_REVERSE_MASK_TASK, "make reverse mask tasks")
		.Spawnable()
		.Require(O_ORDER_REVERSE)
		.Require(O_PART_MASK_SCORE)
		.Process(&Task::Process_MakeReverseMaskTask)
			.Arg(V_MODE, 0, 1)
		;
	
	AddRule(TASK_REVERSE_COMMON_MASK, "reverse common mask")
		.Require(O_ORDER_REVERSE)
		.Require(O_PART_MASK_SCORE)
		.Process(&Task::Process_ReverseCommonMask)
			.Arg(V_MODE, 0, 1)
			.Result(O_SONG_REVERSED_MASK_COMMON)
		;
	
	AddRule(TASK_REVERSE_SEPARATE_MASK, "reverse separate mask")
		.Require(O_ORDER_REVERSE)
		.Require(O_PART_MASK_SCORE)
		.Require(O_SONG_REVERSED_MASK_COMMON)
		.Process(&Task::Process_ReverseSeparateMask)
			.Arg(V_MODE, 0, 1)
			.Result(O_SONG_REVERSED_MASK)
		;
	
	AddRule(TASK_MAKE_REVERSEPATTERN_TASK, "make reverse pattern tasks")
		.Spawnable()
		.Require(O_ORDER_REVERSE)
		.Require(O_PART_SNAP_SCORE)
		.Require(O_LINE_SNAP_SCORE)
		.Require(O_BREAK_SNAP_SCORE)
		.Require(O_SONG_REVERSED_MASK)
		.Process(&Task::Process_MakeReversePattern)
			.Arg(V_MODE, 0, 1)
		;
	
	AddRule(TASK_REVERSEPATTERN, "reverse pattern")
		.Require(O_ORDER_REVERSE)
		.Require(O_PART_SNAP_SCORE)
		.Require(O_LINE_SNAP_SCORE)
		.Require(O_BREAK_SNAP_SCORE)
		.Require(O_SONG_REVERSED_MASK)
		.Process(&Task::Process_ReversePattern)
			.Arg(V_MODE, 0, 1)
			.Result(O_PART_REVERSED_SNAP)
			.Result(O_LINE_REVERSED_SNAP)
			.Result(O_BREAK_REVERSED_SNAP)
		;
	
	AddRule(TASK_MAKE_LYRICS_TASK, "make reversed lyrics task")
		.Require(O_ORDER_REVERSE)
		.Require(O_SONG_DATA_STORYLINE)
		.Require(O_PART_DATA_STORYLINE)
		.Require(O_BREAK_REVERSED_IMPACT)
		.Require(O_PART_REVERSED_SNAP)
		.Require(O_LINE_REVERSED_SNAP)
		.Require(O_BREAK_REVERSED_SNAP)
		.Process(&Task::Process_MakeLyricsTask)
			.Arg(V_MODE, MALE_REVERSED, PTR_COUNT)
		;
	
	AddRule(TASK_LYRICS, "reversed lyrics")
		.Require(O_ORDER_REVERSE)
		.Require(O_SONG_DATA_STORYLINE)
		.Require(O_PART_DATA_STORYLINE)
		.Require(O_BREAK_REVERSED_IMPACT)
		.Require(O_PART_REVERSED_SNAP)
		.Require(O_LINE_REVERSED_SNAP)
		.Require(O_BREAK_REVERSED_SNAP)
		.Input(&Task::CreateInput_Lyrics)
		.Process(&Task::Process_Lyrics)
			.Result(O_SONG_REVERSED_LYRICS)
			.Arg(V_MODE, MALE_REVERSED, PTR_COUNT)
		;
	
	AddRule(TASK_LYRICS_TRANSLATE, "reversed lyrics translate")
		.Require(O_ORDER_REVERSE)
		.Require(O_SONG_REVERSED_LYRICS)
		.Input(&Task::CreateInput_LyricsTranslate)
		.Process(&Task::Process_LyricsTranslate)
			.Result(O_SONG_REVERSED_TRANSLATED_LYRICS)
			.Arg(V_MODE, MALE_REVERSED, PTR_COUNT)
		;
	
	
}

void TaskMgr::Process() {
	
	while (running) {
		this->total = tasks.GetCount();
		
		int ready = 0, got_ready = 0;
		for(int i = 0; i < tasks.GetCount() && running && !Thread::IsShutdownThreads(); i++) {
			Task& t = tasks[i];
			if (!t.ready) {
				ProcessSingle(i);
				if (t.ready) {
					actual++;
					ready++;
					got_ready++;
				}
			}
			else
				ready++;
		}
		this->actual = ready;
		
		if (!got_ready) {
			int tried_retry = 0;
			for (Task& t : tasks) {
				if (t.failed && !t.ready && t.tries < max_tries) {
					t.tries++;
					t.Retry();
					tried_retry++;
				}
			}
			if (!tried_retry) {
				SpawnTasks();
			}
		}
		
		Sleep(10);
	}
	stopped = true;
}

void TaskMgr::ProcessSingle(int task_i) {
	task_lock.Enter();
	
	Task& t = tasks[task_i];
	
	// Skip ready, failed and those with non-ready dependencies
	Index<Task*> seen;
	if (t.ready || t.failed || !IsDepsReady(t, seen))
		;
	else {
		status = "Processing #" + IntStr(task_i);
		t.mgr = this;
		t.Process();
		status = "";
	}
	
	task_lock.Leave();
}

void TaskMgr::Load() {
	LoadFromFile(*this, ConfigFile("taskmgr.bin"));
}

void TaskMgr::Store() {
	StoreToFile(*this, ConfigFile("taskmgr.bin"));
	for (Task& t : tasks)
		t.Store();
}

bool TaskMgr::IsDepsReady(Task& t, Index<Task*>& seen) const {
	bool all_ready = true;
	ASSERT(t.rule);
	TaskRule& r = *t.rule;
	for (TaskOutputType& o : r.reqs) {
		bool found = false;
		for (const Task& t0 : tasks) {
			const TaskRule& r0 = *t0.rule;
			if (t0.ready) {
				for (const TaskOutputType& o0 : r0.results) {
					if (o == o0) {
						found = true;
						break;
					}
				}
				if (found)
					break;
			}
		}
		if (!found)
			return false;
	}
	return true;
}



void TaskMgr::ImportSongAndMakeReversedSong(Song& s) {
	Database& db = Database::Single();
	TaskRule& r = GetRule(TASK_IMPORT_AND_REVERSE);
	for (Task& t : tasks) {
		if (t.rule == &r) {
			LOG("TaskMgr::ImportSongAndMakeReversedSong: error: task exists already");
			return;
		}
	}
	
	Task& t = tasks.Add();
	t.rule = &r;
	t.p.CopyPtrs(db.ctx.p[0]);
	
}

TaskRule& TaskMgr::GetRule(int code) {
	for (TaskRule& r : rules)
		if (r.code == code)
			return r;
	static TaskRule r;
	return r;
}

TaskRule& TaskMgr::AddRule(int code, String name) {
	TaskRule& r = rules.Add();
	r.code = code;
	r.name = name;
	return r;
}

bool TaskMgr::SpawnTasks() {
	int spawned = 0;
	Index<Song*> task_songs;
	for (Task& t : tasks)
		task_songs.FindAdd(t.p.song);
	for (TaskRule& r : rules) {
		if (r.spawnable) {
			ASSERT(r.reqs.GetCount());
			for (Song* s : task_songs.GetKeys()) {
				bool exists_already = false;
				for (Task& t : tasks) {
					if (t.rule == &r && t.p.song == s) {
						exists_already = true;
						break;
					}
				}
				if (exists_already)
					continue;
				
				bool found_all = true;
				for (TaskOutputType tt : r.reqs) {
					bool found = false;
					for (Task& t : tasks) {
						for (TaskOutputType t0 : t.rule->results) {
							if (t0 == tt) {
								found = true;
								break;
							}
						}
						if (found) break;
					}
					if (!found) {
						found_all = false;
						break;
					}
				}
				if (!found_all)
					continue;
				
				int mode_begin = -1, mode_end = 0;
				for (const TaskRule::ArgTuple& arg : r.args) {
					if (arg.a == V_MODE) {
						mode_begin = arg.b;
						mode_end = arg.c;
					}
				}
				
				for (int mode = mode_begin; mode < mode_end; mode++) {
					Task& t = tasks.Add();
					t.rule = &r;
					t.p.CopyPtrs(s->snap[0]);
					t.p.mode = mode;
					spawned++;
				}
			}
		}
	}
	return spawned > 0;
}













TaskRule& TaskRule::Input(void (Task::*fn)()) {
	this->input = fn;
	return *this;
}

TaskRule& TaskRule::Arg(TaskArgType arg, int i0, int i1) {
	ArgTuple& a = args.Add();
	a.a = arg;
	a.b = i0;
	a.c = i1;
	return *this;
}

TaskRule& TaskRule::Require(TaskOutputType arg) {
	reqs.Add(arg);
	return *this;
}

TaskRule& TaskRule::Process(void (Task::*fn)()) {
	process = fn;
	return *this;
}

TaskRule& TaskRule::Result(TaskOutputType arg) {
	results.Add(arg);
	return *this;
}

TaskRule& TaskRule::Spawnable(bool b) {
	spawnable = b;
	return *this;
}

