#include "Task.h"


BEGIN_TEXTLIB_NAMESPACE



void TaskMgrConfig::CreateDefaultTaskRules() {
	
	AddRule(AITASK_TRANSLATE, "translate")
		.Input(&AiTask::CreateInput_Translate)
			.Arg(V_ARGS, 3, 3)
		.Process(&AiTask::Process_Translate);
	
	AddRule(AITASK_GET_SUGGESTION_ATTRIBUTES, "get suggestion attributes")
		.Input(&AiTask::CreateInput_GetSuggestionAttributes)
			.Arg(V_ARGS, 1, 100)
		.Process(&AiTask::Process_GetSuggestionAttributes)
		;
	
	AddRule(AITASK_GET_STRUCTURE_SUGGESTIONS, "get structure suggestions")
		.Input(&AiTask::CreateInput_GetStructureSuggestions)
			.Arg(V_ARGS, 4, 4)
		.Process(&AiTask::Process_GetStructureSuggestions)
		;

	AddRule(AITASK_CREATE_IMAGE, "create image")
		.ImageTask()
		.Input(&AiTask::CreateInput_CreateImage)
			.Arg(V_ARGS, 3, 3)
		.Process(&AiTask::Process_CreateImage)
		;
	
	AddRule(AITASK_EDIT_IMAGE, "edit image")
		.ImageTask()
		.ImageEditTask()
		.Input(&AiTask::CreateInput_EditImage)
			.Arg(V_ARGS, 2, 2)
		.Process(&AiTask::Process_EditImage)
		;
	
	AddRule(AITASK_VARIATE_IMAGE, "variate image")
		.ImageTask()
		.ImageVariateTask()
		.Input(&AiTask::CreateInput_VariateImage)
			.Arg(V_ARGS, 1, 1)
		.Process(&AiTask::Process_VariateImage)
		;
	
	AddRule(AITASK_GET_SONG_DATA_ANALYSIS, "get song data analysis")
		.Input(&AiTask::CreateInput_GetTextDataAnalysis)
			.Arg(V_ARGS, 1, 1)
		.Process(&AiTask::Process_GetTextDataAnalysis)
		;
	
	AddRule(AITASK_GET_ACTION_ANALYSIS, "get action analysis")
		.Input(&AiTask::CreateInput_GetActionAnalysis)
			.Arg(V_ARGS, 1, 1)
		.Process(&AiTask::Process_GetActionAnalysis)
		;
	
	AddRule(AITASK_GET_LYRICS_PHRASE, "get action analysis")
		.Input(&AiTask::CreateInput_GetScriptPhrase)
			.Arg(V_ARGS, 1, 1)
		.Process(&AiTask::Process_GetScriptPhrase)
		;
		
	AddRule(AITASK_RAW_COMPLETION, "raw prompt completion")
		.Process(&AiTask::Process_RawCompletion)
		;
	
	AddRule(AITASK_GET_TOKEN_DATA, "get token data")
		.Input(&AiTask::CreateInput_GetTokenData)
			.Arg(V_ARGS, 1, 1)
		.Process(&AiTask::Process_GetTokenData)
		;
	
	AddRule(AITASK_GET_PHRASE_DATA, "get phrase data")
		.Input(&AiTask::CreateInput_GetPhraseData)
			.Arg(V_ARGS, 1, 1)
		.Process(&AiTask::Process_GetPhraseData)
		;
	
	AddRule(AITASK_GET_ATTRIBUTES, "get attributes")
		.Input(&AiTask::CreateInput_GetAttributes)
			.Arg(V_ARGS, 1, 1)
		.Process(&AiTask::Process_GetAttributes)
		;
	
	AddRule(AITASK_NANA_DATA, "get nana data")
		.Input(&AiTask::CreateInput_GetNanaData)
			.Arg(V_ARGS, 1, 1)
		.Process(&AiTask::Process_GetNanaData)
		;
	
	AddRule(AITASK_LYRICS_SOLVER, "scripts solver")
		.Input(&AiTask::CreateInput_ScriptSolver)
			.Arg(V_ARGS, 1, 1)
		.Process(&AiTask::Process_ScriptSolver)
		;
	
	AddRule(AITASK_STORY_SOLVER, "story solver")
		.Input(&AiTask::CreateInput_StorySolver)
			.Arg(V_ARGS, 1, 1)
		.Process(&AiTask::Process_StorySolver)
		;
	
	AddRule(AITASK_GET_PROGRAM_DATA_ANALYSIS, "scripts solver")
		.Input(&AiTask::CreateInput_GetProgramDataAnalysis)
			.Arg(V_ARGS, 1, 1)
		.Process(&AiTask::Process_GetProgramDataAnalysis)
		;
	
}

void TaskMgrConfig::Load() {
	LoadFromFile(*this, ConfigFile("taskmgr.bin"));
}

void TaskMgrConfig::Store() {
	StoreToFile(*this, ConfigFile("taskmgr.bin"));
}

void TaskMgrConfig::Process() {
	while (running) {
		TaskMgr::Single().Process();
		
		Sleep(10);
	}
	
	stopped = true;
}













AiTask& TaskMgr::AddTask() {
	task_lock.Enter();
	AiTask& t = tasks .Add();
	task_lock.Leave();
	return t;
}

void TaskMgr::Process() {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	
	iters++;
	
	int ready = 0, got_ready = 0;
	
	this->total = tasks.GetCount();
	
	for(int i = 0; i < tasks.GetCount() && mgr.running && !Thread::IsShutdownThreads(); i++) {
		AiTask& t = tasks[i];
		if (!t.ready) {
			ProcessSingle(i);
			if (t.ready) {
				actual++;
				ready++;
				got_ready++;
				task_i++;
			}
		}
		else
			ready++;
	}
	this->actual = ready;
	
	if (!got_ready) {
		int tried_retry = 0;
		for (AiTask& t : tasks) {
			if (t.fatal_error)
				continue;
			if (t.failed && !t.ready && t.tries < mgr.max_tries) {
				t.tries++;
				t.Retry(false, false);
				tried_retry++;
			}
		}
	}
}

void TaskMgr::ProcessSingle(int task_i) {
	task_lock.Enter();
	
	AiTask& t = tasks[task_i];
	
	Index<AiTask*> seen;
	
	// Skip ready, failed and those with non-ready dependencies
	if (t.ready || t.failed)
		;
	else {
		status = "Processing #" + IntStr(task_i);
		t.Process();
		status = "";
	}
	
	task_lock.Leave();
}

void TaskMgr::Translate(String orig_lang, String orig_txt, String trans_lang, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	const TaskRule& r = mgr.GetRule(AITASK_TRANSLATE);
	TaskMgr& p = *this;
	
	AiTask& t = AddTask();
	t.rule = &r;
	t.args << orig_lang << orig_txt << trans_lang;
	t.WhenResult << WhenResult;
}

void TaskMgr::RawCompletion(String prompt, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	const TaskRule& r = mgr.GetRule(AITASK_RAW_COMPLETION);
	TaskMgr& p = *this;
	
	AiTask& t = AddTask();
	t.rule = &r;
	t.raw_input = prompt;
	t.WhenResult << WhenResult;
}

void TaskMgr::GetStructureSuggestions(String req, String avoid, String desc, int total, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	const TaskRule& r = mgr.GetRule(AITASK_GET_STRUCTURE_SUGGESTIONS);
	TaskMgr& p = *this;
	
	AiTask& t = AddTask();
	t.rule = &r;
	t.args << req << avoid << desc << IntStr(total);
	t.WhenResult << WhenResult;
}

void TaskMgr::GetSuggestionAttributes(Vector<String>& structs, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	const TaskRule& r = mgr.GetRule(AITASK_GET_SUGGESTION_ATTRIBUTES);
	TaskMgr& p = *this;
	
	AiTask& t = AddTask();
	t.rule = &r;
	t.args <<= structs;
	t.WhenResult << WhenResult;
}

void TaskMgr::CreateImage(String prompt, int count, Event<Array<Image>&> WhenResult, int reduce_size_mode, Event<> WhenError) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	const TaskRule& r = mgr.GetRule(AITASK_CREATE_IMAGE);
	TaskMgr& p = *this;
	
	AiTask& t = AddTask();
	t.rule = &r;
	t.args << prompt << IntStr(count) << IntStr(reduce_size_mode);
	t.WhenResultImages << WhenResult;
	t.WhenError << WhenError;
}

void TaskMgr::GetEditImage(Image orig, Image mask, String prompt, int count, Event<Array<Image>&> WhenResult, Event<> WhenError) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	const TaskRule& r = mgr.GetRule(AITASK_EDIT_IMAGE);
	TaskMgr& p = *this;
	
	if (orig.GetSize() != mask.GetSize()) {
		WhenError();
		return;
	}
	
	{
		Size sz = orig.GetSize();
		ImageBuffer ib(sz);
		RGBA* dst = ib.Begin();
		const RGBA* orig_it = orig.Begin();
		const RGBA* mask_it = mask.Begin();
		const RGBA* mask_end = mask.End();
		RGBA dark;
		dark.r = 0;
		dark.g = 0;
		dark.b = 0;
		dark.a = 0;
		while (mask_it != mask_end) {
			if (mask_it->a == 0)
				*dst = *orig_it;
			else
				*dst = dark;
			mask_it++;
			orig_it++;
			dst++;
		}
		orig = ib;
	}
	
	AiTask& t = AddTask();
	t.rule = &r;
	t.send_images << orig;
	t.args << prompt << IntStr(count);
	t.WhenResultImages << WhenResult;
	t.WhenError << WhenError;
}

void TaskMgr::VariateImage(Image orig, int count, Event<Array<Image>&> WhenResult, Event<> WhenError) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	const TaskRule& r = mgr.GetRule(AITASK_VARIATE_IMAGE);
	TaskMgr& p = *this;
	
	AiTask& t = AddTask();
	t.rule = &r;
	t.send_images << orig;
	t.args << IntStr(count);
	t.WhenResultImages << WhenResult;
	t.WhenError << WhenError;
}

void TaskMgr::GetTextDataAnalysis(const TextDataAnalysisArgs& args, Event<String> WhenResult, bool keep_going) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	const TaskRule& r = mgr.GetRule(AITASK_GET_SONG_DATA_ANALYSIS);
	TaskMgr& p = *this;


	String s = args.Get();

	task_lock.Enter();
	AiTask& t = tasks.Add();
	t.rule = &r;
	t.args << s;
	t.WhenResult << WhenResult;
	t.keep_going = keep_going;
	task_lock.Leave();
}

void TaskMgr::GetActionAnalysis(const ActionAnalysisArgs& args, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	const TaskRule& r = mgr.GetRule(AITASK_GET_ACTION_ANALYSIS);
	TaskMgr& p = *this;

	String s = args.Get();

	task_lock.Enter();
	AiTask& t = tasks.Add();
	t.rule = &r;
	t.args << s;
	t.WhenResult << WhenResult;
	task_lock.Leave();
}

void TaskMgr::GetScriptPhrase(const ScriptPhraseArgs& args, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	const TaskRule& r = mgr.GetRule(AITASK_GET_LYRICS_PHRASE);
	TaskMgr& p = *this;
	
	String s = args.Get();
	
	task_lock.Enter();
	AiTask& t = tasks.Add();
	t.rule = &r;
	t.args << s;
	t.WhenResult << WhenResult;
	task_lock.Leave();
}

void TaskMgr::GetTokenData(const TokenArgs& args, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	const TaskRule& r = mgr.GetRule(AITASK_GET_TOKEN_DATA);
	TaskMgr& p = *this;
	
	String s = args.Get();
	
	task_lock.Enter();
	AiTask& t = tasks.Add();
	t.rule = &r;
	t.args << s;
	t.WhenResult << WhenResult;
	task_lock.Leave();
}

void TaskMgr::GetPhraseData(const PhraseArgs& args, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	const TaskRule& r = mgr.GetRule(AITASK_GET_PHRASE_DATA);
	TaskMgr& p = *this;
	
	String s = args.Get();
	
	task_lock.Enter();
	AiTask& t = tasks.Add();
	t.rule = &r;
	t.args << s;
	t.WhenResult << WhenResult;
	task_lock.Leave();
}

void TaskMgr::GetAttributes(const AttrArgs& args, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	const TaskRule& r = mgr.GetRule(AITASK_GET_ATTRIBUTES);
	TaskMgr& p = *this;
	
	String s = args.Get();
	
	task_lock.Enter();
	AiTask& t = tasks.Add();
	t.rule = &r;
	t.args << s;
	t.WhenResult << WhenResult;
	task_lock.Leave();
}

void TaskMgr::GetNanaData(const NanaArgs& args, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	const TaskRule& r = mgr.GetRule(AITASK_NANA_DATA);
	TaskMgr& p = *this;
	
	String s = args.Get();
	
	task_lock.Enter();
	AiTask& t = tasks.Add();
	t.rule = &r;
	t.args << s;
	t.WhenResult << WhenResult;
	task_lock.Leave();
}

void TaskMgr::GetScriptSolver(const ScriptSolverArgs& args, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	const TaskRule& r = mgr.GetRule(AITASK_LYRICS_SOLVER);
	TaskMgr& p = *this;
	
	String s = args.Get();
	
	task_lock.Enter();
	AiTask& t = tasks.Add();
	t.rule = &r;
	t.args << s;
	t.WhenResult << WhenResult;
	task_lock.Leave();
}

void TaskMgr::GetStorySolver(const StorySolverArgs& args, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	const TaskRule& r = mgr.GetRule(AITASK_STORY_SOLVER);
	TaskMgr& p = *this;
	
	String s = args.Get();
	
	task_lock.Enter();
	AiTask& t = tasks.Add();
	t.rule = &r;
	t.args << s;
	t.WhenResult << WhenResult;
	task_lock.Leave();
}

void TaskMgr::GetProgramDataAnalysis(const ProgramDataAnalysisArgs& args, Event<String> WhenResult, bool keep_going) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	const TaskRule& r = mgr.GetRule(AITASK_GET_PROGRAM_DATA_ANALYSIS);
	TaskMgr& p = *this;


	String s = args.Get();

	task_lock.Enter();
	AiTask& t = tasks.Add();
	t.rule = &r;
	t.args << s;
	t.WhenResult << WhenResult;
	t.keep_going = keep_going;
	task_lock.Leave();
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














TaskRule& TaskRule::Input(void (AiTask::*fn)()) {
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

TaskRule& TaskRule::Process(void (AiTask::*fn)()) {
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

TaskRule& TaskRule::ImageTask(bool b) {
	image_task = b;
	return *this;
}

TaskRule& TaskRule::ImageEditTask(bool b) {
	imageedit_task = b;
	return *this;
}

TaskRule& TaskRule::ImageVariateTask(bool b) {
	imagevariate_task = b;
	return *this;
}



END_TEXTLIB_NAMESPACE

