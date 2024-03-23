#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


void TaskManager::DoSongs(int ds_i, int fn) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_SONGS;
	t.cb = THISBACK1(GetComponents, &t);
	t.ds_i = ds_i;
	t.batch_i = 0;
	t.fn = fn;
	lock.LeaveWrite();
}

void TaskManager::DoTokens(int ds_i, int fn) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_TOKENS;
	t.cb = THISBACK1(GetTokenData, &t);
	t.ds_i = ds_i;
	t.batch_i = 0;
	t.fn = fn;
	lock.LeaveWrite();
}

void TaskManager::DoUnknownTokenPairs(int ds_i, int fn) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_UNKNOWN_TOKEN_PAIRS;
	t.cb = THISBACK1(GetUnknownTokenPairs, &t);
	t.ds_i = ds_i;
	t.batch_i = 0;
	t.fn = fn;
	lock.LeaveWrite();
}

void TaskManager::DoAmbiguousWordPairs(int ds_i, int fn) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_AMBIGUOUS_WORD_PAIRS;
	t.cb = THISBACK1(GetAmbiguousWordPairs, &t);
	t.ds_i = ds_i;
	t.batch_i = 0;
	t.fn = fn;
	lock.LeaveWrite();
}

void TaskManager::DoVirtualPhrases(int ds_i, int fn) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_VIRTUAL_PHRASES;
	t.cb = THISBACK1(GetVirtualPhrases, &t);
	t.ds_i = ds_i;
	t.batch_i = 0;
	t.fn = fn;
	lock.LeaveWrite();
}

void TaskManager::DoPhrases(int ds_i, int fn) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_PHRASES;
	t.cb = THISBACK1(GetPhrases, &t);
	t.ds_i = ds_i;
	t.batch_i = 0;
	t.fn = fn;
	lock.LeaveWrite();
}

/*void TaskManager::DoNana(int ds_i, int fn, Component& song, Callback OnReady, int line_i, int part_i) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_NANA;
	t.cb = THISBACK1(GetNana, &t);
	t.ds_i = ds_i;
	t.batch_i = 0;
	t.fn = fn;
	t.song = &song;
	t.on_ready = OnReady;
	t.line_i = line_i;
	t.part_i = part_i;
	lock.LeaveWrite();
}*/

void TaskManager::DoContainer(int ds_i, int fn, Callback2<int,int> update) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_CONTAINER;
	t.cb = THISBACK1(GetContainer, &t);
	t.ds_i = ds_i;
	t.batch_i = 0;
	t.fn = fn;
	t.update = update;
	lock.LeaveWrite();
}

void TaskManager::DoActionlistCache(int ds_i) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	Task task;
	task.ds_i = ds_i;
	
	String dir = ConfigFile("share") + DIR_SEPS + "task_actionlist" + DIR_SEPS +
		sda.datasets.GetKey(ds_i);
	if (!DirectoryExists(dir))
		return;
	
	FindFile ff(dir + DIR_SEPS + "*.txt");
	do {
		String title = GetFileTitle(ff.GetName());
		if (title == "." ||title == "..") continue;
		String content = LoadFile(ff.GetPath());
		if (content.Find("RGB(") >= 0)
			OnActionlistColors(content, &task);
		else
			OnActionlistAttrs(content, &task);
	}
	while (ff.Next());
}

void TaskManager::DoActionlist(int ds_i, int fn) {
	if (IsInTaskList(TASK_ACTIONLIST))
		return;
	
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	if (uniq_acts.IsEmpty()) {
		for (const ActionHeader& ah : da.actions.GetKeys()) {
			uniq_acts.GetAdd(ah.action).GetAdd(ah.arg, 0)++;
		}
		struct Sorter {
			bool operator()(const VectorMap<String, int>& a, const VectorMap<String, int>& b) const {
				return a.GetCount() > b.GetCount();
			}
		};
		SortByValue(uniq_acts, Sorter());
		for (auto& v : uniq_acts.GetValues())
			SortByValue(v, StdGreater<int>());
	}
	
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_ACTIONLIST;
	t.cb = THISBACK1(GetActionlist, &t);
	t.ds_i = ds_i;
	t.batch_i = 0;
	t.fn = fn;
	lock.LeaveWrite();
}

void TaskManager::DoActionParallel(int ds_i, int fn) {
	if (IsInTaskList(TASK_ACTION_PARALLELS))
		return;
	
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_ACTION_PARALLELS;
	t.cb = THISBACK1(GetActionParallels, &t);
	t.ds_i = ds_i;
	t.batch_i = 0;
	t.fn = fn;
	lock.LeaveWrite();
}

void TaskManager::DoActionTransition(int ds_i, int fn) {
	if (IsInTaskList(TASK_ACTION_TRANSITIONS))
		return;
	
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_ACTION_TRANSITIONS;
	t.cb = THISBACK1(GetActionTransitions, &t);
	t.ds_i = ds_i;
	t.batch_i = 0;
	t.fn = fn;
	lock.LeaveWrite();
}

void TaskManager::DoWordFix(int ds_i, int fn) {
	//if (IsInTaskList(TASK_WORD_FIX))
	//	return;
	
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_WORD_FIX;
	t.cb = THISBACK1(GetWordFix, &t);
	t.ds_i = ds_i;
	t.batch_i = 0;
	t.fn = fn;
	lock.LeaveWrite();
}

void TaskManager::DoWords(int ds_i, int fn) {
	if (IsInTaskList(TASK_WORD_DATA))
		return;
	
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_WORD_DATA;
	t.cb = THISBACK1(GetWordData, &t);
	t.ds_i = ds_i;
	t.batch_i = 0;
	t.fn = fn;
	lock.LeaveWrite();
}

void TaskManager::DoWordnet(int ds_i, int fn) {
	if (IsInTaskList(TASK_WORDNET))
		return;
	
	PromptOK("TODO");
	return;
	
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_WORDNET;
	t.cb = THISBACK1(GetWordnet, &t);
	t.ds_i = ds_i;
	t.batch_i = 0;
	t.fn = fn;
	lock.LeaveWrite();
}

void TaskManager::DoAttributes(int ds_i, int fn) {
	if (IsInTaskList(TASK_ATTRIBUTES))
		return;
	
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_ATTRIBUTES;
	t.cb = THISBACK1(GetAttributes, &t);
	t.ds_i = ds_i;
	t.batch_i = 0;
	t.fn = fn;
	lock.LeaveWrite();
}


END_TEXTLIB_NAMESPACE

