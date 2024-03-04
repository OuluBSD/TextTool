#ifndef _SocialLib_ProgramDataManager_h_
#define _SocialLib_ProgramDataManager_h_

namespace SocialLib {


typedef enum {
	TASK_CLEAR,
	TASK_SONGS,
	TASK_TOKENS,
	TASK_WORD_DATA,
	TASK_WORD_FIX,
	TASK_UNKNOWN_TOKEN_PAIRS,
	TASK_AMBIGUOUS_WORD_PAIRS,
	
	TASK_VIRTUAL_PHRASES,
	TASK_PHRASES,
	TASK_NANA,
	TASK_CONTAINER,
	TASK_ACTIONLIST,
	TASK_ACTION_PARALLELS,
	TASK_ACTION_TRANSITIONS,
	TASK_WORDNET,
	TASK_ATTRIBUTES,
	
	TASK_COUNT
} TaskType;


struct Task {
	struct Batch : Moveable<Batch> {
		CompanyDataset* company;
		StoryDataset* story;
		String txt;
		int ds_i;
		bool program_begins;
	};
	
	
	struct AttrExtremesBatch : Moveable<AttrExtremesBatch> {
		String group;
	};
	Vector<AttrExtremesBatch> attr_extremes_batches;
	
	
	struct AttrPolarBatch : Moveable<AttrPolarBatch> {
		String group, attr0, attr1;
		Vector<String> attrs;
	};
	Vector<AttrPolarBatch> attr_polar_batches;
	
	
	struct AttrJoinBatch : Moveable<AttrJoinBatch> {
		Vector<String> groups, values;
	};
	Vector<AttrJoinBatch> attr_join_batches;
	
	
	void UpdateBatches(int per_batch);
	
	TaskType type;
	Callback cb;
	Callback on_ready;
	Callback2<int,int> update;
	int fn = -1;
	int ds_i = -1;
	int batch_i = 0;
	int part_i = -1;
	int line_i = -1;
	bool running = false;
	Vector<int> tmp;
	Vector<Batch> batches;
	Index<String> tmp_words, tmp_words2;
	VectorMap<String, Color> word_clr;
	Vector<void*> tmp_ptrs;
	String tmp_str;
	int actual = 0, total = 0;
	Program* program = 0;
	VectorMap<String,Index<String>> uniq_attrs;
	
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
	
	void ImportPromptAnswers();
	void Process();
	
	void GetPrograms(Task* t);
	
	void GetTokenData(Task* t);
	void OnTokenData(String result, Task* t);
	
	void GetAttributes(Task* t);
	void OnAttributes(String result, Task* t);
	void OnAttributePolars(String result, Task* t);
	void OnAttributeJoins(String result, Task* t);
	
	void GetUnknownTokenPairs(Task* t);
	void GetWordProcess(Task* t);
	void GetWordFix(Task* t);
	
	void GetActionlist(Task* t);
	void OnActionlistColors(String result, Task* t);
	void OnActionlistAttrs(String result, Task* t);
	
	void GetActionTransitions(Task* t);
	void GetActionParallels(Task* t);
	
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
	void OnPhraseRoles(String result, Task* t);
	void OnPhraseGeneric(String result, Task* t);
	
	//void GetNana(Task* tp);
	//void OnProgramStory(String result, Task* t);
	void OnSubPicked(String result, Task* t);
	void OnNanaFit(String result, Task* t);
	
	void MakeNana(Task* t);
	void GetRhymeContainers(Task* t);
	void GetRhymeContainersFromTemplates(Task* t);
	void GetContainer(Task* t);
	
	void GetLineActions(Task* t);
	void OnLineActions(String res, Task* t);
	
	void GetWordData(Task* t);
	void OnWordData(String res, Task* t);
	
	void GetSyllables(Task* t);
	void OnSyllables(String res, Task* t);
	
	void GetDetails(Task* t);
	void OnDetails(String res, Task* t);
	
	void GetLineChangeScores(Task* t);
	void OnLineChangeScores(String res, Task* t);
	
	void GetWordnet(Task* t);
	void GetColorAlternatives(Task* t);
	void OnColorAlternatives(String res, Task* t);
	void MakeWordnetsFromTemplates(Task* t);
	
	void RemoveTask(Task& t);
	
public:
	typedef TaskManager CLASSNAME;
	TaskManager();
	
	void Start();
	void Stop();
	void Clear();
	bool IsInTaskList(TaskType type) const;
	void RealizePipe();
	TaskMgr* GetPipe() const {return pipe;}
	TaskMgr& MakePipe() {if (!pipe) RealizePipe(); return *pipe;}
	
	static TaskManager& Single() {static TaskManager o; return o;}
	
	void DoPrograms(int ds_i, int fn);
	void DoTokens(int ds_i, int fn);
	void DoUnknownTokenPairs(int ds_i, int fn);
	void DoAmbiguousWordPairs(int ds_i, int fn);
	void DoVirtualPhrases(int ds_i, int fn);
	void DoPhrases(int ds_i, int fn);
	void DoNana(int ds_i, int fn, Program& program, Callback OnReady, int line_i=-1, int part_i=-1);
	void DoContainer(int ds_i, int fn, Callback2<int,int> update);
	void DoWords(int ds_i, int fn);
	void DoWordFix(int ds_i, int fn);
	
	void DoActionlistCache(int ds_i);
	void DoActionlist(int ds_i, int fn);
	
	void DoActionParallel(int ds_i, int fn);
	void DoActionTransition(int ds_i, int fn);
	void DoWordnet(int ds_i, int fn);
	void DoAttributes(int ds_i, int fn);
	
};

bool GetTypePhrase(Vector<int>& types, const DatasetAnalysis& da, int next_w_i, int w_i, int prev_w_i);


}

#endif
