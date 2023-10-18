#ifndef _Task_Tasks_h_
#define _Task_Tasks_h_


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

struct DalleResponse {
	struct Data : Moveable<Data> {
		String b64_json;
		
		void Jsonize(JsonIO& json) {
			json("b64_json", b64_json);
		}
		String ToString() const {
			String s;
			s	<< "b64_json: " << b64_json << "\n";
			return s;
		}
	};
	
	int64 created = 0;
	Vector<Data> data;
	
	void Jsonize(JsonIO& json) {
		json
			("created", created)
			("data", data)
			;
	}
	String ToString() const {
		String s;
		s << "created: " << created << "\n";
		s << data.ToString();
		return s;
	}
};

struct TaskRule;

struct Task {
	
protected:
	friend struct TaskMgr;
	int created_task_count = 0;
	int id = 0;
	mutable hash_t order_hash = 0;
	
public:

	const TaskRule* rule = 0;
	
	Vector<String> args;
	String output;
	String error;
	bool skip_load = false;
	bool ready = false;
	bool fast_exit = false;
	bool failed = false;
	bool fatal_error = false;
	bool processing = false;
	bool changed = false;
	bool whole_song = false;
	bool wait_task = false;
	bool allow_multi_spawn = false;
	bool is_waiting_deps = false;
	int tries = 0;
	
	PipePtrs p;
	PatternSnap* snap = 0;
	ReverseTask* task = 0;
	SnapContext* ctx = 0;
	
	TaskContent input;
	String raw_input;
	
	// Temp
	Array<Task> result_tasks;
	Vector<Vector<String>> str_map;
	Task* created_by = 0;
	Event<> WhenDone;
	Event<String> WhenResult;
	Event<Array<Image>&> WhenResultImages;
	Event<> WhenError;
	String image_n, image_sz;
	Array<Image> send_images, recv_images;
	
	inline static constexpr int common_mask_gen_multiplier		= 8;
	inline static constexpr int common_mask_max_values			= 10;
	inline static constexpr int common_mask_gens				= 200;
	inline static constexpr int separate_mask_gen_multiplier	= 8;
	inline static constexpr int separate_mask_max_values		= 50;
	inline static constexpr int separate_mask_gens				= 100;
	inline static constexpr int snap_gen_multiplier			= 20;
	inline static constexpr int snap_max_values				= 10;
	inline static constexpr int snap_max_per_mode				= snap_max_values / 3;
	inline static constexpr int snap_gens						= 100;
	
	Task& ResultTask(int r);
	
	void Store(bool force=false);
	void Load();
	bool RunOpenAI();
	bool RunOpenAI_Completion();
	bool RunOpenAI_Image();
	bool ProcessInput();
	void Process();
	void SetError(String s);
	void SetFatalError(String s) {SetError(s); fatal_error = true;}
	void SetWaiting() {wait_task = true;}
	void SetFastExit() {fast_exit = true;}
	String GetInputHash() const;
	String GetOutputHash() const;
	bool HasCreatedTasks(const TaskMgr& m, GroupContext ctx) const;
	bool IsCreatedTasksReady(const TaskMgr& m, GroupContext ctx) const;
	bool CheckArguments();
	bool WriteResults();
	bool ParseOriginalLyrics();
	
	void CreateInput_StoryArc();
	void CreateInput_StoryArcWeighted();
	void CreateInput_PatternMask();
	void CreateInput_PatternMaskWeighted();
	void CreateInput_Pattern();
	void CreateInput_PatternWeighted();
	void CreateInput_Analysis();
	void CreateInput_AttrScores();
	void CreateInput_Lyrics();
	void CreateInput_LyricsTranslate();
	void CreateInput_Impact();
	void CreateInput_ImpactWeighted();
	void CreateInput_ImpactScoring();
	void CreateInput_ForwardLyricsWeighted();
	void CreateInput_Translate();
	void CreateInput_TranslateSongData();
	void CreateInput_UnpackStructureSongData();
	void CreateInput_CheckSongStructureErrors();
	void CreateInput_CheckSongNaturalErrors();
	void CreateInput_ConvertSongStructureToEnglish();
	void CreateInput_EvaluateSongAudience();
	void CreateInput_MakePoetic();
	void CreateInput_EvaluatePoeticStyles();
	void CreateInput_ConvertScreenplayToStructure();
	void CreateInput_ConvertStructureToScreenplay();
	void CreateInput_CheckScreenplayStructureErrors();
	void CreateInput_ConvertScreenplayToPlan();
	void CreateInput_CreateImage();
	void CreateInput_EditImage();
	void CreateInput_VariateImage();
	void CreateInput_RawCompletion();
	void CreateInput_EvaluateSuggestionScores();
	void CreateInput_EvaluateSuggestionOrder();
	void CreateInput_ImproveSourceText();
	void CreateInput_LimitSyllableCount();
	void CreateInput_GetAIAttributes();
	void CreateInput_MorphToAttributes();
	void CreateInput_GetStructureSuggestions();
	void CreateInput_GetSuggestionAttributes();
	void CreateInput_GetNovelThemes();
	void CreateInput_GetNovelIdeas();
	void CreateInput_GetContentSuggestions();
	void CreateInput_GetAllegorySuggestions();
	void CreateInput_GetToneSuggestions();
	void CreateInput_GetImagerySuggestions();
	void CreateInput_GetSymbolismSuggestions();
	void CreateInput_GetPartContentSuggestions();
	void CreateInput_GetPartImagerySuggestions();
	void CreateInput_GetPartSymbolismSuggestions();
	void CreateInput_GetInternalRhymingFirstLine();
	void CreateInput_GetInternalRhymingContinueLine();
	void CreateInput_GetIdeaFromLyrics();
	void CreateInput_GetAttributesFromLyrics();
	void CreateInput_GetProductionIdea();
	void CreateInput_GetIdeaSuggestions();
	void CreateInput_GetPartIdea();
	void CreateInput_GetStoryContext();
	void CreateInput_GetPartContext();
	void CreateInput_GetPartVisualIdeaContext();
	void CreateInput_GetPartVisualIdeaCharacters();
	void CreateInput_GetPartDialogueIdeaContext();
	void CreateInput_GetPartDialogueIdeaStyleSuggestions();
	
	void Process_MakeImportTasks();
	void Process_MakeContextImportTasks();
	void Process_StoryArc();
	void Process_StoryArcWeighted();
	void Process_PatternMask();
	void Process_PatternMaskWeighted();
	void Process_Pattern();
	void Process_PatternWeighted();
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
	void Process_ImpactWeighted();
	void Process_ImpactScoring();
	void Process_MakeImpactScoringTasks();
	void Process_ForwardLyricsWeighted();
	void Process_Translate();
	void Process_TranslateSongData();
	void Process_UnpackStructureSongData();
	void Process_CheckSongStructureErrors();
	void Process_CheckSongNaturalErrors();
	void Process_ConvertSongStructureToEnglish();
	void Process_EvaluateSongAudience();
	void Process_MakePoetic();
	void Process_EvaluatePoeticStyles();
	void Process_ConvertScreenplayToStructure();
	void Process_ConvertStructureToScreenplay();
	void Process_CheckScreenplayStructureErrors();
	void Process_ConvertScreenplayToPlan();
	void Process_CreateImage();
	void Process_EditImage();
	void Process_VariateImage();
	void Process_RawCompletion();
	void Process_EvaluateSuggestionScores();
	void Process_EvaluateSuggestionOrder();
	void Process_ImproveSourceText();
	void Process_LimitSyllableCount();
	void Process_GetAIAttributes();
	void Process_MorphToAttributes();
	void Process_GetStructureSuggestions();
	void Process_GetSuggestionAttributes();
	void Process_GetNovelThemes();
	void Process_GetNovelIdeas();
	void Process_GetContentSuggestions();
	void Process_GetAllegorySuggestions();
	void Process_GetToneSuggestions();
	void Process_GetImagerySuggestions();
	void Process_GetSymbolismSuggestions();
	void Process_GetPartContentSuggestions();
	void Process_GetPartImagerySuggestions();
	void Process_GetPartSymbolismSuggestions();
	void Process_GetInternalRhymingFirstLine();
	void Process_GetInternalRhymingContinueLine();
	void Process_GetIdeaFromLyrics();
	void Process_GetAttributesFromLyrics();
	void Process_GetProductionIdea();
	void Process_GetIdeaSuggestions();
	void Process_GetPartIdea();
	void Process_GetStoryContext();
	void Process_GetPartContext();
	void Process_GetPartVisualIdeaContext();
	void Process_GetPartVisualIdeaCharacters();
	void Process_GetPartDialogueIdeaContext();
	void Process_GetPartDialogueIdeaStyleSuggestions();
	
	void Retry(bool skip_prompt);
	String GetDescription() const;
	String GetTaskDependencyString(bool have_ready_rules, bool rule_names) const;
	String GetTaskDepsWaitingString() const;
	String GetTypeString() const;
	bool AddAttrScoreEntry(AttrScoreGroup& ag, String group, String entry_str);
	void AddAttrScoreId(AttrScoreGroup& ag, const SnapAttrStr& a);
	hash_t GetOrderHash() const;
	String GetInfoInline() const;
	
	TaskMgr& GetTaskMgr();
	Pipe& GetPipe();
	void GetScores(const PatternSnap& snap, Vector<int>& scores);
	void GetMaskScores(const PatternSnap& snap, Vector<int>& scores);
};



#endif
