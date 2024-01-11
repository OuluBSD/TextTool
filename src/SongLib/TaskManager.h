#ifndef _SongLib_SongDataManager_h_
#define _SongLib_SongDataManager_h_

namespace SongLib {


typedef enum {
	TASK_TOKENS,
	TASK_AMBIGUOUS_WORD_PAIRS,
	TASK_VIRTUAL_PHRASES,
	TASK_PHRASES,
	TASK_CONTAINER,
	
	// deprecated?
	TASK_ACTIONLIST,
	TASK_ACTION_PARALLELS,
	TASK_ACTION_TRANSITIONS,
	
	TASK_COUNT
} TaskType;


struct Task {
	TaskType type;
	Callback cb;
	Callback2<int,int> update;
	int fn = -1;
	int ds_i = -1;
	int batch_i = 0;
	bool running = false;
	Vector<int> tmp;
};


class TaskManager {
	RWMutex lock;
	Array<Task> task_list;
	bool running = false, stopped = true;
	TaskMgr* pipe = 0;
	
	TokenArgs token_args;
	PhraseArgs phrase_args;
	
	int per_action_clrs = 60;
	int per_action_attrs = 40;
	VectorMap<String, VectorMap<String, int>> uniq_acts;
	
	void RealizePipe();
	void Process();
	void GetActionlist(Task* t);
	void OnActionlistColors(String result, Task* t);
	void OnActionlistAttrs(String result, Task* t);
	void GetActionTransitions(Task* t);
	void GetActionParallels(Task* t);
	void GetTokenData(Task* t);
	void OnTokenData(String result, Task* t);
	void GetAmbiguousWordPairs(Task* t);
	void OnAmbiguousWordPairs(String result, Task* t);
	void GetVirtualPhrases(Task* t);
	void OnVirtualPhrases(String result, Task* t);
	void OnVirtualPhraseTypes(String result, Task* t);
	void DoClear(Task* tp);
	void GetPhrases(Task* tp);
	void OnPhraseColors(String result, Task* t);
	void OnPhraseAttrs(String result, Task* t);
	void OnPhraseActions(String result, Task* t);
	void OnPhraseScores(String result, Task* t);
	void MakeNana(Task* t);
	void GetRhymeContainers(Task* t);
	void GetRhymeContainersFromTemplates(Task* t);
	void GetContainer(Task* t);
	
	void RemoveTask(Task& t);
	
public:
	typedef TaskManager CLASSNAME;
	TaskManager();
	
	void Start();
	void Stop();
	void Clear();
	bool IsInTaskList(TaskType type) const;
	TaskMgr* GetPipe() const {return pipe;}
	TaskMgr& MakePipe() {if (!pipe) RealizePipe(); return *pipe;}
	
	static TaskManager& Single() {static TaskManager o; return o;}
	
	void DoTokens(int ds_i, int fn);
	void DoAmbiguousWordPairs(int ds_i, int fn);
	void DoVirtualPhrases(int ds_i, int fn);
	void DoPhrases(int ds_i, int fn);
	void DoContainer(int ds_i, int fn, Callback2<int,int> update);
	
	void DoActionlistCache(int ds_i);
	void DoActionlist(int ds_i, int fn);
	
	void DoActionParallel(int ds_i, int fn);
	void DoActionTransition(int ds_i, int fn);
	
};

bool GetTypePhrase(Vector<int>& types, const DatasetAnalysis& da, int next_w_i, int w_i, int prev_w_i);


}

#endif
