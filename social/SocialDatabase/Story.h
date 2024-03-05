#ifndef _SocialDatabase_Story_h_
#define _SocialDatabase_Story_h_


BEGIN_SOCIALLIB_NAMESPACE


struct Company;


struct StoryPart {
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
	String active_idea[IDEAPATH_PARTCOUNT];
	Color active_idea_clr[IDEAPATH_PARTCOUNT];
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
	Vector<String> saved_stories;
	//Vector<int> clr_list;
	Vector<bool> actions_enabled;
	Vector<int> phrase_parts;
	//Index<int> picked_phrase_parts;
	
	static String GetTypeString(int part_type);
	static String GetTypeString(PartType part_type);
	String GetTypeString() const;
	
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
			("saved_stories", saved_stories)
			//("clr_list", clr_list)
			//("picked_phrase_parts", picked_phrase_parts)
			;
		JsonCompressedStream(json, "actions_enabled", actions_enabled);
		JsonCompressedStream(json, "phrase_parts", phrase_parts);
		for(int i = 0; i < IDEAPATH_PARTCOUNT; i++)
			json(	(String)"active_idea[" + IdeaPathString[IDEAPATH_PARTBEGIN+i][1] + "]",
					active_idea[i]);
		for(int i = 0; i < IDEAPATH_PARTCOUNT; i++)
			json(	(String)"active_idea_clr[" + IdeaPathString[IDEAPATH_PARTBEGIN+i][1] + "]",
					active_idea_clr[i]);
	}
	
	// Temp
	Vector<int> valid_rhyme_schemes;
};

struct StoryStructSuggestion {
	String name;
	Vector<String> parts;
	Vector<String> attrs;
	
	void operator=(const StoryStructSuggestion& s) {
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

struct Story : DataFile {
	String						native_title;
	String						english_title;
	String						copyright;
	String						content_vision;
	int							role = 0;
	int							generic = 0;
	bool						is_unsafe = false;
	
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
	StoryStructSuggestion			active_struct;
	Array<StoryPart>			parts;
	Array<StoryStructSuggestion>		struct_suggs;
	Vector<bool>				simple_attrs;
	Index<int>					picked_phrase_parts;
	Vector<int>					clr_list;
	VectorMap<int, String>		suggestions;
	
	
	Vector<int> GetPartPositions(const StoryPart& part) const;
	Vector<int> GetPreviousParts(const StoryPart& part) const;
	Vector<int> GetPreviousPartsNonSkipped(const StoryPart& part) const;
	int FindPart(const StoryPart& part) const;
	Vector<StoryPart*> GetNonSkippedStructureParts();
	StoryPart* FindPartByType(const String& type);
	StoryPart* FindPartByName(const String& name);
	int GetFirstPartPosition() const;
	String GetAnyTitle() const;
	
	Story() {}
	void Store(Company& a);
	void LoadTitle(Company& a, String title);
	void Jsonize(JsonIO& json) {
		json
			("title", native_title)
			("english_title", english_title)
			("copyright", copyright)
			("content_vision", content_vision)
			("role", role)
			("generic", generic)
			("is_unsafe", is_unsafe)
			
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
			;
		
		JsonCompressedStream(json, "simple_attrs", simple_attrs);
		
	}
	
};


END_SOCIALLIB_NAMESPACE


#endif
