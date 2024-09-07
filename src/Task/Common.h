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
	AITASK_LEAD_SOLVER,
	AITASK_VISION,
	AITASK_SNAPSHOT,
	AITASK_SCRIPT_TEXT_PROCESS,
	AITASK_BIOGRAPHY_PROCESS,
	AITASK_BIOGRAPHY_SUMMARY_PROCESS,
	AITASK_CONCECPTUAL_FRAMEWORK_PROCESS,
	AITASK_MARKETPLACE,
	AITASK_BIOGRAPHY_GENERATOR,
	AITASK_CODE,
	
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
	Vector<String> elements;
	
	void Jsonize(JsonIO& json) {
		json	("fn", fn)
				("phrases", phrases)
				("elements", elements)
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
	int sub_fn = -1;
	int lng_i = -1;
	VectorMap<String,String> artist, release, song;
	Vector<String> parts, attrs, phrases, scores, phrases2;
	Vector<int> counts, offsets;
	String part, vision, ref;
	bool is_story = false;
	bool is_unsafe = false;
	bool is_self_centered = false;
	bool ret_fail = false;
	double factor = 0;
	Vector<String> elements;
	String rhyme_element;
	String previously;
	String peek;
	
	
	struct State {
		String element;
		AttrHeader attr;
		int clr_i = -1;
		ActionHeader act;
		String typeclass;
		String content, content_mod;
		String			style_type;
		String			style_entity;
		int				safety = 0;
		int				line_len = 0;
		int				connector = 0;
		String			line_begin;
		
		void Jsonize(JsonIO& json) {
			json	("element", element)
					("attr", attr)
					("clr_i", clr_i)
					("act", act)
					("typeclass", typeclass)
					("content", content)
					("content_mod", content_mod)
					("style_type", style_type)
					("style_entity", style_entity)
					("safety", safety)
					("line_len", line_len)
					("connector", connector)
					("line_begin", line_begin)
					;
		}
	};
	
	State state;
	Array<State> line_states;

	
	void Jsonize(JsonIO& json) {
		json	("fn", fn)
				("sub_fn", sub_fn)
				("lng_i", lng_i)
				("artist", artist)
				("release", release)
				("song", song)
				("parts", parts)
				("attrs", attrs)
				("phrases", phrases)
				("phrases2", phrases2)
				("scores", scores)
				("counts", counts)
				("offsets", offsets)
				("part", part)
				("vision", vision)
				("ref", ref)
				("is_story", is_story)
				("is_unsafe", is_unsafe)
				("is_self_centered", is_self_centered)
				("ret_fail", ret_fail)
				("factor", factor)
				("elements", elements)
				("rhyme_element", rhyme_element)
				("state", state)
				("line_states", line_states)
				("previously", previously)
				("peek", peek)
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

struct LeadSolverArgs {
	int fn = 0;
	int opp_i = 0;
	
	void Jsonize(JsonIO& json) {
		json	("opp_i", opp_i)
				("fn", fn)
				;
	}
	String Get() const {return StoreAsJson(*this);}
	void Put(const String& s) {LoadFromJson(*this, s);}
	
};

struct SongHeaderArgs {
	int fn = 0;
	int tc_i = -1;
	int con_i = -1;
	String lyrics_idea;
	String music_style;
	
	void Jsonize(JsonIO& json) {
		json	("tc_i", tc_i)
				("con_i", con_i)
				("lyrics_idea", lyrics_idea)
				("music_style", music_style)
				("fn", fn)
				;
	}
	String Get() const {return StoreAsJson(*this);}
	void Put(const String& s) {LoadFromJson(*this, s);}
	
};

struct SocialArgs {
	int fn = 0;
	String text, description, profile, photo_description;
	VectorMap<String,String> parts;
	int len = 0;
	
	void Jsonize(JsonIO& json) {
		json	("text", text)
				("desc", description)
				("fn", fn)
				("parts", parts)
				("len", len)
				("profile", profile)
				("photo_description", photo_description)
				;
	}
	String Get() const {return StoreAsJson(*this);}
	void Put(const String& s) {LoadFromJson(*this, s);}
	
};

struct VisionArgs {
	int fn = 0;
	
	void Jsonize(JsonIO& json) {
		json	("fn", fn)
				;
	}
	String Get() const {return StoreAsJson(*this);}
	void Put(const String& s) {LoadFromJson(*this, s);}
	
};

struct SnapshotArgs {
	int fn = 0;
	String entity, title, text;
	VectorMap<String,String> songs;
	
	void Jsonize(JsonIO& json) {
		json	("fn", fn)
				("entity", entity)
				("title", title)
				("text", text)
				("songs", songs)
				;
	}
	String Get() const {return StoreAsJson(*this);}
	void Put(const String& s) {LoadFromJson(*this, s);}
	
};

struct VideoSolverArgs {
	int fn = 0;
	String text, line;
	VectorMap<String,String> parts;
	Vector<String> prompts;
	
	void Jsonize(JsonIO& json) {
		json	("fn", fn)
				("text", text)
				("line", line)
				("parts", parts)
				("prompts", prompts)
				;
	}
	String Get() const {return StoreAsJson(*this);}
	void Put(const String& s) {LoadFromJson(*this, s);}
	
};

struct DemandArgs {
	int fn = 0;
	String role, need, action, event;
	int platform = 0;
	VectorMap<String,Vector<String>> need_causes;
	Vector<bool> enabled;
	
	void Jsonize(JsonIO& json) {
		json	("fn", fn)
				("role", role)
				("need", need)
				("action", action)
				("event", event)
				("platform", platform)
				("need_causes", need_causes)
				("enabled", enabled)
				;
	}
	String Get() const {return StoreAsJson(*this);}
	void Put(const String& s) {LoadFromJson(*this, s);}
	
};

struct ScriptPostArgs {
	int fn = 0;
	String key, desc, text;
	Vector<String> lines, scores;
	bool is_female;
	
	void Jsonize(JsonIO& json) {
		json	("fn", fn)
				("key", key)
				("desc", desc)
				("text", text)
				("lines", lines)
				("scores", scores)
				("is_female", is_female)
				;
	}
	String Get() const {return StoreAsJson(*this);}
	void Put(const String& s) {LoadFromJson(*this, s);}
	
};

struct BeliefArgs {
	int fn = 0;
	Vector<String> user;
	Vector<String> pos, neg;
	
	void Jsonize(JsonIO& json) {
		json	("fn", fn)
				("user", user)
				("pos", pos)
				("neg", neg)
				;
	}
	String Get() const {return StoreAsJson(*this);}
	void Put(const String& s) {LoadFromJson(*this, s);}
	
};

struct ScriptTextProcessArgs {
	int fn = 0;
	Vector<String> user;
	Vector<String> pos, neg;
	
	int sub_fn = -1;
	int lng_i = -1;
	VectorMap<String,String> artist, release, song;
	Vector<String> parts, attrs, phrases, scores;
	Vector<int> counts, offsets;
	String part, vision, ref;
	bool is_story = false;
	bool is_unsafe = false;
	bool is_self_centered = false;
	double factor = 0;
	
	

	void Jsonize(JsonIO& json) {
		json	("fn", fn)
				("user", user)
				("pos", pos)
				("neg", neg)
				("sub_fn", sub_fn)
				("lng_i", lng_i)
				("artist", artist)
				("release", release)
				("song", song)
				("parts", parts)
				("attrs", attrs)
				("phrases", phrases)
				("scores", scores)
				("counts", counts)
				("offsets", offsets)
				("part", part)
				("vision", vision)
				("ref", ref)
				("is_story", is_story)
				("is_unsafe", is_unsafe)
				("is_self_centered", is_self_centered)
				("factor", factor)
					;
	}
	String Get() const {return StoreAsJson(*this);}
	void Put(const String& s) {LoadFromJson(*this, s);}
	
};

struct BiographyProcessArgs {
	int fn = 0;
	String category;
	String text;
	int year = 0;
	Vector<String> scores;
	
	
	void Jsonize(JsonIO& json) {
		json	("fn", fn)
				("category", category)
				("text", text)
				("year", year)
				("scores", scores)
			;
	}
	String Get() const {return StoreAsJson(*this);}
	void Put(const String& s) {LoadFromJson(*this, s);}
};

struct BiographySummaryProcessArgs {
	int fn = 0;
	//String text, description, profile, photo_description;
	VectorMap<String,String> parts;
	//int len = 0;
	
	void Jsonize(JsonIO& json) {
		json	//("text", text)
				//("desc", description)
				("fn", fn)
				("parts", parts)
				//("len", len)
				//("profile", profile)
				//("photo_description", photo_description)
				;
	}
	String Get() const {return StoreAsJson(*this);}
	void Put(const String& s) {LoadFromJson(*this, s);}
};

struct ConceptualFrameworkArgs {
	int fn = 0;
	VectorMap<String,String> elements;
	Vector<String> scores;
	String lyrics, genre;
	
	void Jsonize(JsonIO& json) {
		json	("fn", fn)
				("elements", elements)
				("scores", scores)
				("lyrics", lyrics)
				("genre", genre)
				;
	}
	String Get() const {return StoreAsJson(*this);}
	void Put(const String& s) {LoadFromJson(*this, s);}
};

struct MarketplaceArgs {
	int fn = 0;
	VectorMap<String,String> map;
	
	void Jsonize(JsonIO& json) {
		json	("fn", fn)
				("map", map)
				;
	}
	String Get() const {return StoreAsJson(*this);}
	void Put(const String& s) {LoadFromJson(*this, s);}
};

struct BiographyGeneratorArgs {
	int fn = 0;
	String name, biography, preferred_genres, text;
	int birth_year, category, year;
	
	void Jsonize(JsonIO& json) {
		json	("fn", fn)
				("name", name)
				("biography", biography)
				("preferred_genres", preferred_genres)
				("text", text)
				("birth_year", birth_year)
				("category", category)
				("year", year)
				;
	}
	String Get() const {return StoreAsJson(*this);}
	void Put(const String& s) {LoadFromJson(*this, s);}
};

struct CodeArgs {
	int fn = 0;
	String file;
	Vector<String> features;
	String platform, os, ide, lang;
	
	enum {
		MAKE_CODE,
		GET_PLATFORMS,
		GET_OPERATING_SYSTEMS,
		GET_SW_LANGUAGES,
		GET_SW_PARADIGMS,
		GET_IDE,
		GET_LIBRARIES,
		GET_PROJECT_TYPES,
	};
	void Jsonize(JsonIO& json) {
		json	("fn", fn)
				("file", file)
				("platform", platform)
				("os", os)
				("ide", ide)
				("lang", lang)
				;
	}
	String Get() const {return StoreAsJson(*this);}
	void Put(const String& s) {LoadFromJson(*this, s);}
};


END_TEXTLIB_NAMESPACE


#endif
