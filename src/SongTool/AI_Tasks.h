#ifndef _SongTool_AI_Tasks_h_
#define _SongTool_AI_Tasks_h_


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

struct AI_Task {
	enum {
		TASK_PATTERNMASK,
		TASK_ANALYSIS,
		TASK_STORYARC,
		TASK_IMPACT,
		
		TASK_MAKE_PATTERN_TASKS,
		TASK_IMPACT_SCORING,
		TASK_MAKE_IMPACT_SCORING_TASKS,
		TASK_PATTERN,
		
		TASK_MAKE_ATTRSCORES_TASKS,
		TASK_ATTRSCORES,
		TASK_SONGSCORE,
		
		TASK_MAKE_REVERSE_IMPACT_TASK,
		TASK_REVERSE_IMPACT,
		
		TASK_MAKE_REVERSE_MASK_TASK,
		TASK_REVERSE_MASK,
		
		TASK_MAKE_REVERSEPATTERN_TASK,
		TASK_REVERSEPATTERN,
		
		TASK_MAKE_LYRICS_TASK,
		TASK_LYRICS,
		TASK_LYRICS_TRANSLATE,
		
		TASK_COUNT
	};
	
	Vector<String> args;
	String input;
	String output;
	int type = -1;
	int response_length = 0;
	String error;
	bool skip_load = false;
	bool ready = false;
	bool failed = false;
	bool processing = false;
	bool changed = false;
	bool whole_song = false;
	bool wait_task = false;
	hash_t hash = 0;
	int tries = 0;
	
	Vector<AI_Task*> depends_on;
	Ptrs p;
	ReverseTask* task = 0;
	TaskMgr* mgr = 0;
	
	static constexpr int gen_multiplier		= 20;
	static constexpr int gens				= 200;
	static constexpr int max_snap_values	= 20;
	static constexpr int max_mask_values	= 50;
	
	void Store();
	void Load();
	bool RunOpenAI();
	void Process();
	void SetError(String s);
	String GetInputHash() const;
	String GetOutputHash() const;
	void CreateInput();
	void CreateInput_StoryArc();
	void CreateInput_PatternMask();
	void CreateInput_Pattern();
	void CreateInput_Analysis();
	void CreateInput_AttrScores();
	void CreateInput_Lyrics();
	void CreateInput_LyricsTranslate();
	void CreateInput_Impact();
	void CreateInput_ImpactScoring();
	void CreateInput_MakeImpactScoringTasks();
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
	void Process_ReverseMask();
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
	String GetTypeString() const;
	bool IsDepsReady(Index<AI_Task*>& seen) const;
	bool AddAttrScoreEntry(AttrScoreGroup& ag, String group, String entry_str);
	void AddAttrScoreId(AttrScoreGroup& ag, const SnapAttr& a);
	
};

void GetScores(const PatternSnap& snap, Vector<int>& scores);
void GetMaskScores(const PatternSnap& snap, Vector<int>& scores);


#endif
