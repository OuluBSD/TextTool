#ifndef _TextDatabase_Script_h_
#define _TextDatabase_Script_h_


BEGIN_TEXTLIB_NAMESPACE



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
	Vector<String> active_source_wordsalad;
	Vector<String> active_source_text;
	Vector<Vector<String>> source_text_suggestions;
	Vector<String> source; // lines
	Vector<String> ai_source;
	String rhyme_scheme;
	String mockup;
	VectorMap<String,String> data;
	bool outdated_suggestions = true;
	int content_cursor = -1;
	PartType part_type = SINGING;
	int bar_length = 0;
	Vector<Vector<Vector<Color>>> colors;
	Vector<Vector<Color>> listener_colors;
	Vector<Vector<Vector<String>>> vocabulary;
	Vector<Vector<String>> wordsalads;
	Vector<Vector<Color>> wordsalad_clrs;
	Vector<double> wordgroup_factors;
	RhymeContainer nana;
	Vector<Vector<int>> thrd_actions;
	Vector<String> saved_scripts;
	//Vector<int> clr_list;
	Vector<bool> actions_enabled;
	Vector<int> phrase_parts;
	//Index<int> picked_phrase_parts;
	String singer;
	
	static String GetTypeString(int part_type);
	static String GetTypeString(PartType part_type);
	String GetTypeString() const;
	int GetExpectedLineCount(Script& s) const;
	int GetContrastIndex(Script& s) const;
	
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("type", type)
			("active_source_wordsalad", active_source_wordsalad)
			("active_source_text", active_source_text)
			("source_text_suggestions", source_text_suggestions)
			("source", source)
			("ai_source", ai_source)
			("rhyme_scheme", rhyme_scheme)
			("data", data)
			("outdated_suggestions", outdated_suggestions)
			("content_cursor", content_cursor)
			("bar_length", bar_length)
			("part_type", (int&)part_type)
			("colors", colors)
			("listener_colors", listener_colors)
			("vocabulary", vocabulary)
			("wordsalads", wordsalads)
			("wordsalad_clrs", wordsalad_clrs)
			("wordgroup_factors", wordgroup_factors)
			("mockup", mockup)
			("nana", nana)
			("thrd_actions", thrd_actions)
			("saved_" + __scripts, saved_scripts)
			("singer", singer)
			//("clr_list", clr_list)
			//("picked_phrase_parts", picked_phrase_parts)
			;
		JsonCompressedStream(json, "actions_enabled", actions_enabled);
		JsonCompressedStream(json, "phrase_parts", phrase_parts);
	}
	
	// Temp
	Vector<int> valid_rhyme_schemes;
};

struct StructSuggestion {
	String name;
	Vector<String> parts;
	Vector<String> attrs;
	
	void operator=(const StructSuggestion& s) {
		name = s.name;
		parts <<= s.parts;
		attrs <<= s.attrs;
	}
	void Clear() {
		name.Clear();
		parts.Clear();
		attrs.Clear();
	}
	double GetEstimatedDuration(int bpm) const;
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("parts", parts)
			("attrs", attrs)
			;
	}
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

struct ContentVisionIdea : Moveable<ContentVisionIdea> {
	int tc_i = -1, con_i = -1;
	String text, singers_2, singers_3;
	Vector<int> scores;
	double ScoreAv() const {if (scores.IsEmpty()) return 0; int s = 0; for (int i : scores) s += i; return (double)s/scores.GetCount();}
	
	void Jsonize(JsonIO& json) {
		json
			("tc_i", tc_i)
			("con_i", con_i)
			("text", text)
			("s2", singers_2)
			("s3", singers_3)
			("scores", scores)
			;
	}
	bool operator()(const ContentVisionIdea& a, const ContentVisionIdea& b) const {return a.ScoreAv() >= b.ScoreAv();}
};

struct ContentVisionOwner {
	Vector<ContentVisionIdea>	ideas;
	
	Vector<int> FindIdeaIndices(int tc_i, int con_i) const;
	Vector<const ContentVisionIdea*> FindIdeas(int tc_i, int con_i) const;
	Vector<ContentVisionIdea*> FindIdeas(int tc_i, int con_i);
	void ClearIdeas(int tc_i, int con_i);
	double FindBestScore(int tc_i) const;
	double FindBestScore(int tc_i, int con_i) const;
};


struct Script : DataFile, ContentVisionOwner {
	String						native_title;
	String						english_title;
	String						copyright;
	String						content_vision;
	int							typeclass = 0;
	int							content = 0;
	int							lng_i = 0;
	int							belief_i = 0;
	bool						is_unsafe = false;
	bool						is_story = false;
	bool						is_self_centered = false;
	String						singer0_name;
	String						singer1_name;
	String						singer2_name;
	String						singer0_parts;
	String						singer1_parts;
	String						singer2_parts;
	String						lead;
	
	String user_structure;
	String required_parts;
	String avoid_parts;
	String structure_suggestion_description;
	int parts_total = 0;
	int bpm = 0;
	
	int verse_length = 8;
	int prechorus_length = 2;
	int chorus_length = 4;
	int bridge_length = 4;
	
	String						structure_str;
	String						text;
	StructSuggestion			active_struct;
	Array<StaticPart>			parts;
	Array<StructSuggestion>		struct_suggs;
	Vector<bool>				simple_attrs;
	Index<int>					picked_phrase_parts;
	Vector<int>					clr_list;
	VectorMap<int, String>		suggestions;
	Vector<String>				post_analysis;
	Array<ScriptPostFix>		postfixes;
	
	Vector<int> GetPartPositions(const StaticPart& part) const;
	Vector<int> GetPreviousParts(const StaticPart& part) const;
	Vector<int> GetPreviousPartsNonSkipped(const StaticPart& part) const;
	int FindPart(const StaticPart& part) const;
	Vector<StaticPart*> GetNonSkippedStructureParts();
	StaticPart* FindPartByType(const String& type);
	StaticPart* FindPartByName(const String& name);
	int GetFirstPartPosition() const;
	String GetAnyTitle() const;
	
	//Script() {post_analysis.SetCount(POSTSCRIPT_COUNT);}
	Script() {post_analysis.SetCount(SCORE_COUNT);}
	void Store(Entity& a);
	void LoadTitle(Entity& a, String title);
	void Jsonize(JsonIO& json) {
		json
			("title", native_title)
			("english_title", english_title)
			("copyright", copyright)
			("content_vision", content_vision)
			(__typeclass, typeclass)
			(__content, content)
			("is_" + __unsafe, is_unsafe)
			("is_" + __story, is_story)
			("is_" + __self_centered, is_self_centered)
			
			("user_structure", user_structure)
			("required_parts", required_parts)
			("avoid_parts", avoid_parts)
			("structure_suggestion_description", structure_suggestion_description)
			("parts_total", parts_total)
			("bpm", bpm)
			
			("verse_length", verse_length)
			("prechorus_length", prechorus_length)
			("chorus_length", chorus_length)
			("bridge_length", bridge_length)
			
			("structure_str", structure_str)
			("text", text)
			("active_struct", active_struct)
			("parts", parts)
			("struct_suggs", struct_suggs)
			("picked_phrase_parts", picked_phrase_parts)
			("clr_list", clr_list)
			("suggestions", suggestions)
			("lng_i", lng_i)
			("belief_i", belief_i)
			("post_analysis", post_analysis)
			
			("postfixes", postfixes)
			
			("singer0_name", singer0_name)
			("singer1_name", singer1_name)
			("singer2_name", singer2_name)
			("singer0_parts", singer0_parts)
			("singer1_parts", singer1_parts)
			("singer2_parts", singer2_parts)
			("lead", lead)
			
			("ideas", ideas)
			;
		
		JsonCompressedStream(json, "simple_attrs", simple_attrs);
		
		if (json.IsLoading()) {
			//if (post_analysis.GetCount() < POSTSCRIPT_COUNT)
			//	post_analysis.SetCount(POSTSCRIPT_COUNT);
			if (post_analysis.GetCount() < SCORE_COUNT)
				post_analysis.SetCount(SCORE_COUNT);
		}
	}
	
};


END_TEXTLIB_NAMESPACE


#endif
