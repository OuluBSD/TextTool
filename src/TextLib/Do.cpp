#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


void TaskManager::DoSongs(int fn) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_SONGS;
	t.cb = THISBACK1(GetComponents, &t);
	
	t.batch_i = 0;
	t.fn = fn;
	lock.LeaveWrite();
}

void TaskManager::DoTokensUsingExisting(int fn) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_TOKENS_USING_EXISTING;
	t.cb = THISBACK1(GetTokenDataUsingExisting, &t);
	
	t.batch_i = 0;
	t.fn = fn;
	lock.LeaveWrite();
}

void TaskManager::DoTokens(int fn) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_TOKENS;
	t.cb = THISBACK1(GetTokenData, &t);
	
	t.batch_i = 0;
	t.fn = fn;
	lock.LeaveWrite();
}

void TaskManager::DoUnknownTokenPairs(int fn) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_UNKNOWN_TOKEN_PAIRS;
	t.cb = THISBACK1(GetUnknownTokenPairs, &t);
	
	t.batch_i = 0;
	t.fn = fn;
	lock.LeaveWrite();
}

void TaskManager::DoAmbiguousWordPairsUsingExisting(int fn) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_AMBIGUOUS_WORD_PAIRS;
	t.cb = THISBACK1(GetAmbiguousWordPairsUsingExisting, &t);
	
	t.batch_i = 0;
	t.fn = fn;
	lock.LeaveWrite();
}

void TaskManager::DoAmbiguousWordPairs(int fn) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_AMBIGUOUS_WORD_PAIRS;
	t.cb = THISBACK1(GetAmbiguousWordPairs, &t);
	
	t.batch_i = 0;
	t.fn = fn;
	lock.LeaveWrite();
}

void TaskManager::DoVirtualPhrases(int fn) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_VIRTUAL_PHRASES;
	t.cb = THISBACK1(GetVirtualPhrases, &t);
	
	t.batch_i = 0;
	t.fn = fn;
	lock.LeaveWrite();
}

void TaskManager::DoVirtualPhrasesUsingExisting(int fn) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_VIRTUAL_PHRASES_USING_EXISTING;
	t.cb = THISBACK1(GetVirtualPhrasesUsingExisting, &t);
	
	t.batch_i = 0;
	t.fn = fn;
	lock.LeaveWrite();
}

void TaskManager::DoPhrases(int fn) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_PHRASES;
	t.cb = THISBACK1(GetPhrases, &t);
	
	t.batch_i = 0;
	t.fn = fn;
	lock.LeaveWrite();
}

void TaskManager::DoContainer(int fn, Callback2<int,int> update) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_CONTAINER;
	t.cb = THISBACK1(GetContainer, &t);
	
	t.batch_i = 0;
	t.fn = fn;
	t.update = update;
	lock.LeaveWrite();
}

void TaskManager::DoActionlistCache() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	Task task;
	
	String& share = MetaDatabase::Single().share;
	String dir = ConfigFile(share) + DIR_SEPS + "task_actionlist";
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

void TaskManager::DoActionlist(int fn) {
	//if (IsInTaskList(TASK_ACTIONLIST))
	//	return;
	
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	{
		uniq_acts.Clear();
		for (const ActionHeader& ah : da.actions.GetKeys()) {
			if ((ah.action.GetCount() && ah.action[0] == '\"') || (ah.arg.GetCount() && ah.arg[0] == '\"'))
				continue;
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
	
	t.batch_i = 0;
	t.fn = fn;
	lock.LeaveWrite();
}

void TaskManager::DoActionlistUsingExisting(int fn) {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	{
		uniq_acts.Clear();
		for (const ActionHeader& ah : da.actions.GetKeys()) {
			if ((ah.action.GetCount() && ah.action[0] == '\"') || (ah.arg.GetCount() && ah.arg[0] == '\"'))
				continue;
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
	t.cb = THISBACK1(GetActionlistUsingExisting, &t);
	
	t.batch_i = 0;
	t.fn = fn;
	lock.LeaveWrite();
}

void TaskManager::DoActionParallel(int fn) {
	//if (IsInTaskList(TASK_ACTION_PARALLELS))
	//	return;
	
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_ACTION_PARALLELS;
	t.cb = THISBACK1(GetActionParallels, &t);
	
	t.batch_i = 0;
	t.fn = fn;
	lock.LeaveWrite();
}

void TaskManager::DoActionTransition(int fn) {
	//if (IsInTaskList(TASK_ACTION_TRANSITIONS))
	//	return;
	
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_ACTION_TRANSITIONS;
	t.cb = THISBACK1(GetActionTransitions, &t);
	
	t.batch_i = 0;
	t.fn = fn;
	lock.LeaveWrite();
}

void TaskManager::DoWordFix(int fn) {
	//if (IsInTaskList(TASK_WORD_FIX))
	//	return;
	
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_WORD_FIX;
	t.cb = THISBACK1(GetWordFix, &t);
	
	t.batch_i = 0;
	t.fn = fn;
	lock.LeaveWrite();
}

void TaskManager::DoWords(int lng_i, int fn) {
	//if (IsInTaskList(TASK_WORD_DATA))
	//	return;
	
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_WORD_DATA;
	t.cb = THISBACK1(GetWordData, &t);
	
	t.batch_i = 0;
	t.fn = fn;
	lock.LeaveWrite();
}

void TaskManager::DoWordsUsingExisting(int lng_i, int fn) {
	//if (IsInTaskList(TASK_WORD_DATA))
	//	return;
	
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_WORD_DATA_USING_EXISTING;
	t.cb = THISBACK1(GetWordDataUsingExisting, &t);
	
	t.batch_i = 0;
	t.fn = fn;
	lock.LeaveWrite();
}

void TaskManager::DoWordnet(int fn) {
	//if (IsInTaskList(TASK_WORDNET))
	//	return;
	
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_WORDNET;
	t.cb = THISBACK1(GetWordnet, &t);
	
	t.batch_i = 0;
	t.fn = fn;
	lock.LeaveWrite();
}

void TaskManager::DoAttributes(int fn) {
	//if (IsInTaskList(TASK_ATTRIBUTES))
	//	return;
	
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	lock.EnterWrite();
	Task& t = task_list.Add();
	t.type = TASK_ATTRIBUTES;
	t.cb = THISBACK1(GetAttributes, &t);
	
	t.batch_i = 0;
	t.fn = fn;
	lock.LeaveWrite();
}


END_TEXTLIB_NAMESPACE

