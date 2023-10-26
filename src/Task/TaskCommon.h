#ifndef _Task_TaskCommon_h_
#define _Task_TaskCommon_h_

typedef enum : int {
	V_ATTR_SCORING,
	V_PTR_PIPE,
	V_PTR_LINE,
	//V_PTR_PIPE_UNIQUELINES,
	V_MODE,
	V_DIR,
	V_ARGS,
	V_SONG_LYRICS,
	V_SONG_PARTS,
	V_LINE_TXT,
	V_HUMAN_INPUT_LINE_TXT,
	V_ATTRS,
	V_TXT,
	V_MALE_FEMALE_MASK,
} TaskArgType;

typedef enum : int {
	O_NEVER,
	
	O_ORDER_IMPORT,
	O_ORDER_IMPORT_DETAILED,
	O_ORDER_REVERSE,
	
	O_DB_ATTRS,
	O_DB_ATTR_SCORES,
	
	O_TASKS,
	
	O_SONG_MASK,
	//O_SONG_MASK_WEIGHTED,
	O_SONG_ANALYSIS,
	O_SONG_DATA_STORYLINE,
	//O_SONG_DATA_STORYLINE_WEIGHTED,
	O_SONG_UNIQLINE_ATTRS,
	O_SONG_SNAP,
	//O_SONG_SNAP_WEIGHTED,
	O_SONG_REVERSED_MASK_COMMON,
	O_SONG_REVERSED_MASK,
	O_SONG_REVERSED_LYRICS,
	O_SONG_REVERSED_TRANSLATED_LYRICS,
	
	O_PART_MASK,
	//O_PART_MASK_WEIGHTED,
	O_PART_MASK_SCORE,
	O_PART_DATA_STORYLINE,
	//O_PART_DATA_STORYLINE_WEIGHTED,
	O_PART_SNAP,
	O_PART_SNAP_SCORE,
	//O_PART_SNAP_WEIGHTED,
	O_PART_REVERSED_SNAP,
	
	O_LINE_SNAP,
	O_LINE_SNAP_SCORE,
	//O_LINE_SNAP_WEIGHTED,
	O_LINE_REVERSED_SNAP,
	
	O_BREAK_SNAP,
	O_BREAK_SNAP_SCORE,
	//O_BREAK_SNAP_WEIGHTED,
	O_BREAK_IMPACT,
	O_BREAK_IMPACT_SCORES,
	O_BREAK_REVERSED_IMPACT,
	O_BREAK_REVERSED_SNAP,
	//O_BREAK_IMPACT_WEIGHTED,
	O_BREAK_LYRICS,
	//O_BREAK_LYRICS_WEIGHTED,
	
	O_NEXT_CTX_JUMP, // allows next context to start being processed
	
} TaskOutputType;

inline bool IsTaskSkippingContext(TaskOutputType tt) {
	switch (tt) {
		case O_ORDER_IMPORT:
		case O_ORDER_IMPORT_DETAILED:
		case O_ORDER_REVERSE:
		case O_NEXT_CTX_JUMP:
			return true;
		default:
			return false;
	}
}

typedef enum : int {
	TASK_TRANSLATE,
	TASK_TRANSLATE_SONG_DATA,
	TASK_UNPACK_STRUCTURE_SONG_DATA,
	TASK_CHECK_ERRORS_IN_SONG_STRUCT_DATA,
	TASK_CHECK_ERRORS_IN_SONG_NL_DATA,
	TASK_CHECK_ERRORS_IN_SCREENPLAY_STRUCT_DATA,
	TASK_CONVERT_SONG_STRUCTURE_TO_ENGLISH,
	TASK_EVALUATE_SONG_AUDIENCE,
	TASK_MAKE_SONG_POETIC,
	TASK_EVALUATE_POETIC_STYLES,
	TASK_MORPH_TO_ATTRIBUTES,
	TASK_CONVERT_SCREENPLAY_TO_STRUCTURE,
	TASK_CONVERT_STRUCTURE_TO_SCREENPLAY,
	TASK_CONVERT_SCREENPLAY_TO_PLAN,
	TASK_CREATE_IMAGE,
	TASK_EDIT_IMAGE,
	TASK_VARIATE_IMAGE,
	TASK_RAW_COMPLETION,
	TASK_EVALUATE_SUGGESTION_SCORES,
	TASK_EVALUATE_SUGGESTION_ORDER,
	TASK_IMPROVE_SOURCE_TEXT,
	TASK_LIMIT_SYLLABLE_COUNT,
	TASK_GET_AI_ATTRIBUTES,
	TASK_GET_STRUCTURE_SUGGESTIONS,
	TASK_GET_SUGGESTION_ATTRIBUTES,
	TASK_GET_NOVEL_THEMES,
	TASK_GET_NOVEL_IDEAS,
	TASK_GET_TONE_SUGGESTIONS,
	TASK_GET_CONTENT_SUGGESTIONS,
	TASK_GET_ALLEGORICAL_DEVICES,
	TASK_GET_IMAGERY_SUGGESTIONS,
	TASK_GET_SYMBOLISM_SUGGESTIONS,
	TASK_GET_IDEA_SUGGESTIONS,
	TASK_GET_PART_CONTENT_SUGGESTIONS,
	TASK_GET_PART_IMAGERY_SUGGESTIONS,
	TASK_GET_PART_SYMBOLISM_SUGGESTIONS,
	TASK_GET_PART_IDEA,
	TASK_GET_INTERNAL_RHYMING_FIRST_LINE,
	TASK_GET_INTERNAL_RHYMING_CONTINUE_LINE,
	TASK_GET_IDEA_FROM_LYRICS,
	TASK_GET_ATTRIBUTES_FROM_LYRICS,
	TASK_GET_PRODUCTION_IDEA,
	TASK_GET_STORY_CONTEXT,
	TASK_GET_PART_CONTEXT,
	TASK_GET_PART_VISUAL_IDEA_CONTEXT,
	TASK_GET_PART_VISUAL_IDEA_CHARACTERS,
	TASK_GET_PART_DIALOGUE_IDEA,
	TASK_GET_PART_DIALOGUE_IDEA_STYLE,
	TASK_GET_COLOR_IDEA,
	TASK_GET_VOCABULARY,
	TASK_GET_VOCABULARY_IDEA,
	
	TASK_IMPORT_AND_REVERSE,
	TASK_CONTEXT_IMPORT_AND_REVERSE,
	
	TASK_PATTERNMASK,
	TASK_PATTERNMASK_WEIGHTED,
	TASK_ANALYSIS,
	TASK_STORYARC,
	TASK_STORYARC_WEIGHTED,
	TASK_IMPACT,
	TASK_IMPACT_WEIGHTED,
	TASK_FORWARD_LYRICS_WEIGHTED,
	
	TASK_MAKE_IMPACT_SCORING_TASKS,
	TASK_IMPACT_SCORING,
	
	TASK_MAKE_PATTERN_TASKS,
	TASK_PATTERN,
	TASK_PATTERN_WEIGHTED,
	
	TASK_MAKE_ATTRSCORES_TASKS,
	TASK_ATTRSCORES,
	TASK_ATTRSCORES_READY,
	TASK_SONGSCORE,
	
	TASK_MAKE_REVERSE_IMPACT_TASK,
	TASK_REVERSE_IMPACT,
	
	TASK_MAKE_REVERSE_MASK_TASK,
	TASK_REVERSE_COMMON_MASK,
	TASK_REVERSE_SEPARATE_MASK,
	
	TASK_MAKE_REVERSEPATTERN_TASK,
	TASK_REVERSEPATTERN,
	
	TASK_MAKE_LYRICS_TASK,
	TASK_LYRICS,
	TASK_LYRICS_TRANSLATE,
	
	TASK_COUNT
} TaskType;

NAMESPACE_UPP
NTL_MOVEABLE(TaskArgType)
NTL_MOVEABLE(TaskOutputType)
NTL_MOVEABLE(TaskType)
END_UPP_NAMESPACE



struct IdeaArgs {
	// Song
	String song_idea[IDEAPATH_COUNT];
	VectorMap<String, Vector<String>> part_ideas;
	VectorMap<String,String> attrs;
	String part_name;
	
	
	void Jsonize(JsonIO& json) {
		for(int i = 0; i < IDEAPATH_COUNT; i++)
			json("song_idea[" + IntStr(i) + "]", song_idea[i]);
		json("part_ideas", part_ideas);
		json("attrs", attrs);
		json("part_name", part_name);
	}
	String Get() const {return StoreAsJson(*this);}
	void Put(const String& s) {LoadFromJson(*this, s);}
};

struct RhymingArgs {
	// Song
	String song_idea[IDEAPATH_COUNT];
	VectorMap<String, Vector<String>> known_part_ideas;
	Vector<String> best_previous_lines;
	
	// Part
	String part;
	String part_idea[IDEAPATH_PARTCOUNT];
	
	// Params
	int syllable_count;
	Vector<String> forbidden_words;
	String frozen_begin;
	String frozen_end;
	String rhyme_idea[IDEAPATH_PARTCOUNT];
	VectorMap<String, String> attrs;
	
	void Jsonize(JsonIO& json) {
		for(int i = 0; i < IDEAPATH_COUNT; i++)
			json("song_idea[" + IntStr(i) + "]", song_idea[i]);
		json("known_part_ideas", known_part_ideas);
		json("best_previous_lines", best_previous_lines);
		json("part", part);
		for(int i = 0; i < IDEAPATH_PARTCOUNT; i++)
			json("part_idea[" + IntStr(i) + "]", part_idea[i]);
		for(int i = 0; i < IDEAPATH_PARTCOUNT; i++)
			json("rhyme_idea[" + IntStr(i) + "]", rhyme_idea[i]);
		json	("syllable_count", syllable_count)
				("forbidden_words", forbidden_words)
				("frozen_begin", frozen_begin)
				("frozen_end", frozen_end)
				("attrs", attrs)
				;
	}
	String Get() const {return StoreAsJson(*this);}
	void Put(const String& s) {LoadFromJson(*this, s);}
	
};

struct ProductionArgs {
	struct Part {
		String name;
		String type;
		String chords;
		void Jsonize(JsonIO& json) {
			json	("name", name)
					("type", type)
					("chords", chords)
					;
		}
	};
	// Song
	Vector<String> attrs;
	Array<Part> parts;
	int bpm;
	
	void Jsonize(JsonIO& json) {
		json	("parts", parts)
				("attrs", attrs)
				("bpm", bpm)
				;
	}
	String Get() const {return StoreAsJson(*this);}
	void Put(const String& s) {LoadFromJson(*this, s);}
	
};

struct PoeticStylesArgs {
	String rhyme;
	String rhyme_scheme;
	int line_count;
	Vector<String> attrs;
	Vector<int> syllable_count;
	Vector<String> forbidden_words;
	Vector<String> frozen_begin;
	Vector<String> frozen_end;
	String rhyme_idea[IDEAPATH_PARTCOUNT];
	
	void Jsonize(JsonIO& json) {
		json	("rhyme", rhyme)
				("rhyme_scheme", rhyme_scheme)
				("line_count", line_count)
				("attrs", attrs)
				("syllable_count", syllable_count)
				("forbidden_words", forbidden_words)
				("frozen_begin", frozen_begin)
				("frozen_end", frozen_end)
				;
		for(int i = 0; i < IDEAPATH_PARTCOUNT; i++)
			json("rhyme_idea[" + IntStr(i) + "]", rhyme_idea[i]);
	}
	String Get() const {return StoreAsJson(*this);}
	void Put(const String& s) {LoadFromJson(*this, s);}
	
};

struct MorphArgs {
	Vector<String> source;
	VectorMap<String,String> attrs;
	String song_idea[IDEAPATH_COUNT];
	
	void Jsonize(JsonIO& json) {
		for(int i = 0; i < IDEAPATH_COUNT; i++)
			json("song_idea[" + IntStr(i) + "]", song_idea[i]);
		json	("source", source)
				("attrs", attrs)
				;
	}
	String Get() const {return StoreAsJson(*this);}
	void Put(const String& s) {LoadFromJson(*this, s);}
	
};

struct StoryContextArgs {
	String stories[STORY_COUNT];
	VectorMap<String,String> attrs;
	VectorMap<String, Vector<String>> part_ideas;
	int get_story_i = -1;
	String part_name;
	
	void Jsonize(JsonIO& json) {
		for(int i = 0; i < STORY_COUNT; i++)
			json("stories[" + IntStr(i) + "]", stories[i]);
		json	("part_ideas", part_ideas)
				("attrs", attrs)
				("get_story_i", get_story_i)
				("part_name", part_name)
				;
	}
	String Get() const {return StoreAsJson(*this);}
	void Put(const String& s) {LoadFromJson(*this, s);}
	
};

struct VisualContextArgs {
	Vector<String> phases;
	int phase_count = 0;
	String part_name;
	String style;
	VectorMap<String,String> attrs;
	Vector<String> characters;
	
	void Jsonize(JsonIO& json) {
		json	("phases", phases)
				("phase_count", phase_count)
				("part_name", part_name)
				("attrs", attrs)
				("characters", characters)
				("style", style)
				;
	}
	String Get() const {return StoreAsJson(*this);}
	void Put(const String& s) {LoadFromJson(*this, s);}
	
};

static const int DIALOGUE_COUNT = 2;

struct ColorIdeaArgs {
	int fn;
	VectorMap<String, Vector<String>> dialogue;
	Vector<Vector<Color>> prev_line;
	Vector<Vector<Color>> next_line;
	Vector<Vector<Color>> main;
	VectorMap<String,Color> begin_colors;
	
	void Jsonize(JsonIO& json) {
		json	("fn", fn)
				("dialogue", dialogue)
				("prev_line", prev_line)
				("next_line", next_line)
				("main", main)
				("begin_colors", begin_colors)
				;
	}
	String Get() const {return StoreAsJson(*this);}
	void Put(const String& s) {LoadFromJson(*this, s);}
	
};


struct VocabularyArgs {
	int fn;
	String phrase;
	
	void Jsonize(JsonIO& json) {
		json	("fn", fn)
				("phrase", phrase)
				;
	}
	String Get() const {return StoreAsJson(*this);}
	void Put(const String& s) {LoadFromJson(*this, s);}
	
};


struct VocabularyIdeaArgs {
	int fn;
	Vector<String> visual, characters, dialogue1, dialogue2, color;
	Vector<Vector<String>> phrases;
	Vector<Color> colors;
	
	void Jsonize(JsonIO& json) {
		json	("fn", fn)
				("visual", visual)
				("characters", characters)
				("dialogue1", dialogue1)
				("dialogue2", dialogue2)
				("color", color)
				("phrases", phrases)
				("colors", colors)
				;
	}
	String Get() const {return StoreAsJson(*this);}
	void Put(const String& s) {LoadFromJson(*this, s);}
	
};



#endif
