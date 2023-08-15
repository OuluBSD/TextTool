#include "SongTool.h"



void TaskMgrConfig::CreateDefaultTaskRules() {
	
	AddRule(TASK_IMPORT_AND_REVERSE, "import song and make reversed song")
		.Process(&Task::Process_MakeImportTasks)
			.Arg(V_PTR_PIPE)
			.Arg(V_SONG_LYRICS)
			.Result(O_TASKS)
			.Result(O_ORDER_IMPORT)
			.Result(O_ORDER_IMPORT_DETAILED)
			.CrossMode()
		;
	
	AddRule(TASK_CONTEXT_IMPORT_AND_REVERSE, "import context")
		.Require(O_ORDER_IMPORT)
		.Require(O_NEXT_CTX_JUMP)
		.Process(&Task::Process_MakeContextImportTasks)
			.Arg(V_PTR_PIPE)
			.Arg(V_SONG_LYRICS)
			.Result(O_ORDER_REVERSE)
			//.Result(O_SONG_UNIQLINES)
			.Result(O_TASKS)
			.CrossMode()
		;
	
	AddRule(TASK_PATTERNMASK, "pattern masks of parts of song")
		.Require(O_ORDER_IMPORT)
		.Input(&Task::CreateInput_PatternMask)
			.Arg(V_PTR_PIPE)
			.Arg(V_MODE, 0, MODE_COUNT)
			.Arg(V_ARGS, 2, 2)
		.Process(&Task::Process_PatternMask)
			.Result(O_SONG_MASK)
			.Result(O_PART_MASK)
			.Result(O_DB_ATTRS)
		.Hash(V_ATTRS)
		.Hash(V_TXT)
		.SeparateItems()
		;
	
	AddRule(TASK_PATTERNMASK_WEIGHTED, "weighted pattern masks")
		.Require(O_ORDER_IMPORT)
		.RequireMode(O_SONG_MASK, MODE_BEGIN, FEMALE)
		.RequireMode(O_PART_MASK, MODE_BEGIN, FEMALE)
		.Require(O_BREAK_LYRICS) //_WEIGHTED)
		.Input(&Task::CreateInput_PatternMaskWeighted)
			.Arg(V_PTR_PIPE)
			.Arg(V_MODE, 0, MODE_COUNT)
			.Arg(V_ARGS, 2, 2)
		.Process(&Task::Process_PatternMaskWeighted)
			.Result(O_SONG_MASK) //_WEIGHTED)
			.Result(O_PART_MASK) //_WEIGHTED)
			.Result(O_DB_ATTRS)
		.Hash(V_MALE_FEMALE_MASK)
		.SeparateItems()
		;
	
	AddRule(TASK_ANALYSIS, "analysis of parts of song")
		.Require(O_ORDER_IMPORT_DETAILED)
		.Input(&Task::CreateInput_Analysis)
			.Arg(V_PTR_PIPE)
			.Arg(V_SONG_PARTS)
			.Arg(V_MODE, 0, MODE_COUNT)
		.Process(&Task::Process_Analysis)
			.Arg(V_ARGS, 1, INT_MAX)
			.Result(O_SONG_ANALYSIS)
		.Hash(V_TXT)
		;
	
	AddRule(TASK_STORYARC, "story arc of song")
		.Require(O_ORDER_IMPORT)
		.Input(&Task::CreateInput_StoryArc)
			.Arg(V_PTR_PIPE)
			.Arg(V_SONG_PARTS)
			.Arg(V_MODE, 0, MODE_COUNT)
			.Arg(V_HUMAN_INPUT_LINE_TXT)
		.Process(&Task::Process_StoryArc)
			.Result(O_PART_DATA_STORYLINE)
			.Result(O_SONG_DATA_STORYLINE)
		.Hash(V_TXT)
		;
	
	AddRule(TASK_STORYARC_WEIGHTED, "story arc WEIGHTED mode")
		.RequireMode(O_PART_DATA_STORYLINE, MODE_BEGIN, FEMALE)
		.RequireMode(O_SONG_DATA_STORYLINE, MODE_BEGIN, FEMALE)
		.Input(&Task::CreateInput_StoryArcWeighted)
		.Process(&Task::Process_StoryArcWeighted)
			.Result(O_PART_DATA_STORYLINE) //_WEIGHTED)
			.Result(O_SONG_DATA_STORYLINE) //_WEIGHTED)
		;
	
	AddRule(TASK_IMPACT, "impacts")
		.Require(O_ORDER_IMPORT)
		.Input(&Task::CreateInput_Impact)
			.Arg(V_PTR_PIPE)
			.Arg(V_PTR_LINE)
			.Arg(V_MODE, 0, MODE_COUNT)
		.Process(&Task::Process_Impact)
			.Result(O_BREAK_IMPACT)
		;
	
	AddRule(TASK_IMPACT_WEIGHTED, "weighted impacts")
		.Require(O_ORDER_IMPORT)
		.RequireMode(O_BREAK_IMPACT, MODE_BEGIN, FEMALE)
		.Input(&Task::CreateInput_ImpactWeighted)
			.Arg(V_PTR_PIPE)
			.Arg(V_PTR_LINE)
			.Arg(V_MODE, 0, MODE_COUNT)
		.Process(&Task::Process_ImpactWeighted)
			.Result(O_BREAK_IMPACT) //_WEIGHTED)
		;
	
	AddRule(TASK_FORWARD_LYRICS_WEIGHTED, "lyrics from weighted impacts")
		.Require(O_ORDER_IMPORT)
		.Require(O_BREAK_IMPACT) //_WEIGHTED)
		.Input(&Task::CreateInput_ForwardLyricsWeighted)
			.Arg(V_PTR_PIPE)
			.Arg(V_PTR_LINE)
			.Arg(V_MODE, 0, MODE_COUNT)
		.Process(&Task::Process_ForwardLyricsWeighted)
			.Result(O_BREAK_LYRICS) //_WEIGHTED)
		;
	
	AddRule(TASK_MAKE_IMPACT_SCORING_TASKS, "make impact scoring tasks")
		.Spawnable()
		.Require(O_ORDER_IMPORT)
		.Require(O_BREAK_IMPACT)
		.RequireMode(O_BREAK_IMPACT, WEIGHTED, WEIGHTED)
		.RequireMode(O_BREAK_LYRICS, WEIGHTED, WEIGHTED)
		.Process(&Task::Process_MakeImpactScoringTasks)
			.Arg(V_PTR_PIPE)
			.Arg(V_MODE, 0, MODE_COUNT)
			.Result(O_TASKS)
		.DebugInput()
		;
	
	AddRule(TASK_IMPACT_SCORING, "impact scoring")
		.Require(O_ORDER_IMPORT)
		.Require(O_BREAK_IMPACT)
		.RequireMode(O_BREAK_IMPACT, WEIGHTED, WEIGHTED)
		.Input(&Task::CreateInput_ImpactScoring)
			.Arg(V_PTR_PIPE)
			.Arg(V_MODE, 0, MODE_COUNT)
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
			.Arg(V_PTR_PIPE)
			.Arg(V_MODE, 0, MODE_COUNT)
			//.Arg(V_PTR_PIPE_UNIQUELINES)
			.Result(O_TASKS)
		.DebugInput()
		;
	
	AddRule(TASK_PATTERN, "pattern")
		.Require(O_ORDER_IMPORT)
		.Require(O_SONG_MASK)
		.Require(O_PART_MASK)
		//.Require(O_SONG_UNIQLINES)
		.Input(&Task::CreateInput_Pattern)
			.Arg(V_PTR_PIPE)
			.Arg(V_MODE, 0, HUMAN_INPUT_MODE_COUNT)
		.Process(&Task::Process_Pattern)
			.Result(O_SONG_UNIQLINE_ATTRS)
			//.Result(O_SONG_SNAP)
			.Result(O_PART_SNAP)
			.Result(O_LINE_SNAP)
			.Result(O_BREAK_SNAP)
			.Result(O_DB_ATTRS)
		;
	
	AddRule(TASK_PATTERN_WEIGHTED, "pattern weighted")
		.Require(O_ORDER_IMPORT)
		.RequireMode(O_PART_SNAP, MALE, FEMALE)
		.RequireMode(O_LINE_SNAP, MALE, FEMALE)
		.RequireMode(O_BREAK_SNAP, MALE, FEMALE)
		.Input(&Task::CreateInput_PatternWeighted)
			.Arg(V_PTR_PIPE)
			.Arg(V_MODE, WEIGHTED, MODE_COUNT)
		.Process(&Task::Process_PatternWeighted)
			.Result(O_SONG_UNIQLINE_ATTRS)
			//.Result(O_SONG_SNAP)
			.Result(O_PART_SNAP)
			.Result(O_LINE_SNAP)
			.Result(O_BREAK_SNAP)
		;
	
	AddRule(TASK_MAKE_ATTRSCORES_TASKS, "make attribute score tasks")
		.MultiSpawnable()
		.Require(O_ORDER_IMPORT)
		.Require(O_DB_ATTRS)
		.Require(O_SONG_MASK)
		.Require(O_PART_MASK)
		.Require(O_SONG_UNIQLINE_ATTRS)
		.Process(&Task::Process_MakeAttrScores)
			.Arg(V_PTR_PIPE)
			.Arg(V_MODE, 0, HUMAN_INPUT_MODE_COUNT)
			.Result(O_TASKS)
		.DebugInput()
		;
	
	AddRule(TASK_ATTRSCORES, "attribute scores")
		.Require(O_ORDER_IMPORT)
		.Require(O_DB_ATTRS)
		.Require(O_SONG_MASK)
		.Require(O_PART_MASK)
		.Input(&Task::CreateInput_AttrScores)
			.Arg(V_PTR_PIPE)
			.Arg(V_MODE, 0, HUMAN_INPUT_MODE_COUNT)
		.Process(&Task::Process_AttrScores)
		;
	
	AddRule(TASK_ATTRSCORES_READY, "attribute scores ready")
		.Result(O_DB_ATTR_SCORES)
		.DebugInput()
		;
	
	AddRule(TASK_SONGSCORE, "song score")
		.Require(O_ORDER_IMPORT)
		.Require(O_DB_ATTR_SCORES)
		.Require(O_PART_SNAP)
		.Require(O_LINE_SNAP)
		.Require(O_BREAK_SNAP)
		.Require(O_PART_SNAP)
		.Require(O_LINE_SNAP)
		.Require(O_BREAK_SNAP)
		.Process(&Task::Process_SongScores)
			.Arg(V_PTR_PIPE)
			.Arg(V_MODE, 0, HUMAN_INPUT_MODE_COUNT)
			.Result(O_PART_MASK_SCORE)
			.Result(O_PART_SNAP_SCORE)
			.Result(O_LINE_SNAP_SCORE)
			.Result(O_BREAK_SNAP_SCORE)
		.DebugInput()
		;
	
	AddRule(TASK_MAKE_REVERSE_IMPACT_TASK, "make reverse impact tasks")
		.Spawnable()
		.Require(O_NEVER)
		.Require(O_ORDER_REVERSE)
		.Require(O_BREAK_IMPACT_SCORES)
		.Process(&Task::Process_MakeReverseImpactTask)
			.Arg(V_MODE, 0, 1)
		.DebugInput()
			.Result(O_TASKS)
		;
	
	AddRule(TASK_REVERSE_IMPACT, "reverse impact")
		.Require(O_ORDER_REVERSE)
		.Require(O_BREAK_IMPACT_SCORES)
		.Process(&Task::Process_ReverseImpact)
			.Arg(V_MODE, 0, 1)
			.Result(O_BREAK_REVERSED_IMPACT)
		.DebugInput()
		;
	
	AddRule(TASK_MAKE_REVERSE_MASK_TASK, "make reverse mask tasks")
		.Spawnable()
		.Require(O_NEVER)
		.Require(O_ORDER_REVERSE)
		.Require(O_PART_MASK_SCORE)
		.Process(&Task::Process_MakeReverseMaskTask)
			.Arg(V_MODE, 0, 1)
			.Result(O_TASKS)
		.DebugInput()
		;
	
	AddRule(TASK_REVERSE_COMMON_MASK, "reverse common mask")
		.Require(O_ORDER_REVERSE)
		.Require(O_PART_MASK_SCORE)
		.Process(&Task::Process_ReverseCommonMask)
			.Arg(V_MODE, 0, 1)
			.Result(O_SONG_REVERSED_MASK_COMMON)
		.DebugInput()
		;
	
	AddRule(TASK_REVERSE_SEPARATE_MASK, "reverse separate mask")
		.Require(O_ORDER_REVERSE)
		.Require(O_PART_MASK_SCORE)
		.Require(O_SONG_REVERSED_MASK_COMMON)
		.Process(&Task::Process_ReverseSeparateMask)
			.Arg(V_MODE, 0, 1)
			.Result(O_SONG_REVERSED_MASK)
		.DebugInput()
		;
	
	AddRule(TASK_MAKE_REVERSEPATTERN_TASK, "make reverse pattern tasks")
		.Spawnable()
		.Require(O_NEVER)
		.Require(O_ORDER_REVERSE)
		.Require(O_PART_SNAP_SCORE)
		.Require(O_LINE_SNAP_SCORE)
		.Require(O_BREAK_SNAP_SCORE)
		.Require(O_SONG_REVERSED_MASK)
		.Process(&Task::Process_MakeReversePattern)
			.Arg(V_MODE, 0, 1)
			.Result(O_TASKS)
		.DebugInput()
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
			.Result(O_NEXT_CTX_JUMP)
		.DebugInput()
		;
	
	
	AddRule(TASK_MAKE_LYRICS_TASK, "make reversed lyrics task")
		.Spawnable()
		.Require(O_ORDER_REVERSE)
		.Require(O_SONG_DATA_STORYLINE)
		.Require(O_PART_DATA_STORYLINE)
		.Require(O_BREAK_REVERSED_IMPACT)
		.Require(O_PART_REVERSED_SNAP)
		.Require(O_LINE_REVERSED_SNAP)
		.Require(O_BREAK_REVERSED_SNAP)
		.Process(&Task::Process_MakeLyricsTask)
			.Arg(V_DIR, BACKWARD, DIR_COUNT)
			.Result(O_TASKS)
		.DebugInput()
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
			.Arg(V_DIR, BACKWARD, DIR_COUNT)
		.DebugInput()
		;
	
	AddRule(TASK_LYRICS_TRANSLATE, "reversed lyrics translate")
		.Require(O_ORDER_REVERSE)
		.Require(O_SONG_REVERSED_LYRICS)
		.Input(&Task::CreateInput_LyricsTranslate)
		.Process(&Task::Process_LyricsTranslate)
			.Result(O_SONG_REVERSED_TRANSLATED_LYRICS)
			.Arg(V_DIR, BACKWARD, DIR_COUNT)
		.DebugInput()
		;
	
	
}

void TaskMgrConfig::Load() {
	LoadFromFile(*this, ConfigFile("taskmgr.bin"));
}

void TaskMgrConfig::Store() {
	StoreToFile(*this, ConfigFile("taskmgr.bin"));
}

void TaskMgrConfig::Process() {
	Database& db = Database::Single();
	
	while (running) {
		for (Artist& art : db.artists) {
			for (Release& rel : art.releases) {
				for (Song& song : rel.songs) {
					if (song.pipe) {
						Pipe& pipe = *song.pipe;
						pipe.TaskMgr::Process();
					}
				}
			}
		}
		Sleep(10);
	}
	
	stopped = true;
}














void TaskMgr::LoadTaskOrder() {
	String dir = ConfigFile("taskmgr");
	RealizeDirectory(dir);
	String fname = IntStr64(GetSongHash()) + ".bin";
	String path = AppendFileName(dir, fname);
	if (FileExists(path))
		LoadFromFile(*this, path);
}

void TaskMgr::StoreTaskOrder() {
	String dir = ConfigFile("taskmgr");
	RealizeDirectory(dir);
	String fname = IntStr64(GetSongHash()) + ".bin";
	String path = AppendFileName(dir, fname);
	StoreToFile(*this, path);
}

void TaskMgr::Process() {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	
	if (!iters) {
		LoadTaskOrder();
	}
	iters++;
	
	int ready = 0, got_ready = 0;
	
	this->total = tasks.GetCount();
	
	bool shorten_task_order = false;
	
	if (!task_order_cache_missed) {
		while (task_i < task_order.GetCount()) {
			hash_t task_hash0 = task_order[task_i];
			bool task_ready = false;
			for(int i = 0; i < tasks.GetCount() && mgr.running && !Thread::IsShutdownThreads(); i++) {
				Task& t = tasks[i];
				hash_t task_hash1 = t.GetOrderHash();
				if (task_hash0 == task_hash1) {
					if (!t.ready) {
						ProcessSingle(i);
						if (t.ready) {
							task_order_dbg << t.GetInfoInline();
							actual++;
							ready++;
							got_ready++;
							task_ready = true;
							// StoreTaskOrder(); // No need to store, because it follows old list
						}
					}
					else
						task_ready = true; // TODO almost an error, is it?
					break;
				}
			}
			if (task_ready) {
				task_i++;
			}
			else {
				// Task failed, but it's not necessarily fail of ordered task processing.
				
				// Possible reasons:
				// - task is being spawned in the end of this function
				// - the AI task input was changed and it caused some new problems.
				shorten_task_order = true;
				break;
			}
		}
	}
	
	for(int i = 0; i < tasks.GetCount() && mgr.running && !Thread::IsShutdownThreads(); i++) {
		Task& t = tasks[i];
		if (!t.ready) {
			ProcessSingle(i);
			if (t.ready) {
				actual++;
				ready++;
				got_ready++;
				task_i++;
				task_order_dbg << t.GetInfoInline();
				
				hash_t hash = t.GetOrderHash();
				if (shorten_task_order) {
					// If somehow we ended up being in the right hash (then keep going)
					// TODO: strengthen this! it's very weak mechanism to get back
					if (task_i < task_order.GetCount() && task_order[task_i] == hash) {
						task_i++;
						break; // loop indirectly back to ordered hash processing
					}
					// CACHE MISS!
					else {
						//DUMPC(task_order_dbg);
						task_order.SetCount(task_i); // the failure is certain here.
						task_order << hash;
						task_order_cache_missed = true;
					}
					shorten_task_order = false;
				}
				else {
					task_order << hash;
					//DUMPC(task_order_dbg);
				}
				
				StoreTaskOrder();
			}
		}
		else
			ready++;
	}
	this->actual = ready;
	
	if (!got_ready) {
		int tried_retry = 0;
		for (Task& t : tasks) {
			if (t.fatal_error)
				continue;
			if (t.failed && !t.ready && t.tries < mgr.max_tries) {
				t.tries++;
				t.Retry(false);
				tried_retry++;
			}
		}
		if (!tried_retry) {
			SpawnTasks();
		}
	}
}

void TaskMgr::ProcessSingle(int task_i) {
	task_lock.Enter();
	
	Task& t = tasks[task_i];
	
	Index<Task*> seen;
	t.is_waiting_deps = !IsDepsReady(t, seen);
	
	// Skip ready, failed and those with non-ready dependencies
	if (t.ready || t.failed || t.is_waiting_deps)
		;
	else {
		status = "Processing #" + IntStr(task_i);
		t.Process();
		status = "";
	}
	
	task_lock.Leave();
}

bool TaskMgr::IsDepsReady(Task& t, Index<Task*>& seen) const {
	bool all_ready = true;
	ASSERT(t.rule);
	const TaskRule& r = *t.rule;
	for (const TaskOutputType& o : r.reqs) {
		bool found = false;
		int user_mode_range_i = r.req_mode_ranges.Find(o);
		bool user_mode_range = user_mode_range_i >= 0;
		
		if (user_mode_range) {
			const auto& range = r.req_mode_ranges[user_mode_range_i];
			found = true;
			for (int m = range.a; m <= range.b; m++) {
				bool mode_found = false;
				for (const Task& t0 : tasks) {
					if (t0.p.a.mode != (SnapMode)m)
						continue;
					const TaskRule& r0 = *t0.rule;
					if (t0.ready) {
						for (const TaskOutputType& o0 : r0.results) {
							if (o == o0) {
								mode_found = true;
								break;
							}
						}
						if (mode_found)
							break;
					}
				}
				if (!mode_found) {
					found = false;
					break;
				}
			}
		}
		else {
			for (const Task& t0 : tasks) {
				if (!t0.rule->allow_cross_mode && t.p.a.mode != t0.p.a.mode)
					continue;
				const TaskRule& r0 = *t0.rule;
				if (t0.ready) {
					for (const TaskOutputType& o0 : r0.results) {
						if (o == o0) {
							// Special case (must be previous context)
							if (o == O_NEXT_CTX_JUMP) {
								if (t0.p.a.ctx != t.p.a.ctx-1)
									continue;
							}
							found = true;
							break;
						}
					}
					if (found)
						break;
				}
			}
			// Special case: first context doesn't require this
			if (o == O_NEXT_CTX_JUMP) {
				if (t.p.a.ctx == CTX_BEGIN)
					found = true;
			}
		}
		
		if (!found)
			return false;
	}
	return true;
}



void TaskMgr::ImportSongAndMakeReversedSong(Pipe& p) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	Database& db = Database::Single();
	const TaskRule& r = mgr.GetRule(TASK_IMPORT_AND_REVERSE);
	
	if (1) {
		Vector<int> rm_list;
		for(int i = 0; i < tasks.GetCount(); i++) {
			Task& t = tasks[i];
			if (t.p.pipe == &p)
				rm_list << i;
		}
		tasks.Remove(rm_list);
	}
	else {
		for (Task& t : tasks) {
			if (t.rule == &r) {
				LOG("TaskMgr::ImportSongAndMakeReversedSong: error: task exists already");
				return;
			}
		}
	}
	
	Task& t = tasks.Add();
	t.rule = &r;
	//t.p.CopyPtrs(p.p);
	t.p.a = ZeroArg();
	t.p.pipe = &p;
	
	
}

TaskRule& TaskMgrConfig::GetRule(int code) {
	for (TaskRule& r : rules)
		if (r.code == code)
			return r;
	static TaskRule r;
	return r;
}

const TaskRule& TaskMgrConfig::GetRule(int code) const {
	for (const TaskRule& r : rules)
		if (r.code == code)
			return r;
	static TaskRule r;
	return r;
}

TaskRule& TaskMgrConfig::AddRule(int code, String name) {
	for (auto& r : rules) {
		ASSERT(r.code != code);
	}
	TaskRule& r = rules.Add();
	r.code = code;
	r.name = name;
	return r;
}

GroupContext TaskMgr::GetGroupContextLimit() const {
	int tt = 0;
	for (const Task& t : tasks) {
		if (t.rule->code == O_NEXT_CTX_JUMP && t.ready)
			tt = max(tt, (int)t.p.a.ctx + 1);
	}
	return (GroupContext)(tt+1);
}

bool TaskMgr::SpawnTasks() {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	
	int spawned = 0;
	Index<Pipe*> task_songs;
	for (Task& t : tasks)
		task_songs.FindAdd(t.p.pipe);
	GroupContext ctx_limit = GetGroupContextLimit();
	for (const TaskRule& r : mgr.rules) {
		for (GroupContext ctx = CTX_BEGIN; ctx != ctx_limit; ((int&)ctx)++) {
			bool ctx_spawned = false;
			if (r.spawnable) {
				bool verbose = false; //r.code == TASK_MAKE_IMPACT_SCORING_TASKS && task_songs.GetCount();
				if (verbose) {
					DUMP((int)ctx);
				}
				ASSERT(r.reqs.GetCount());
				for (Pipe* s : task_songs.GetKeys()) {
					Task* exists_already = 0;
					for (Task& t : tasks) {
						if (t.rule == &r && t.p.pipe == s && t.p.a.ctx == ctx) {
							exists_already = &t;
							break;
						}
					}
					if (exists_already) {
						// Some rules can be allowed to spawn multiple times
						// if task has made successful tasks and have not been used to spawn another already
						if (r.multi_spawnable && exists_already->allow_multi_spawn) {
							if (!exists_already->ready ||
								!exists_already->HasCreatedTasks(*this, ctx) ||
								!exists_already->IsCreatedTasksReady(*this, ctx))
								continue;
							
							// Use this flag only once
							exists_already->allow_multi_spawn = false;
						}
						else
							continue;
					}
					
					bool found_all = true;
					int dbg_i = -1;
					for (TaskOutputType tt : r.reqs) {
						dbg_i++;
						bool skips_ctx = IsTaskSkippingContext(tt);
						bool found = false;
						for (Task& t : tasks) {
							if (t.p.a.ctx != ctx && !skips_ctx)
								continue;
							for (TaskOutputType t0 : t.rule->results) {
								if (t0 == tt) {
									found = true;
									break;
								}
							}
							if (found) break;
						}
						// Special case
						if (tt == O_NEXT_CTX_JUMP) {
							if (ctx == CTX_BEGIN)
								found = true;
						}
						if (!found) {
							if (verbose) {
								DUMP(dbg_i);
								DUMP((int)tt);
							}
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
						t.p.CopyPtrs(s->Get0());
						t.p.a.mode = (SnapMode)mode;
						t.p.a.ctx = ctx;
						t.id = ++spawn_id;
						spawned++;
						ctx_spawned = true;
					}
				}
			}
			
			// Only for the last context that allowed spawning
			if (ctx_spawned)
				break;
		}
	}
	return spawned > 0;
}

hash_t TaskMgr::GetSongHash() const {
	if (hash)
		return hash;
	
	const Pipe& pipe = static_cast<const Pipe&>(*this);
	ASSERT(pipe.song);
	Song& song = *pipe.song;
	
	CombineHash ch;
	ch << song.EditorPtrs::artist->name;
	ch << song.EditorPtrs::release->title;
	ch << song.title;
	hash = ch;
	ASSERT(hash != 0);
	return hash;
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

TaskRule& TaskRule::RequireMode(TaskOutputType arg, SnapMode begin, SnapMode end) {
	reqs.Add(arg);
	ASSERT(req_mode_ranges.Find(arg) < 0);
	auto& t = req_mode_ranges.Add(arg);
	t.a = begin;
	t.b = end;
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

TaskRule& TaskRule::MultiSpawnable(bool b) {
	spawnable = b;
	multi_spawnable = b;
	return *this;
}

TaskRule& TaskRule::CrossMode(bool b) {
	allow_cross_mode = b;
	return *this;
}

TaskRule& TaskRule::Hash(TaskArgType t) {
	hashes.Add(t);
	return *this;
}

TaskRule& TaskRule::SeparateItems(bool b) {
	separate_items = b;
	return *this;
}

TaskRule& TaskRule::DebugInput(bool b) {
	debug_input = b;
	return *this;
}

