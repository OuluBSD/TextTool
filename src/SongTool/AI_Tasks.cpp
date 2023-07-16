#include "SongTool.h"


bool AI_Task::IsDepsReady(Index<AI_Task*>& seen) const {
	bool all_ready = true;
	for (AI_Task* dep : depends_on) {
		if (seen.Find(dep) >= 0)
			continue;
		seen.Add(dep);
		if (!dep->ready || !dep->IsDepsReady(seen)) {
			all_ready = false;
			break;
		}
	}
	return all_ready;
}

void AI_Task::Store() {
	if (output.IsEmpty()) return;
	if (!changed) return;
	Database& db = Database::Single();
	String dir = ConfigFile("share" DIR_SEPS "ai_results" DIR_SEPS);
	RealizeDirectory(dir);
	String filename = GetInputHash() + ".txt";
	String file = dir + filename;
	SaveFileBOMUtf8(file, output);
	changed = false;
}

void AI_Task::Load() {
	if (skip_load)
		return;
	Database& db = Database::Single();
	String dir = ConfigFile("share" DIR_SEPS "ai_results" DIR_SEPS);
	RealizeDirectory(dir);
	String filename = GetInputHash() + ".txt";
	String file = dir + filename;
	output = LoadFileBOM(file);
}

void AI_Task::SetError(String s) {
	failed = true;
	error = s;
}

String AI_Task::GetInputHash() const {
	hash_t h = this->hash ? this->hash : input.GetHashValue();
	return HexString((void*)&h, sizeof(h));
}

String AI_Task::GetOutputHash() const {
	hash_t h = output.GetHashValue();
	return HexString((void*)&h, sizeof(h));
}

String AI_Task::GetDescription() const {
	String s;
	s << GetTypeString();
	return s;
}

String AI_Task::GetTypeString() const {
	switch (type) {
		case TASK_PATTERNMASK: return t_("Pattern mask");
		case TASK_MAKE_PATTERN_TASKS: return t_("Make pattern tasks");
		case TASK_PATTERN: return t_("Pattern");
		case TASK_ANALYSIS: return t_("Analysis");
		case TASK_MAKE_ATTRSCORES_TASKS: return t_("Make attribute score tasks");
		case TASK_ATTRSCORES: return t_("Attributes scores");
		case TASK_SONGSCORE: return t_("Song scores");
		case TASK_MAKE_REVERSEPATTERN_TASK: return t_("Convert score to pattern");
		case TASK_REVERSEPATTERN: return t_("Reverse pattern");
		case TASK_LYRICS: return t_("Lyrics");
		case TASK_MAKE_LYRICS_TASK: return t_("Make lyrics task");
		case TASK_LYRICS_TRANSLATE: return t_("Translate lyrics");
		default: return "<error>";
	}
}

void AI_Task::CreateInput() {
	switch (type) {
		case TASK_PATTERNMASK: CreateInput_PatternMask(); break;
		case TASK_MAKE_PATTERN_TASKS: break;
		case TASK_PATTERN: CreateInput_Pattern(); break;
		case TASK_ANALYSIS: CreateInput_Analysis(); break;
		case TASK_MAKE_ATTRSCORES_TASKS: break;
		case TASK_ATTRSCORES: CreateInput_AttrScores(); break;
		case TASK_SONGSCORE: break;
		case TASK_MAKE_REVERSEPATTERN_TASK: break;
		case TASK_REVERSEPATTERN: break;
		case TASK_LYRICS: CreateInput_Lyrics(); break;
		case TASK_LYRICS_TRANSLATE: CreateInput_LyricsTranslate(); break;
		case TASK_MAKE_LYRICS_TASK: break;
		default: break;
	}
	
	Load();
}

void AI_Task::Process() {
	processing = true;
	
	bool ok = true;
	
	if (output.IsEmpty())
		Load();
	
	if (input.IsEmpty())
		CreateInput();
	
	input.Replace("\r","");
	output = TrimBoth(output);
	
	if (output.IsEmpty())
		Load();
	
	if (!input.IsEmpty() && output.IsEmpty() && type != TASK_MAKE_PATTERN_TASKS && type != TASK_MAKE_ATTRSCORES_TASKS) {
		if (output.IsEmpty())
			Load();
		DUMP(GetInputHash());
		if (output.IsEmpty())
			ok = RunOpenAI();
	}
	
	output.Replace("\r","");
	
	if (ok) {
		switch (type) {
			case TASK_PATTERNMASK: Process_PatternMask(); break;
			case TASK_MAKE_PATTERN_TASKS: Process_MakePatternTasks();break;
			case TASK_PATTERN: Process_Pattern(); break;
			case TASK_ANALYSIS: Process_Analysis(); break;
			case TASK_MAKE_ATTRSCORES_TASKS: Process_MakeAttrScores(); break;
			case TASK_ATTRSCORES: Process_AttrScores(); break;
			case TASK_SONGSCORE: Process_SongScores(); break;
			case TASK_MAKE_REVERSEPATTERN_TASK: Process_MakeReversePattern(); break;
			case TASK_REVERSEPATTERN: Process_ReversePattern(); break;
			case TASK_MAKE_LYRICS_TASK: Process_MakeLyricsTask(); break;
			case TASK_LYRICS: Process_Lyrics(); break;
			case TASK_LYRICS_TRANSLATE: Process_LyricsTranslate(); break;
			default: break;
		}
	}
	
	if (!failed) ready = true;
	skip_load = false;
	processing = false;
	changed = true;
}

bool AI_Task::RunOpenAI() {
	output.Clear();
	
	if (!response_length) {
		LOG("warning: no response length set");
		response_length = 1024;
	}
	String prompt = input;
	LOG(prompt);
	
	prompt.Replace("\n", "\\n");
	prompt.Replace("\"", "\\\"");
	
	String txt = R"(
    {
        "model": "text-davinci-003",
        "prompt": ")" + prompt + R"(",
        "max_tokens": )" + IntStr(response_length) + R"(,
        "temperature": 1
    }
    )";
    LOG(txt);
	
	nlohmann::json json = nlohmann::json::parse(txt.Begin(), txt.End());
	try {
		auto completion = openai::completion().create(json);
	    LOG("Response is:\n" << completion.dump(2));
	    
	    OpenAiResponse response;
	    LoadFromJson(response, String(completion.dump(2)));
	    //LOG(response.ToString());
	    
	    if (response.choices.GetCount())
			output = response.choices[0].text;
	    else
	        output.Clear();
	}
	catch (std::runtime_error e) {
		SetError(e.what());
		return false;
	}
	
	// Fix unicode formatting
	output = ToUnicode(output, CHARSET_UTF8).ToString();
	
	changed = true;
	Store();
	return true;
}
