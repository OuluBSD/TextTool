#ifndef _Task_Common_h_
#define _Task_Common_h_


BEGIN_TEXTLIB_NAMESPACE


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
	int lng_i = -1;
	VectorMap<String,String> artist, release, song;
	Vector<String> parts, attrs, phrases;
	Vector<int> counts, offsets;
	String part, vision;
	
	void Jsonize(JsonIO& json) {
		json	("fn", fn)
				("lng_i", lng_i)
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
