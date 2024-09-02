#ifndef _TextDatabase_Script_h_
#define _TextDatabase_Script_h_


BEGIN_TEXTLIB_NAMESPACE



struct LineScore : Moveable<LineScore> {
	Vector<String> lines;
	Vector<int> scores;
	int line_n = 0;
	
	String Get(int i, int j) const;
	int GetScore(int i, int j) const;
	void SetCount(int i, int line_n);
	void Set(int i, int j, const String& s);
	void SetScore(int i, int j, int value);
	int GetCount() const;
	
	void Jsonize(JsonIO& json) {
		json
			("lines", lines)
			("scores", scores)
			("line_n", line_n)
			;
	}
};

struct LineElement {
	String element;
	AttrHeader attr;
	ActionHeader act;
	int clr_i = -1;
	int typeclass_i = -1;
	int con_i = -1;
	hash_t sorter = 0;
	
	void Jsonize(JsonIO& json) {
		json
			("e", element)
			("at", attr)
			("clr", clr_i)
			("a", act)
			("t", typeclass_i)
			("c", con_i)
			("s", (int64&)sorter)
			;
	}
};


struct DynLine : Moveable<DynLine> {
	String			text;
	String			alt_text;
	String			edit_text;
	String			user_text;
	String			expanded;
	Vector<String>	suggs;
	int				pp_i = -1;
	int				end_pp_i = -1;
	LineElement		el;
	
	// Suggestion variables
	int				style_type = 0;
	int				style_entity = 0;
	int				safety = 0;
	int				line_len = 0;
	int				connector = 0;
	String			line_begin;
	
	void Jsonize(JsonIO& json) {
		json
			("text", text)
			("alt_text", alt_text)
			("edit_text", edit_text)
			("user_text", user_text)
			("ex", expanded)
			("el", el)
			("suggs", suggs)
			("pp_i", pp_i)
			("end_pp_i", end_pp_i)
			("style_type", style_type)
			("style_entity", style_entity)
			("safety", safety)
			("line_len", line_len)
			("connector", connector)
			("line_begin", line_begin)
			;
		if (json.IsLoading()) {
			String element;
			json("element", element);
			if (!element.IsEmpty() && el.element.IsEmpty())
				el.element = element;
		}
	}
	void CopySuggestionVars(const DynLine& dl) {
		style_type = dl.style_type;
		style_entity = dl.style_entity;
		safety = dl.safety;
		line_len = dl.line_len;
		connector = dl.connector;
		line_begin = dl.line_begin;
	}
};

struct DynSub : Moveable<DynSub> {
	LineElement		el;
	Vector<DynLine>	lines;
	String			story;
	
	void Jsonize(JsonIO& json) {
		json
			("lines", lines)
			("el", el)
			("story", story)
			;
		if (json.IsLoading()) {
			String e0, e1;
			json("element0", e0);
			json("element1", e1);
			if (!e0.IsEmpty() && el.element.IsEmpty())
				el.element = e0;
			if (!e1.IsEmpty() && el.element.IsEmpty())
				el.element = e1;
		}
	}
};

struct DynPart {
	VoiceType		voice_type = VOICE_SINGING;
	TextPartType	text_type = TXT_NORMAL;
	int				text_num = -1;
	int				text_lines = 0;
	int				text_lines_per_sub = 0;
	String			person;
	LineElement		el;
	Vector<DynSub>	sub;
	Vector<int>		phrase_parts;
	String			story;
	
	void Jsonize(JsonIO& json) {
		json
			("voice_type", (int&)voice_type)
			("text_type", (int&)text_type)
			("text_num", text_num)
			("text_lines", text_lines)
			("text_lines_per_sub", text_lines_per_sub)
			("person", person)
			("el", el)
			("sub", sub)
			("phrase_parts", phrase_parts)
			("story", story)
			;
		if (json.IsLoading()) {
			String element;
			json("element", element);
			if (!element.IsEmpty() && el.element.IsEmpty())
				el.element = element;
		}
	}
	String GetName(int appmode) const;
	int GetExpectedLineCount() const;
	int GetContrastIndex() const;
	String GetLineElementString(int line) const;
	
};

// Deprecated
struct StaticPart {
	// Part types
	typedef enum : int {
		SINGING,
		RAPPING,
		POETRY,
		DIALOG,
		SKIP,
		
		PART_TYPE_COUNT
	} PartType;
	
	String name;
	String type; // abbreviation like V1, PC2, C
	PartType part_type = SINGING;
	String singer;
	RhymeContainer text, coarse_text;
	RhymeContainer reference;
	RhymeContainer generated;
	Vector<LineScore>	conv;
	Vector<bool> actions_enabled;
	Vector<int> phrase_parts;
	
	static String GetTypeString(int part_type);
	static String GetTypeString(PartType part_type);
	String GetTypeString() const;
	int GetExpectedLineCount(Script& s) const;
	int GetContrastIndex(Script& s) const;
	
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("type", type)
			("part_type", (int&)part_type)
			("singer", singer)
			("text", text)
			("coarse_text", coarse_text)
			("reference", reference)
			("generated", generated)
			("conv", conv)
			;
		JsonCompressedStream(json, "actions_enabled", actions_enabled);
		JsonCompressedStream(json, "phrase_parts", phrase_parts);
	}
	
	// Temp
	Vector<int> valid_rhyme_schemes;
};

// Deprecated
struct ScriptStructure {
	String name;
	Vector<String> parts;
	Vector<String> attrs;
	int structured_script_i = -1;
	
	void operator=(const ScriptStructure& s) {
		name = s.name;
		parts <<= s.parts;
		attrs <<= s.attrs;
		structured_script_i = s.structured_script_i;
	}
	void Clear() {
		name.Clear();
		parts.Clear();
		attrs.Clear();
		structured_script_i = -1;
	}
	double GetEstimatedDuration(int bpm) const;
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("parts", parts)
			("attrs", attrs)
			("structured_script_i", structured_script_i)
			;
	}
};


// In use
struct ScriptStruct : Moveable<ScriptStruct> {
	ScriptStruct() {}
	ScriptStruct(const ScriptStruct& s) {*this = s;}
	ScriptStruct(ScriptStruct&& s) {*this = s;}
	void operator=(const ScriptStruct& s) {
		parts <<= s.parts;
	}
	
	struct SubSubPart : Moveable<SubSubPart> {
		Vector<int> token_texts;
		int cls = -1;
		
		SubSubPart() {}
		SubSubPart(const SubSubPart& s) {*this = s;}
		void Serialize(Stream& s) {s % token_texts % cls;}
		void Jsonize(JsonIO& json) {json("token_texts", token_texts)("cls", cls);}
		void operator=(const SubSubPart& s) {
			token_texts <<= s.token_texts;
			cls = s.cls;
		}
	};
	struct SubPart : Moveable<SubPart> {
		Vector<SubSubPart> sub;
		int cls = -1;
		int repeat = 0;
		
		SubPart() {}
		SubPart(const SubPart& s) {*this = s;}
		void Serialize(Stream& s) {s % sub % cls % repeat;}
		void Jsonize(JsonIO& json) {json("sub", sub)("cls", cls)("repeat", repeat);}
		void operator=(const SubPart& s) {
			sub <<= s.sub;
			cls = s.cls;
			repeat = s.repeat;
		}
	};
	struct Part : Moveable<Part> {
		Vector<SubPart> sub;
		int type = -1;
		int num = -1;
		int cls = -1, typeclass = -1, content = -1;
		
		Part() {}
		Part(const Part& p) {*this = p;}
		void Serialize(Stream& s) {s % sub % type % num % cls % typeclass % content;}
		void Jsonize(JsonIO& json) {json("sub", sub)("type", type)("num", num)("cls", cls)("tc", typeclass)("c", content);}
		void operator=(const Part& s) {
			sub <<= s.sub;
			type = s.type;
			num = s.num;
			cls = s.cls;
			typeclass = s.typeclass;
			content = s.content;
		}
	};
	Vector<Part> parts;
	
	void Serialize(Stream& s) {s % parts;}
	
	void Jsonize(JsonIO& json) {
		json
			("parts", parts)
			;
	}
	bool HasAnyClasses() const {
		for (const auto& p : parts) {
			if (p.cls >= 0) return true;
			for (const auto& s : p.sub) {
				if (s.cls >= 0) return true;
				for (const auto& ss : s.sub) {
					if (ss.cls >= 0) return true;
				}
			}
		}
		return false;
	}
	double GetNormalScore() const;
	
	#if 0
	String StoreToString() {
		StringDumper d;
		int i = parts.GetCount();
		d % i;
		for (Part& p : parts) {
			i = p.sub.GetCount();
			d % p.type % p.num % i;
			for (SubPart& s : p.sub) {
				d % s.repeat;
				i = s.token_texts.GetCount();
				d % i;
				for (int tt : s.token_texts)
					d % tt;
			}
		}
		return d;
	}
	void LoadFromString(const String& s) {
		StringParser d(s);
		int i;
		d % i;
		if (d.err || i > 1000)
			return;
		parts.SetCount(i);
		for (Part& p : parts) {
			d % p.type % p.num % i;
			if (d.err || i > 1000)
				return;
			p.sub.SetCount(i);
			for (SubPart& s : p.sub) {
				d % s.repeat % i;
				s.token_texts.SetCount(i,-1);
				for (int& tt : s.token_texts)
					d % tt;
			}
		}
	}
	#endif
};


#if 0
enum {
	POSTSCRIPT_STORYLINE_INCONSISTENCIES,
	POSTSCRIPT_LOGICAL_ISSUES,
	POSTSCRIPT_DIFFICULT_PARTS_TO_RELATE_TO,
	POSTSCRIPT_CONTRADICTORY_MESSAGES,
	POSTSCRIPT_NEGATIVE_HIDDEN_MESSAGES,
	POSTSCRIPT_NEGATIVE_UNDERLYING_THEMES,
	POSTSCRIPT_NEGATIVE_SOCIAL_COMMENTARY,
	POSTSCRIPT_NEGATIVE_EMOTIONAL_DEPTH,
	POSTSCRIPT_NEGATIVE_SOCIETAL_INFLUENCE,
	POSTSCRIPT_NEGATIVE_CULTURAL_INFLUENCE,
	POSTSCRIPT_PSYCHOANALYTICAL_SEXUAL_TRAUMA_BASED_MESSAGES,
	POSTSCRIPT_PSYCHOANALYTICAL_VIOLENCE_TRAUMA_BASED_MESSAGES,
	POSTSCRIPT_PSYCHOANALYTICAL_ECONOMICAL_TRAUMA_BASED_MESSAGES,
	POSTSCRIPT_PSYCHOANALYTICAL_SOCIAL_TRAUMA_BASED_MESSAGES,
	POSTSCRIPT_PSYCHOANALYTICAL_AUTHORITY_TRAUMA_BASED_MESSAGES,
	POSTSCRIPT_PSYCHOANALYTICAL_RELATIONSHIP_TRAUMA_BASED_MESSAGES,
	POSTSCRIPT_SOCIAL_PSYCHOLOGY_OVERSIMPLIFIED_BEHAVIOR,
	POSTSCRIPT_SOCIAL_PSYCHOLOGY_OVERSIMPLIFIED_MOTIVATION,
	POSTSCRIPT_SOCIAL_PSYCHOLOGY_OVERSIMPLIFIED_PERCEPTION,
	POSTSCRIPT_SOCIAL_PSYCHOLOGY_OVERSIMPLIFIED_IMMATURITY,
	POSTSCRIPT_SOCIAL_PSYCHOLOGY_NEGATIVE_SACRIFICATION,
	POSTSCRIPT_SOCIAL_PSYCHOLOGY_NEGATIVE_APPEAL_TO_AUTHORITY,
	POSTSCRIPT_SOCIAL_PSYCHOLOGY_NEGATIVE_GROUPTHINK,
	POSTSCRIPT_SOCIAL_PSYCHOLOGY_NEGATIVE_STEREOTYPES_AND_BIASES,
	POSTSCRIPT_MENTAL_HEALTH_UNHEALTHY_COPING_MECHANISMS,
	POSTSCRIPT_MENTAL_HEALTH_ROMANTICIZATION_OF_MENTAL_ILLNESS,
	POSTSCRIPT_MENTAL_HEALTH_PERPETUATION_OF_HARMFUL_STIGMAS,
	POSTSCRIPT_MENTAL_HEALTH_INFERTILITY,
	
	POSTSCRIPT_COUNT,
};

String GetPostScriptAnalysisKey(int i);
String GetPostScriptAnalysisDescription(int i);

enum {
	POSTSCRIPTMOD_TONE_INC_HAPPY,
	POSTSCRIPTMOD_TONE_INC_SAD,
	POSTSCRIPTMOD_TONE_INC_ANGRY,
	POSTSCRIPTMOD_TONE_INC_NOSTALGIC,
	POSTSCRIPTMOD_TONE_INC_ROMANTIC,
	POSTSCRIPTMOD_TONE_INC_SARCASTIC,
	POSTSCRIPTMOD_TONE_INC_HOPEFUL,
	POSTSCRIPTMOD_TONE_INC_ANXIOUS,
	POSTSCRIPTMOD_TONE_INC_MELANCHOLIC,
	POSTSCRIPTMOD_TONE_INC_APATHETIC,
	POSTSCRIPTMOD_TONE_INC_EMPOWERING,
	POSTSCRIPTMOD_TONE_INC_HEARTWARMING,
	POSTSCRIPTMOD_TONE_INC_DARK,
	POSTSCRIPTMOD_TONE_INC_UPLIFTING,
	POSTSCRIPTMOD_TONE_INC_PLAYFUL,
	POSTSCRIPTMOD_TONE_INC_DRAMATIC,
	POSTSCRIPTMOD_TONE_INC_REFLECTIVE,
	POSTSCRIPTMOD_TONE_INC_MYSTERIOUS,
	POSTSCRIPTMOD_TONE_INC_HUMOROUS,
	POSTSCRIPTMOD_TONE_INC_PASSIONATE,
	POSTSCRIPTMOD_TONE_INC_BITTERSWEET,
	POSTSCRIPTMOD_TONE_INC_REBELLIOUS,
	POSTSCRIPTMOD_CHANGE_THE_PERSPECTIVE,
	POSTSCRIPTMOD_CHANGE_THE_TENSE,
	POSTSCRIPTMOD_CHANGE_THE_POINT_OF_VIEW,
	POSTSCRIPTMOD_ADD_ADJECTIVES,
	POSTSCRIPTMOD_CHANGE_ADJECTIVES,
	POSTSCRIPTMOD_SWITCH_ORDER_OF_LINES,
	POSTSCRIPTMOD_ADD_REPETITION,
	POSTSCRIPTMOD_ADD_REFERENCES_TO_CURRENT_EVENTS,
	POSTSCRIPTMOD_ADD_REFERENCES_TO_POPULAR_CULTURE,
	POSTSCRIPTMOD_INJECT_PERSONAL_EXPERIENCE,
	POSTSCRIPTMOD_CHANGE_TO_DIFFERENT_SPEAKER_AND_CHARACTER,
	POSTSCRIPTMOD_ADD_INTERNAL_RHYMES,
	POSTSCRIPTMOD_INCORPORATE_LITERARY_ALLITERATIONS,
	POSTSCRIPTMOD_INCORPORATE_LITERARY_ASSONANCES,
	POSTSCRIPTMOD_INCORPORATE_DIFFERENT_THEME,
	POSTSCRIPTMOD_ADD_SLANG_AND_DIALECT,
	POSTSCRIPTMOD_REWRITE_TIME_PERIOD,
	POSTSCRIPTMOD_REWRITE_CULTURAL_CONTEXT,
	POSTSCRIPTMOD_ADD_UNCONVENTIONAL_WORD_CHOICES,
	POSTSCRIPTMOD_ADD_UNEXPECTED_WORD_CHOICES,
	
	POSTSCRIPTMOD_COUNT,
};

String GetPostScriptModificationKey(int i);
#endif


struct ScriptPostFix {
	struct Weak : Moveable<Weak> {
		int line_i = -1;
		int altline_i = -1;
		String description;
		void Jsonize(JsonIO& json) {
			json
				("l0", line_i)
				("l1", altline_i)
				("d", description)
				;
		}
	};
	struct Improvement : Moveable<Improvement> {
		int line_i = -1;
		String text;
		void Jsonize(JsonIO& json) {
			json
				("l", line_i)
				("txt", text)
				;
		}
	};
	struct Variation : Moveable<Variation> {
		String text;
		Vector<int> scores;
		int ScoreSum() const {int s = 0; for (int i : scores) s += i; return s;}
		double ScoreAv() const {int s = 0; for (int i : scores) s += i; return (double)s/scores.GetCount();}
		void Jsonize(JsonIO& json) {
			json
				("txt", text)
				("scores", scores)
				;
			if (json.IsLoading()) scores.SetCount(10,0);
		}
		
	};
	Vector<Vector<Weak>> weaks;
	Vector<String> src_lines;
	VectorMap<int,String> src_line_parts;
	Vector<Vector<Improvement>> improvements;
	Vector<Variation> variations;
	
	void Jsonize(JsonIO& json) {
		json
			("weaks", weaks)
			("src_lines", src_lines)
			("src_line_parts", src_line_parts)
			("improvements", improvements)
			("variations", variations)
			;
	}
};

struct Script : DataFile {
	String						native_title;
	String						copyright;
	String						content_vision;
	String						lead;
	int64						belief_uniq = 0;
	int							lng_i = 0;
	int							typeclass = 0;
	int							content = 0;
	bool						is_unsafe = false;
	bool						is_story = false;
	bool						is_self_centered = false;
	
	Vector<bool>				simple_attrs;
	Vector<int>					clr_list;
	Vector<String>				post_analysis;
	Array<ScriptPostFix>		postfixes;
	ScriptStruct				ref_struct;
	
	Array<DynPart>				parts;
	Vector<bool>				actions_enabled;
	Vector<int>					phrase_parts[ContentType::PART_COUNT];
	
	// Deprecated
	ScriptStructure				__active_struct;
	VectorMap<int, String>		__suggestions;
	String						__text;
	Array<StaticPart>			__parts;
	
	
	Vector<int> GetPartPositions(const StaticPart& part) const;
	Vector<int> GetPreviousParts(const StaticPart& part) const;
	Vector<int> GetPreviousPartsNonSkipped(const StaticPart& part) const;
	int FindPart(const StaticPart& part) const;
	Vector<StaticPart*> GetNonSkippedStructureParts();
	StaticPart* FindPartByType(const String& type);
	DynPart* FindPartByName(int appmode, const String& name);
	int GetFirstPartPosition() const;
	String GetAnyTitle() const;
	String GetText() const;
	String GetUserText(int appmode) const;
	String GetTextStructure(bool coarse) const;
	void LoadStructuredText(const String& s);
	void LoadStructuredTextExt(const String& s);
	void SetEditText(const String& s);
	
	//Script() {post_analysis.SetCount(POSTSCRIPT_COUNT);}
	Script() {post_analysis.SetCount(SCORE_COUNT);}
	~Script();
	void Store(Entity& a);
	void LoadTitle(Entity& a, String title);
	void Jsonize(JsonIO& json) {
		json
			("title", native_title)
			("copyright", copyright)
			("content_vision", content_vision)
			("lead", lead)
			("belief_uniq", belief_uniq)
			("lng_i", lng_i)
			(__typeclass, typeclass)
			(__content, content)
			("is_" + __unsafe, is_unsafe)
			("is_" + __story, is_story)
			("is_" + __self_centered, is_self_centered)
			("clr_list", clr_list)
			("post_analysis", post_analysis)
			("postfixes", postfixes)
			("parts", __parts)
			("ref_struct", ref_struct)
			("new_parts", parts)
			("active_struct", __active_struct)
			("suggestions", __suggestions)
			("text", __text)
			;
		
		for(int i = 0; i < ContentType::PART_COUNT; i++)
			JsonCompressedStream(json, "phrase_parts" + IntStr(i), phrase_parts[i]);
		JsonCompressedStream(json, "actions_enabled", actions_enabled);
		JsonCompressedStream(json, "simple_attrs", simple_attrs);
		
		if (json.IsLoading()) {
			if (post_analysis.GetCount() < SCORE_COUNT)
				post_analysis.SetCount(SCORE_COUNT);
		}
	}
	
};


END_TEXTLIB_NAMESPACE


#endif
