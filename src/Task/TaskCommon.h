#ifndef _Task_TaskCommon_h_
#define _Task_TaskCommon_h_


BEGIN_TEXTLIB_NAMESPACE


typedef enum : int {
	V_ATTR_SCORING,
	V_ARGS
} TaskArgType;

typedef enum : int {
	O_NEVER,
	
	O_ORDER_IMPORT,
	O_ORDER_IMPORT_DETAILED,
	O_ORDER_REVERSE,
	
	
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
	AITASK_TRANSLATE,
	AITASK_TRANSLATE_SONG_DATA,
	AITASK_CREATE_IMAGE,
	AITASK_EDIT_IMAGE,
	AITASK_VARIATE_IMAGE,
	
	AITASK_GET_SUGGESTION_ATTRIBUTES,
	AITASK_RAW_COMPLETION,
	AITASK_GET_STRUCTURE_SUGGESTIONS,
	AITASK_GET_LYRICS_PHRASE,
	AITASK_GET_SONG_DATA_ANALYSIS,
	AITASK_GET_ACTION_ANALYSIS,
	
	AITASK_GET_TOKEN_DATA,
	AITASK_GET_PHRASE_DATA,
	AITASK_GET_ATTRIBUTES,
	AITASK_NANA_DATA,
	AITASK_LYRICS_SOLVER,
	AITASK_STORY_SOLVER,
	AITASK_GET_PROGRAM_DATA_ANALYSIS,
	
	AITASK_COUNT
} AiTaskType;


END_TEXTLIB_NAMESPACE

NAMESPACE_UPP
using namespace TextLib;
NTL_MOVEABLE(TaskArgType)
NTL_MOVEABLE(TaskOutputType)
NTL_MOVEABLE(AiTaskType)
END_UPP_NAMESPACE

BEGIN_TEXTLIB_NAMESPACE


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


struct WordSaladIdeaArgs {
	int fn;
	Vector<Vector<String>> vocabulary;
	Vector<Vector<Color>> colors;
	Vector<Vector<Color>> listener_colors_in_begin;
	Vector<Color> listener_colors;
	Vector<String> visual, characters, dialogue1, dialogue2;
	
	void Jsonize(JsonIO& json) {
		json	("fn", fn)
				("vocabulary", vocabulary)
				("colors", colors)
				("listener_colors_in_begin", listener_colors_in_begin)
				("listener_colors", listener_colors)
				("visual", visual)
				("characters", characters)
				("dialogue1", dialogue1)
				("dialogue2", dialogue2)
				;
	}
	String Get() const {return StoreAsJson(*this);}
	void Put(const String& s) {LoadFromJson(*this, s);}
	
};

struct ContextIdeaArgs {
	struct Part : Moveable<Part> {
		int duration = 0;
		int total_notes = 0;
		int est_words = 0;
		String title;
		
		void Jsonize(JsonIO& json) {
			json	("duration", duration)
					("total_notes", total_notes)
					("est_words", est_words)
					("title", title)
					;
		}
	};
	int fn;
	int bpm = 0;
	Vector<Part> parts;
	VectorMap<String,double> roles;
	
	void Jsonize(JsonIO& json) {
		json	("fn", fn)
				("bpm", bpm)
				("parts", parts)
				("roles", roles)
				;
	}
	String Get() const {return StoreAsJson(*this);}
	void Put(const String& s) {LoadFromJson(*this, s);}
	
};

struct TextDataAnalysisArgs {
	int fn;
	int score_mode;
	String artist, song, text;
	Vector<String> words;
	Vector<String> phrases;
	
	void Jsonize(JsonIO& json) {
		json	("fn", fn)
				("sm", score_mode)
				("artist", artist)
				("song", song)
				("text", text)
				("words", words)
				("phrases", phrases)
				;
	}
	String Get() const {return StoreAsJson(*this);}
	void Put(const String& s) {LoadFromJson(*this, s);}
	
};


struct ActionAnalysisArgs {
	int fn;
	Vector<String> actions;
	
	void Jsonize(JsonIO& json) {
		json	("fn", fn)
				("actions", actions)
				;
	}
	String Get() const {return StoreAsJson(*this);}
	void Put(const String& s) {LoadFromJson(*this, s);}
	
};



struct ScriptPhraseArgs {
	int fn;
	int len;
	Vector<String> tmpls, args, attr_groups, attr_values;
	Vector<ActionHeader> actions;
	Vector<Color> colors;
	Index<int> color_groups;
	Index<String> words;
	
	void Jsonize(JsonIO& json) {
		json	("fn", fn)
				("actions", actions)
				("args", args)
				("words", words)
				("tmpls", tmpls)
				("attr_groups", attr_groups)
				("attr_values", attr_values)
				("colors", colors)
				("color_groups", color_groups)
				("len", len)
				;
	}
	String Get() const {return StoreAsJson(*this);}
	void Put(const String& s) {LoadFromJson(*this, s);}
	
};

struct TokenArgs {
	int fn;
	Vector<String> words;
	
	void Jsonize(JsonIO& json) {
		json	("fn", fn)
				("words", words)
				;
	}
	String Get() const {return StoreAsJson(*this);}
	void Put(const String& s) {LoadFromJson(*this, s);}
	
};

struct PhraseArgs {
	int fn;
	Vector<String> phrases;
	
	void Jsonize(JsonIO& json) {
		json	("fn", fn)
				("phrases", phrases)
				;
	}
	String Get() const {return StoreAsJson(*this);}
	void Put(const String& s) {LoadFromJson(*this, s);}
	
};

struct AttrArgs {
	int fn;
	String group;
	Vector<String> groups, values;
	String attr0, attr1;
	
	void Jsonize(JsonIO& json) {
		json	("fn", fn)
				("group", group)
				("groups", groups)
				("values", values)
				("attr0", attr0)
				("attr1", attr1)
				;
	}
	String Get() const {return StoreAsJson(*this);}
	void Put(const String& s) {LoadFromJson(*this, s);}
	
};

struct NanaArgs {
	int fn;
	Vector<String> phrases, parts;
	Vector<int> counts;
	String part, nana, phrase, pre_text;
	WString pron;
	
	void Jsonize(JsonIO& json) {
		json	("fn", fn)
				("parts", parts)
				("phrases", phrases)
				("counts", counts)
				("part", part)
				("nana", nana)
				("phrase", phrase)
				("pre_text", pre_text)
				("pron", pron)
				;
	}
	String Get() const {return StoreAsJson(*this);}
	void Put(const String& s) {LoadFromJson(*this, s);}
	
};

struct ScriptSolverArgs {
	int fn;
	VectorMap<String,String> artist, release, song;
	Vector<String> parts, attrs, phrases;
	Vector<int> counts, offsets;
	String part;
	
	void Jsonize(JsonIO& json) {
		json	("fn", fn)
				("artist", artist)
				("release", release)
				("song", song)
				("parts", parts)
				("attrs", attrs)
				("phrases", phrases)
				("counts", counts)
				("offsets", offsets)
				("part", part)
				;
	}
	String Get() const {return StoreAsJson(*this);}
	void Put(const String& s) {LoadFromJson(*this, s);}
	
};

struct StorySolverArgs {
	int fn;
	VectorMap<String,String> company, campaign, program;
	Vector<String> parts, attrs, phrases;
	Vector<int> counts, offsets;
	String part;
	
	void Jsonize(JsonIO& json) {
		json	("fn", fn)
				("company", company)
				("campaign", campaign)
				("program", program)
				("parts", parts)
				("attrs", attrs)
				("phrases", phrases)
				("counts", counts)
				("offsets", offsets)
				("part", part)
				;
	}
	String Get() const {return StoreAsJson(*this);}
	void Put(const String& s) {LoadFromJson(*this, s);}
	
};

struct ProgramDataAnalysisArgs {
	int fn;
	int score_mode;
	String company, program, text;
	Vector<String> words;
	Vector<String> phrases;
	
	void Jsonize(JsonIO& json) {
		json	("fn", fn)
				("sm", score_mode)
				("company", company)
				("program", program)
				("text", text)
				("words", words)
				("phrases", phrases)
				;
	}
	String Get() const {return StoreAsJson(*this);}
	void Put(const String& s) {LoadFromJson(*this, s);}
	
};




END_TEXTLIB_NAMESPACE


#endif
