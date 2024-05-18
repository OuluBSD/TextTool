#include "Task.h"


BEGIN_TEXTLIB_NAMESPACE



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













template <class T>
String TaskMgr::MakeName(T& o, int appmode, const char* name) {
	String s;
	if (appmode >= 0 && appmode < DB_COUNT)
		s << GetAppModeString(appmode) << ": ";
	s << name;
	s << " #" << o.fn;
	return s;
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

void TaskMgr::Translate(String orig_lang, String orig_txt, String trans_lang, Event<String> WhenResult, bool slightly_dialect) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	TaskMgr& p = *this;
	
	AiTask& t = AddTask();
	
	t.SetRule(AITASK_TRANSLATE, "translate")
		.Input(&AiTask::CreateInput_Translate)
		.Process(&AiTask::Process_Default);
	
	t.args << orig_lang << orig_txt << trans_lang << IntStr(slightly_dialect);
	t.WhenResult << WhenResult;
}

void TaskMgr::RawCompletion(String prompt, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	TaskMgr& p = *this;
	
	AiTask& t = AddTask();
	
	t.SetRule(AITASK_RAW_COMPLETION, "raw prompt completion")
		.Process(&AiTask::Process_Default);
		
	t.raw_input = prompt;
	t.WhenResult << WhenResult;
}

void TaskMgr::GetStructureSuggestions(int appmode, const StructureArgs& args, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	TaskMgr& p = *this;
	
	String s = args.Get();
	
	task_lock.Enter();
	AiTask& t = tasks.Add();
	t.appmode = appmode;
	
	t.SetRule(AITASK_GET_STRUCTURE_SUGGESTIONS, MakeName(args, appmode, "get structure suggestions"))
		.Input(&AiTask::CreateInput_GetStructureSuggestions)
		.Process(&AiTask::Process_GetStructureSuggestions);

	t.args << s;
	t.WhenResult << WhenResult;
	task_lock.Leave();
}

void TaskMgr::GetSuggestionAttributes(int appmode, const StructureArgs& args, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	TaskMgr& p = *this;
	
	String s = args.Get();
	
	task_lock.Enter();
	AiTask& t = tasks.Add();
	t.appmode = appmode;
	
	t.SetRule(AITASK_GET_SUGGESTION_ATTRIBUTES, MakeName(args, appmode, "get suggestion attributes"))
		.Input(&AiTask::CreateInput_GetSuggestionAttributes)
		.Process(&AiTask::Process_GetSuggestionAttributes);
	
	t.args << s;
	t.WhenResult << WhenResult;
	task_lock.Leave();
}

void TaskMgr::CreateImage(String prompt, int count, Event<Array<Image>&> WhenResult, int reduce_size_mode, Event<> WhenError) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	TaskMgr& p = *this;
	
	AiTask& t = AddTask();
	
	t.SetRule(AITASK_CREATE_IMAGE, "create image")
		.ImageTask()
		.Input(&AiTask::CreateInput_CreateImage)
		.Process(&AiTask::Process_CreateImage);
	
	t.args << prompt << IntStr(count) << IntStr(reduce_size_mode);
	t.WhenResultImages << WhenResult;
	t.WhenError << WhenError;
}

void TaskMgr::GetEditImage(Image orig, Image mask, String prompt, int count, Event<Array<Image>&> WhenResult, Event<> WhenError) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
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
	
	t.SetRule(AITASK_EDIT_IMAGE, "edit image")
		.ImageTask()
		.ImageEditTask()
		.Input(&AiTask::CreateInput_EditImage)
		.Process(&AiTask::Process_EditImage);
	
	t.send_images << orig;
	t.args << prompt << IntStr(count);
	t.WhenResultImages << WhenResult;
	t.WhenError << WhenError;
}

void TaskMgr::VariateImage(Image orig, int count, Event<Array<Image>&> WhenResult, Event<> WhenError) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	TaskMgr& p = *this;
	
	AiTask& t = AddTask();
	
	t.SetRule(AITASK_VARIATE_IMAGE, "variate image")
		.ImageTask()
		.ImageVariateTask()
		.Input(&AiTask::CreateInput_VariateImage)
		.Process(&AiTask::Process_VariateImage);
	
	t.send_images << orig;
	t.args << IntStr(count);
	t.WhenResultImages << WhenResult;
	t.WhenError << WhenError;
}

void TaskMgr::GetSourceDataAnalysis(int appmode, const SourceDataAnalysisArgs& args, Event<String> WhenResult, bool keep_going) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	TaskMgr& p = *this;
	
	String s = args.Get();
	
	task_lock.Enter();
	AiTask& t = tasks.Add();
	t.appmode = appmode;
	
	t.SetRule(AITASK_GET_SONG_DATA_ANALYSIS, MakeName(args, appmode, "get song data analysis"))
		.Input(&AiTask::CreateInput_GetSourceDataAnalysis)
		.Process(&AiTask::Process_Default);
	
	t.args << s;
	t.WhenResult << WhenResult;
	t.keep_going = keep_going;
	task_lock.Leave();
}

void TaskMgr::GetActionAnalysis(int appmode, const ActionAnalysisArgs& args, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	TaskMgr& p = *this;

	String s = args.Get();

	task_lock.Enter();
	AiTask& t = tasks.Add();
	t.appmode = appmode;
	
	t.SetRule(AITASK_GET_ACTION_ANALYSIS, MakeName(args, appmode, "get action analysis"))
		.Input(&AiTask::CreateInput_GetActionAnalysis)
		.Process(&AiTask::Process_Default);
	
	t.args << s;
	t.WhenResult << WhenResult;
	task_lock.Leave();
}

void TaskMgr::GetTokenData(int appmode, const TokenArgs& args, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	TaskMgr& p = *this;
	
	String s = args.Get();
	
	task_lock.Enter();
	AiTask& t = tasks.Add();
	t.appmode = appmode;
	
	t.SetRule(AITASK_GET_TOKEN_DATA, MakeName(args, appmode, "get token data"))
		.Input(&AiTask::CreateInput_GetTokenData)
		.Process(&AiTask::Process_Default);
	
	t.args << s;
	t.WhenResult << WhenResult;
	task_lock.Leave();
}

void TaskMgr::GetPhraseData(int appmode, const PhraseArgs& args, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	TaskMgr& p = *this;
	
	String s = args.Get();
	
	task_lock.Enter();
	AiTask& t = tasks.Add();
	t.appmode = appmode;
	
	t.SetRule(AITASK_GET_PHRASE_DATA, MakeName(args, appmode, "get phrase data"))
		.Input(&AiTask::CreateInput_GetPhraseData)
		.Process(&AiTask::Process_Default);
	
	t.args << s;
	t.WhenResult << WhenResult;
	task_lock.Leave();
}

void TaskMgr::GetAttributes(int appmode, const AttrArgs& args, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	TaskMgr& p = *this;
	
	String s = args.Get();
	
	task_lock.Enter();
	AiTask& t = tasks.Add();
	t.appmode = appmode;
	
	t.SetRule(AITASK_GET_ATTRIBUTES, MakeName(args, appmode, "get attributes"))
		.Input(&AiTask::CreateInput_GetAttributes)
		.Process(&AiTask::Process_GetAttributes);
	
	t.args << s;
	t.WhenResult << WhenResult;
	task_lock.Leave();
}

void TaskMgr::GetScriptSolver(int appmode, const ScriptSolverArgs& args, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	TaskMgr& p = *this;
	
	String s = args.Get();
	
	task_lock.Enter();
	AiTask& t = tasks.Add();
	t.appmode = appmode;
	t.SetRule(AITASK_LYRICS_SOLVER, MakeName(args, appmode, "scripts solver"))
		.Input(&AiTask::CreateInput_ScriptSolver)
		.Process(&AiTask::Process_Default);
	
	t.args << s;
	t.WhenResult << WhenResult;
	task_lock.Leave();
}

void TaskMgr::GetLeadSolver(int appmode, const LeadSolverArgs& args, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	TaskMgr& p = *this;
	
	String s = args.Get();
	
	task_lock.Enter();
	AiTask& t = tasks.Add();
	t.appmode = appmode;
	t.SetRule(AITASK_LEAD_SOLVER, MakeName(args, -1, "lead solver"))
		.Input(&AiTask::CreateInput_LeadSolver)
		.Process(&AiTask::Process_Default);
	
	t.args << s;
	t.WhenResult << WhenResult;
	task_lock.Leave();
}

void TaskMgr::GetSocial(const SocialArgs& args, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	TaskMgr& p = *this;
	
	String s = args.Get();
	
	task_lock.Enter();
	AiTask& t = tasks.Add();
	t.SetRule(AITASK_LEAD_SOLVER, MakeName(args, -1, "social"))
		.Input(&AiTask::CreateInput_Social)
		.Process(&AiTask::Process_Default);
	
	t.args << s;
	t.WhenResult << WhenResult;
	task_lock.Leave();
}

void TaskMgr::GetVision(const String& jpeg, const VisionArgs& args, Event<String> WhenResult) {
	const TaskMgrConfig& mgr = TaskMgrConfig::Single();
	TaskMgr& p = *this;
	
	String s = args.Get();
	
	task_lock.Enter();
	AiTask& t = tasks.Add();
	t.SetRule(AITASK_VISION, MakeName(args, -1, "vision"))
		.Input(&AiTask::CreateInput_Vision)
		.Process(&AiTask::Process_Default);
	
	t.args << s;
	t.jpeg = jpeg;
	t.WhenResult << WhenResult;
	task_lock.Leave();
}

















TaskRule& TaskRule::SetRule(int code, const String& name) {
	this->code = code;
	this->name = name;
	return *this;
}

TaskRule& TaskRule::Input(void (AiTask::*fn)()) {
	this->input = fn;
	return *this;
}

TaskRule& TaskRule::Process(void (AiTask::*fn)()) {
	process = fn;
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

