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
	O_SONG_ANALYSIS,
	O_SONG_DATA_STORYLINE,
	O_SONG_UNIQLINE_ATTRS,
	O_SONG_SNAP,
	O_SONG_REVERSED_MASK_COMMON,
	O_SONG_REVERSED_MASK,
	O_SONG_REVERSED_LYRICS,
	O_SONG_REVERSED_TRANSLATED_LYRICS,
	
	O_PART_MASK,
	O_PART_MASK_SCORE,
	O_PART_DATA_STORYLINE,
	O_PART_SNAP,
	O_PART_SNAP_SCORE,
	O_PART_REVERSED_SNAP,
	
	O_LINE_SNAP,
	O_LINE_SNAP_SCORE,
	O_LINE_REVERSED_SNAP,
	
	O_BREAK_SNAP,
	O_BREAK_SNAP_SCORE,
	O_BREAK_IMPACT,
	O_BREAK_IMPACT_SCORES,
	O_BREAK_REVERSED_IMPACT,
	O_BREAK_REVERSED_SNAP,
	O_BREAK_LYRICS,
	
	O_NEXT_CTX_JUMP, // allows next context to start being processed
	
} TaskOutputType;

typedef enum : int {
	AITASK_TRANSLATE,
	AITASK_TRANSLATE_SONG_DATA,
	AITASK_CREATE_IMAGE,
	AITASK_EDIT_IMAGE,
	AITASK_VARIATE_IMAGE,
	
	AITASK_GET_SUGGESTION_ATTRIBUTES,
	AITASK_RAW_COMPLETION,
	AITASK_GET_STRUCTURE_SUGGESTIONS,
	AITASK_GET_ACTION_ANALYSIS,
	AITASK_GET_SONG_DATA_ANALYSIS,
	
	AITASK_GET_TOKEN_DATA,
	AITASK_GET_PHRASE_DATA,
	AITASK_GET_ATTRIBUTES,
	AITASK_LYRICS_SOLVER,
	AITASK_STORY_SOLVER,
	
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


struct SourceDataAnalysisArgs {
	int fn;
	String artist, song, text;
	Vector<String> words;
	Vector<String> phrases;
	
	void Jsonize(JsonIO& json) {
		json	("fn", fn)
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

struct ScriptSolverArgs {
	int fn;
	VectorMap<String,String> artist, release, song;
	Vector<String> parts, attrs, phrases;
	Vector<int> counts, offsets;
	String part, vision;
	
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
				("vision", vision)
				;
	}
	String Get() const {return StoreAsJson(*this);}
	void Put(const String& s) {LoadFromJson(*this, s);}
	
};

struct StructureArgs {
	int fn = 0;
	String req;
	String avoid;
	String desc;
	int total;
	Vector<String> structs;
	
	void Jsonize(JsonIO& json) {
		json	("req", req)
				("fn", fn)
				("avoid", avoid)
				("desc", desc)
				("total", total)
				("structs", structs)
				;
	}
	String Get() const {return StoreAsJson(*this);}
	void Put(const String& s) {LoadFromJson(*this, s);}
	
};


END_TEXTLIB_NAMESPACE


#endif
