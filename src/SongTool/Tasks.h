#ifndef _SongTool_Tasks_h_
#define _SongTool_Tasks_h_


struct TaskMgr;

struct OpenAiResponse {
	struct Choice : Moveable<Choice> {
		String text;
		String finish_reason;
		int index;
		Vector<double> logprobs;
		
		void Jsonize(JsonIO& json) {
			json("text", text)
				("finish_reason", finish_reason)
				("logprobs", logprobs)
				("index", index);
		}
		String ToString() const {
			String s;
			s	<< "text: " << text << "\n"
				<< "finish_reason: " << finish_reason << "\n"
				<< "logprobs: ";
			for (double d : logprobs)
				s << d <<", ";
			s	<< "\nindex: " << index << "\n";
			return s;
		}
	};
	struct Usage {
		int completion_tokens;
		int prompt_tokens;
		int total_tokens;
		void Jsonize(JsonIO& json) {
			json("completion_tokens", completion_tokens)
				("prompt_tokens", prompt_tokens)
				("total_tokens", total_tokens);
		}
		String ToString() const {
			String s;
			s	<< "completion_tokens: " << completion_tokens << "\n"
				<< "prompt_tokens: " << prompt_tokens << "\n"
				<< "total_tokens: " << total_tokens << "\n";
			return s;
		}
	};
	Vector<Choice> choices;
	String id;
	String model;
	String object;
	Usage usage;
	
	void Jsonize(JsonIO& json) {
		json
			("choices", choices)
			("id", id)
			("model", model)
			("object", object)
			("usage", usage)
			;
	}
	String ToString() const {
		String s;
		for(auto& c : choices)
			s << c.ToString();
		s	<< "id: " << id << "\n"
			<< "model: " << model << "\n"
			<< "object: " << object << "\n"
			<< usage.ToString() << "\n";
		return s;
	}
};

struct TaskRule;

struct Task {
	TaskRule* rule = 0;
	Vector<String> args;
	String input;
	String output;
	//int type = -1;
	int response_length = 0;
	String error;
	bool skip_load = false;
	bool ready = false;
	bool fast_exit = false;
	bool failed = false;
	bool processing = false;
	bool changed = false;
	bool whole_song = false;
	bool wait_task = false;
	bool allow_multi_spawn = false;
	bool is_waiting_deps = false;
	hash_t hash = 0;
	int tries = 0;
	
	//Vector<Task*> depends_on;
	Ptrs p;
	ReverseTask* task = 0;
	TaskMgr* mgr = 0;
	
	// temp
	Array<Task> result_tasks;
	Vector<Vector<String>> str_map;
	Task* created_by = 0;
	
	static constexpr int common_mask_gen_multiplier		= 8;
	static constexpr int common_mask_max_values			= 10;
	static constexpr int common_mask_gens				= 200;
	static constexpr int separate_mask_gen_multiplier	= 8;
	static constexpr int separate_mask_max_values		= 50;
	static constexpr int separate_mask_gens				= 100;
	static constexpr int snap_gen_multiplier			= 20;
	static constexpr int snap_max_values				= 10;
	static constexpr int snap_max_per_mode				= snap_max_values / 3;
	static constexpr int snap_gens						= 100;
	
	Task& ResultTask(int r);
	
	void Store(bool force=false);
	void Load();
	bool RunOpenAI();
	bool ProcessInput();
	void Process();
	void SetError(String s);
	void SetWaiting() {wait_task = true;}
	void SetFastExit() {fast_exit = true;}
	String GetInputHash() const;
	String GetOutputHash() const;
	bool HasCreatedTasks() const;
	bool IsCreatedTasksReady() const;
	bool CheckArguments();
	bool WriteResults();
	bool ParseOriginalLyrics();
	void CreateInput_StoryArc();
	void CreateInput_PatternMask();
	void CreateInput_Pattern();
	void CreateInput_Analysis();
	void CreateInput_AttrScores();
	void CreateInput_Lyrics();
	void CreateInput_LyricsTranslate();
	void CreateInput_Impact();
	void CreateInput_ImpactScoring();
	void Process_MakeImportTasks();
	void Process_StoryArc();
	void Process_PatternMask();
	void Process_Pattern();
	void Process_Analysis();
	void Process_MakePatternTasks();
	void Process_MakeAttrScores();
	void Process_AttrScores();
	void Process_SongScores();
	void Process_MakeReverseImpactTask();
	void Process_ReverseImpact();
	void Process_MakeReverseMaskTask();
	void Process_ReverseCommonMask();
	void Process_ReverseSeparateMask();
	void Process_MakeReversePattern();
	void Process_MakeLyricsTask();
	void Process_ReversePattern();
	void Process_Lyrics();
	void Process_LyricsTranslate();
	void Process_Impact();
	void Process_ImpactScoring();
	void Process_MakeImpactScoringTasks();
	
	void Retry();
	String GetDescription() const;
	String GetTaskDependencyString(bool have_ready_rules, bool rule_names) const;
	String GetTaskDepsWaitingString() const;
	String GetTypeString() const;
	bool AddAttrScoreEntry(AttrScoreGroup& ag, String group, String entry_str);
	void AddAttrScoreId(AttrScoreGroup& ag, const SnapAttr& a);
	
};

void GetScores(const PatternSnap& snap, Vector<int>& scores);
void GetMaskScores(const PatternSnap& snap, Vector<int>& scores);


#endif
