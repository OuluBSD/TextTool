#ifndef _Database_Lyrics_h_
#define _Database_Lyrics_h_


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
	Vector<String> saved_lyrics;
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
			("saved_lyrics", saved_lyrics)
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

struct Lyrics {
	String						native_title;
	String						english_title;
	String						prj_name;
	String						structure_str;
	StructSuggestion			active_struct;
	Array<StaticPart>			parts;
	Array<StructSuggestion>		struct_suggs;
	Vector<bool>				simple_attrs;
	Index<int>					picked_phrase_parts;
	Vector<int>					clr_list;
	bool						is_rapper = false;
	
	
	Vector<int> GetPartPositions(const StaticPart& part) const;
	Vector<int> GetPreviousParts(const StaticPart& part) const;
	Vector<int> GetPreviousPartsNonSkipped(const StaticPart& part) const;
	int FindPart(const StaticPart& part) const;
	Vector<StaticPart*> GetNonSkippedStructureParts();
	StaticPart* FindPartByType(const String& type);
	StaticPart* FindPartByName(const String& name);
	int GetFirstPartPosition() const;
	
	void Jsonize(JsonIO& json) {
		json
			("title", native_title)
			("english_title", english_title)
			("prj_name", prj_name)
			("structure_str", structure_str)
			("parts", parts)
			("active_struct", active_struct)
			("struct_suggs", struct_suggs)
			("picked_phrase_parts", picked_phrase_parts)
			("clr_list", clr_list)
			("is_rapper", is_rapper)
			;
		JsonCompressedStream(json, "simple_attrs", simple_attrs);
		
	}
	
};


#endif
