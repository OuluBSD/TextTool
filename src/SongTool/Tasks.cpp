#include "SongTool.h"


void Task::Store(bool force) {
	if (output.IsEmpty()) return;
	if (!changed) return;
	Database& db = Database::Single();
	String dir = ConfigFile("share" DIR_SEPS "ai_results" DIR_SEPS);
	RealizeDirectory(dir);
	String filename = GetInputHash() + ".txt";
	String file = dir + filename;
	//ASSERT(force || !FileExists(file));
	SaveFileBOMUtf8(file, output);
	changed = false;
}

void Task::Load() {
	if (skip_load)
		return;
	Database& db = Database::Single();
	String dir = ConfigFile("share" DIR_SEPS "ai_results" DIR_SEPS);
	RealizeDirectory(dir);
	String filename = GetInputHash() + ".txt";
	String file = dir + filename;
	if (FileExists(file))
		output = LoadFileBOM(file);
}

void Task::SetError(String s) {
	failed = true;
	error = s;
}

String Task::GetInputHash() const {
	hash_t h = this->hash ? this->hash : input.GetHashValue();
	return HexString((void*)&h, sizeof(h));
}

String Task::GetOutputHash() const {
	hash_t h = output.GetHashValue();
	return HexString((void*)&h, sizeof(h));
}

String Task::GetDescription() const {
	String s;
	s << GetTypeString();
	return s;
}

bool Task::HasCreatedTasks() const {
	TaskMgr& m = TaskMgr::Single();
	for (const Task& t : m.tasks) {
		if (t.created_by == this)
			return true;
	}
	return false;
}

bool Task::IsCreatedTasksReady() const {
	TaskMgr& m = TaskMgr::Single();
	for (const Task& t : m.tasks) {
		if (t.created_by != this)
			continue;
		if (!t.ready)
			return false;
	}
	return true;
}

String Task::GetTaskDependencyString(bool have_ready_rules, bool rule_names) const {
	TaskMgr& m = TaskMgr::Single();
	String s;
	Index<TaskRule*> rules;
	
	int i = 0;
	for (const Task& t : m.tasks) {
		if (t.ready != have_ready_rules) {
			i++;
			continue;
		}
		bool found = false;
		for (TaskOutputType ot : this->rule->reqs) {
			for (TaskOutputType ot0 : t.rule->results) {
				if (ot == ot0) {
					found = true;
					break;
				}
			}
		}
		if (created_by == &t)
			found = true;
		if (found) {
			if (rule_names) {
				rules.FindAdd(t.rule);
			}
			else {
				if (!s.IsEmpty()) s << ", ";
				s << "#" << i;
			}
		}
		i++;
	}
	
	if (rule_names) {
		for (int i = rules.GetCount()-1; i >= 0; i--) {
			TaskRule* r = rules[i];
			if (!s.IsEmpty()) s << ", ";
			s << r->name;
		}
	}
	return s;
}

String Task::GetTaskDepsWaitingString() const {
	return GetTaskDependencyString(false, false);
}

String Task::GetTypeString() const {
	return rule->name;
}
/*
void Task::CreateInput() {
	switch (type) {
		case TASK_PATTERNMASK:				CreateInput_PatternMask(); break;
		case TASK_ANALYSIS:					CreateInput_Analysis(); break;
		case TASK_STORYARC:					CreateInput_StoryArc(); break;
		case TASK_IMPACT:					CreateInput_Impact(); break;
		
		case TASK_MAKE_PATTERN_TASKS:		break;
		case TASK_PATTERN:					CreateInput_Pattern(); break;
		case TASK_IMPACT_SCORING:			CreateInput_ImpactScoring(); break;
		case TASK_MAKE_IMPACT_SCORING_TASKS:break;
		
		case TASK_MAKE_ATTRSCORES_TASKS:	break;
		case TASK_ATTRSCORES:				CreateInput_AttrScores(); break;
		case TASK_SONGSCORE:				break;
		
		case TASK_MAKE_REVERSE_IMPACT_TASK:	break;
		case TASK_REVERSE_IMPACT:			break;
		
		case TASK_MAKE_REVERSE_MASK_TASK:	break;
		case TASK_REVERSE_COMMON_MASK:		break;
		case TASK_REVERSE_SEPARATE_MASK:	break;
		
		case TASK_MAKE_REVERSEPATTERN_TASK:	break;
		case TASK_REVERSEPATTERN:			break;
		
		case TASK_MAKE_LYRICS_TASK:			break;
		case TASK_LYRICS:					CreateInput_Lyrics(); break;
		case TASK_LYRICS_TRANSLATE:			CreateInput_LyricsTranslate(); break;
		
		default: break;
	}
	
	Load();
}*/

bool Task::ProcessInput() {
	bool ok = true;
	
	// Return if this task won't have input function
	if (!rule->input)
		return ok;
	
	// Create input with given function
	if (input.IsEmpty() && rule->input) {
		(this->*rule->input)();
		if (fast_exit)
			return true;
		Load();
	}
	
	// Remove Win32 uselessness (\r in newline)
	input.Replace("\r","");
	output = TrimBoth(output);
	
	// Request output from completion-mode AI
	if (!input.IsEmpty() && output.IsEmpty()) {
		ok = RunOpenAI();
	}
	
	// Remove Win32 uselessness (\r in newline)
	output.Replace("\r","");
	
	if (output.IsEmpty())
		return false;
	
	return ok;
}

void Task::Process() {
	LOG("Task::Process: begin of " << rule->name);
	processing = true;
	
	bool ok = true;
	
	ok = CheckArguments();
	
	ok = ok && ProcessInput();
	
	if (fast_exit) {
		ready = true;
	}
	else if (ok) {
		if (rule->process)
			(this->*rule->process)();
		
		if (wait_task) {
			wait_task = false;
		}
		else {
			if (!failed) {
				if (WriteResults())
					ready = true;
				else
					SetError("internal error: could not write results");
			}
			changed = true;
		}
	}
	
	fast_exit = false;
	skip_load = false;
	processing = false;
	
	LOG("Task::Process: end of " << rule->name);
}

bool Task::CheckArguments() {
	TaskRule& r = *rule;
	
	for (const TaskRule::ArgTuple& arg : r.args) {
		TaskArgType type = arg.a;
		int i0 = arg.b;
		int i1 = arg.c;
		
		switch (type) {
		case V_ATTR_SCORING:
			//TODO
			break;
		case V_PTR_SONG:
			if (!p.song) {
				SetError("song pointer is missing");
				return false;
			}
			break;
		case V_PTR_LINE:
			if (!p.line) {
				SetError("line pointer is missing");
				return false;
			}
			break;
		case V_PTR_SONG_UNIQUELINES:
			if (!p.song->headers[0].unique_lines.GetCount() ||
				!p.song->headers[1].unique_lines.GetCount()) {
				SetError("song's unique lines are missing");
				return false;
			}
			break;
		case V_MODE:
			if (p.mode < 0) {
				SetError("mode is not set");
				return false;
			}
			if (p.mode < i0 || p.mode >= i1) {
				SetError("mode is not in range");
				return false;
			}
			break;
		case V_ARGS:
			if (args.GetCount() < i0 || args.GetCount() > i1) {
				SetError("argument count is not in range");
				return false;
			}
			break;
		case V_SONG_LYRICS:
			for(int i = 0; i < GENDER_COUNT; i++) {
				if (p.song->headers[i].content.IsEmpty()) {
					SetError("lyrics are missing for gender " + GetModeString(i));
					return false;
				}
			}
			break;
		case V_SONG_PARTS:
			if (p.song->parts.IsEmpty()) {
				SetError("no parts in song");
				return false;
			}
			break;
		case V_LINE_TXT:
			for(int i = 0; i < GENDER_COUNT; i++) {
				for (Part& part : p.song->parts) {
					for (Line& line : part.lines) {
						if (line.snap[i].txt.IsEmpty()) {
							SetError("text is missing for gender " + GetModeString(i));
							return false;
						}
					}
				}
			}
			break;
		}
	}
	
	return true;
}

bool Task::WriteResults() {
	Database& db = Database::Single();
	TaskMgr& m = TaskMgr::Single();
	
	for (TaskOutputType t :  rule->results) {
		switch (t) {
		// flags for dependencies only
		case O_ORDER_IMPORT: break;
		case O_ORDER_IMPORT_DETAILED: break;
		case O_ORDER_REVERSE: break;
		
		case O_DB_ATTRS: ASSERT(db.attrs.groups.GetCount());  break;
		case O_DB_ATTR_SCORES: ASSERT(db.attrscores.groups.GetCount()); break;
		
		case O_TASKS:
			if (result_tasks.IsEmpty()) {
				SetError("no tasks to add to queue");
				return false;
			}
			m.lock.EnterWrite();
			m.total += result_tasks.GetCount();
			while (result_tasks.GetCount())
				m.tasks.Add(result_tasks.Detach(0));
			m.lock.LeaveWrite();
			break;
		
		case O_SONG_MASK: break;
		case O_SONG_ANALYSIS: break;
		case O_SONG_DATA_STORYLINE: break;
		case O_SONG_UNIQLINES: ASSERT(p.song->headers[0].unique_lines.GetCount()); ASSERT(p.song->headers[1].unique_lines.GetCount()); break;
		case O_SONG_UNIQLINE_ATTRS: break;
		case O_SONG_SNAP: break;
		case O_SONG_REVERSED_MASK_COMMON: break;
		case O_SONG_REVERSED_MASK: break;
		case O_SONG_REVERSED_LYRICS: break;
		case O_SONG_REVERSED_TRANSLATED_LYRICS: break;
		
		case O_PART_MASK: break;
		case O_PART_MASK_SCORE: break;
		case O_PART_DATA_STORYLINE: break;
		case O_PART_SNAP: break;
		case O_PART_SNAP_SCORE: break;
		case O_PART_REVERSED_SNAP: break;
		
		case O_LINE_SNAP: break;
		case O_LINE_SNAP_SCORE: break;
		case O_LINE_REVERSED_SNAP: break;
		
		case O_BREAK_SNAP: break;
		case O_BREAK_SNAP_SCORE: break;
		case O_BREAK_IMPACT: break;
		case O_BREAK_IMPACT_SCORES: break;
		case O_BREAK_REVERSED_IMPACT: break;
		case O_BREAK_REVERSED_SNAP: break;
		}
	}
	
	return true;
}

bool Task::RunOpenAI() {
	output.Clear();
	
	if (!response_length) {
		LOG("warning: no response length set");
		response_length = 1024;
	}
	String prompt = input;
	//LOG(prompt);
	
	prompt.Replace("\n", "\\n");
	prompt.Replace("\t", "\\t");
	prompt.Replace("\"", "\\\"");
	
	prompt = FixInvalidChars(prompt); // NOTE: warning: might break something
	//prompt.Replace("\'", "\\\'");
	
	String txt = R"({
    "model": "text-davinci-003",
    "prompt": ")" + prompt + R"(",
    "max_tokens": )" + IntStr(response_length) + R"(,
    "temperature": 1
})";
    //LOG(txt);
	
	try {
		nlohmann::json json = nlohmann::json::parse(txt.Begin(), txt.End());
		auto completion = openai::completion().create(json);
	    //LOG("Response is:\n" << completion.dump(2));
	    
	    OpenAiResponse response;
	    LoadFromJson(response, String(completion.dump(2)));
	    //LOG(response.ToString());
	    
	    if (response.choices.GetCount())
			output = response.choices[0].text;
	    else
	        output.Clear();
	}
	catch (std::runtime_error e) {
		LOG(prompt);
		LOG(txt);
		fatal_error = true;
		SetError(e.what());
		return false;
	}
	catch (std::string e) {
		LOG(prompt);
		LOG(txt);
		fatal_error = true;
		SetError(e.c_str());
		return false;
	}
	catch (NLOHMANN_JSON_NAMESPACE::detail::parse_error e) {
		LOG(prompt);
		LOG(txt);
		LOG(e.what());
		fatal_error = true;
		SetError(e.what());
		return false;
	}
	catch (std::exception e) {
		LOG(prompt);
		LOG(txt);
		SetError(e.what());
		fatal_error = true;
		return false;
	}
	/*catch (...) {
		SetError("unknown error");
		return false;
	}*/
	
	// Fix unicode formatting
	output = ToUnicode(output, CHARSET_UTF8).ToString();
	
	changed = true;
	Store();
	return output.GetCount() > 0;
}

void Task::Retry() {
	input.Clear();
	output.Clear();
	skip_load = true;
	failed = false;
	fatal_error = false;
	ready = false;
	error.Clear();
	changed = true;
	tries = 0;
}

Task& Task::ResultTask(int r) {
	Task& t = result_tasks.Add();
	t.rule = &TaskMgr::Single().GetRule(r);
	t.created_by = this;
	return t;
}