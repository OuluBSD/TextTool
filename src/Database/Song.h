#ifndef _SongTool_Song_h_
#define _SongTool_Song_h_


struct SongIdea {
	String title, target_song, reference_song, description;
	
	void Serialize(Stream& s) {
		s % title
		  % target_song
		  % reference_song
		  % description
		  ;
	}
	void Jsonize(JsonIO& json) {
		json
			("title", title)
			("target_song", target_song)
			("reference_song", reference_song)
			("description", description)
			;
	}
};

#if 0
struct SongHeader {
	// Local
	VectorMap<String,String>				data;
	VectorMap<String, Vector<SnapAttrStr>>	unique_lines;
	Analysis								analysis;
	
	void Serialize(Stream& s) {
		s	% data
			% content
			% unique_lines
			% analysis;
	}
	void Jsonize(JsonIO& json) {
		json
			("content", content)
			/*("data", data)
			("unique_lines", unique_lines)
			("analysis", analysis)*/
			;
	}
};
#endif



struct StaticPerson {
	String text;
	Color clr;
	int cursor = -1;
	
	void Jsonize(JsonIO& json) {
		json
			("text", text)
			("clr", clr)
			("cursor", cursor)
			;
	}
};

struct StaticSymbolism {
	String text;
	Color clr;
	Array<StaticPerson> persons;
	int cursor = -1;
	
	void Jsonize(JsonIO& json) {
		json
			("text", text)
			("clr", clr)
			("persons", persons)
			("cursor", cursor)
			;
	}
};

struct StaticImagery {
	String text;
	Color clr;
	Array<StaticSymbolism> symbolisms;
	int cursor = -1;
	
	void Jsonize(JsonIO& json) {
		json
			("text", text)
			("clr", clr)
			("symbolisms", symbolisms)
			("cursor", cursor)
			;
	}
	
};

struct StaticContentSuggestion {
	String text;
	Color clr;
	Array<StaticImagery> imageries;
	int cursor = -1;
	
	void Jsonize(JsonIO& json) {
		json
			("text", text)
			("clr", clr)
			("imageries", imageries)
			("cursor", cursor)
			;
	}
};



struct StaticSuggestion {
	String style, content;
	int score = 0;
	int ai_score = 0;
	int ai_score_extra = 0;
	void Jsonize(JsonIO& json) {
		json
			("style", style)
			("content", content)
			("score", score)
			("ai_score", ai_score)
			("ai_score_extra", ai_score_extra)
			;
	}
};

struct StaticNote {
	String frozen_syllable;
	bool only_first_syllable = false;
	bool long_singable_syllable = false;
	
	void Jsonize(JsonIO& json) {
		json
			("frozen_syllable", frozen_syllable)
			("only_first_syllable", only_first_syllable)
			("long_singable_syllable", long_singable_syllable)
			;
	}
};


// deprecated?
struct StaticRhyme {
	Vector<String> source;
	//Vector<String> ai_source;
	Array<StaticNote> notes;
	Array<StaticSuggestion> suggestions;
	VectorMap<String,String> data;
	double beat_length = 0;
	bool outdated_suggestions = true;
	void Jsonize(JsonIO& json) {
		json
			("source", source)
			//("ai_source", ai_source)
			("notes", notes)
			("suggestion", suggestions)
			("data", data)
			("outdated_suggestions", outdated_suggestions)
			("beat_length", beat_length)
			;
	}
	int GetBestSuggestion() const;
};

struct StaticPhrase {
	String txt;
	String group, group_value;
	String src_artist, src_song;
	Color clr;
	
	void Jsonize(JsonIO& json) {
		json
			("txt", txt)
			("src_artist", src_artist)
			("src_song", src_song)
			("group", group)
			("group_value", group_value)
			("clr", clr)
			;
	}
};

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
	Array<StaticRhyme> rhymes;
	String rhyme_scheme;
	String mockup;
	VectorMap<String,String> data;
	bool outdated_suggestions = true;
	Array<StaticContentSuggestion> contents;
	int content_cursor = -1;
	PartType part_type = SINGING;
	int bar_length = 0;
	Vector<Vector<Vector<Color>>> colors;
	Vector<Vector<Color>> listener_colors;
	Vector<Vector<Vector<String>>> vocabulary;
	Vector<Vector<String>> wordsalads;
	Vector<Vector<Color>> wordsalad_clrs;
	Vector<double> wordgroup_factors;
	Array<StaticPhrase> phrases;
	RhymeContainer nana;
	Vector<Vector<int>> thrd_actions;
	Vector<String> saved_lyrics;
	
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
			("rhymes", rhymes)
			("rhyme_scheme", rhyme_scheme)
			("data", data)
			("outdated_suggestions", outdated_suggestions)
			("contents", contents)
			("content_cursor", content_cursor)
			("bar_length", bar_length)
			("part_type", (int&)part_type)
			("colors", colors)
			("listener_colors", listener_colors)
			("vocabulary", vocabulary)
			("wordsalads", wordsalads)
			("wordsalad_clrs", wordsalad_clrs)
			("wordgroup_factors", wordgroup_factors)
			("phrases", phrases)
			("mockup", mockup)
			("nana", nana)
			("thrd_actions", thrd_actions)
			("saved_lyrics", saved_lyrics)
			;
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


struct StaticAllegoricalDevice {
	Array<StaticContentSuggestion> contents;
	String text;
	Color clr;
	int cursor = -1;
	
	void Jsonize(JsonIO& json) {
		json
			("text", text)
			("clr", clr)
			("contents", contents)
			("cursor", cursor)
			;
	}
};

struct StaticToneSuggestion {
	Array<StaticAllegoricalDevice> allegories;
	String text;
	Color clr;
	int cursor = -1;
	
	void Jsonize(JsonIO& json) {
		json
			("text", text)
			("clr", clr)
			("allegories", allegories)
			("cursor", cursor)
			;
	}
};

struct StaticIdea {
	Array<StaticToneSuggestion> tones;
	String text;
	Color clr;
	int cursor = -1;
	
	void Jsonize(JsonIO& json) {
		json
			("text", text)
			("clr", clr)
			("tones", tones)
			("cursor", cursor)
			;
	}
};

struct StaticTheme {
	Array<StaticIdea> ideas;
	String text;
	Color clr;
	int cursor = -1;
	
	void Jsonize(JsonIO& json) {
		json
			("text", text)
			("clr", clr)
			("ideas", ideas)
			("cursor", cursor)
			;
	}
};

struct ProductionIdea {
	String topic, value;
	bool is_positive = true;
	
	void Jsonize(JsonIO& json) {
		json
			("topic", topic)
			("value", value)
			("is_positive", is_positive)
			;
	}
	
};

struct AttrProbability : Moveable<AttrProbability> {
	String group, value;
	int percent;
	
	AttrProbability() {}
	AttrProbability(const AttrProbability& r) {*this = r;}
	void operator=(const AttrProbability& r) {group = r.group; value = r.value; percent = r.percent;}
	void Serialize(Stream& s) {s % group % value % percent;}
	void Jsonize(JsonIO& json) {
		json
			("group", group)
			("value", value)
			("percent", percent)
			;
	}
};
struct Song :
	DataFile,
	EditorPtrs
{
	struct StructSuggestion {
		String name;
		Vector<String> parts;
		Vector<String> attrs;
		//Vector<String> chords;
		//Vector<int> part_types;
		
		void operator=(const StructSuggestion& s) {
			name = s.name;
			parts <<= s.parts;
			attrs <<= s.attrs;
			//chords <<= s.chords;
			//part_types <<= s.part_types;
		}
		void Clear() {
			name.Clear();
			parts.Clear();
			attrs.Clear();
			//chords.Clear();
			//part_types.Clear();
		}
		double GetEstimatedDuration(int bpm) const;
		void Jsonize(JsonIO& json) {
			json
				("name", name)
				("parts", parts)
				("attrs", attrs)
				//("chords", chords)
				//("part_types", part_types)
				;
		}
	};
	
	// Public
	String						artist;
	String						native_title;
	String						english_title;
	String						prj_name;
	String						structure_str;
	Vector<String>				structure; // TRASH
	VectorMap<String,String>	data;
	Array<StaticPart>			parts;
	Array<StructSuggestion>		struct_suggs;
	Array<StaticTheme>			themes;
	StructSuggestion			active_struct;
	int							default_line_syllables = 0;
	int							default_attr_count = 7;
	int							theme_cursor = -1;
	int							part_cursor = -1;
	String						active_idea[IDEAPATH_COUNT];
	Color						active_idea_clr[IDEAPATH_COUNT];
	Array<ProductionIdea>		prod_ideas;
	Vector<AttrProbability>		active_roles;
	
	void Store();
	void LoadTitle(String title);
	void ReloadStructure();
	
	Vector<int> GetPartPositions(const StaticPart& part) const;
	Vector<int> GetPreviousParts(const StaticPart& part) const;
	Vector<int> GetPreviousPartsNonSkipped(const StaticPart& part) const;
	int FindPart(const StaticPart& part) const;
	Vector<StaticPart*> GetNonSkippedStructureParts();
	StaticPart* FindPartByType(const String& type);
	int GetFirstPartPosition() const;
	
	void Serialize(Stream& s) {TODO}
	void Jsonize(JsonIO& json) {
		json
			("artist", artist)
			("title", native_title)
			("english_title", english_title)
			("prj_name", prj_name)
			("default_line_syllables", default_line_syllables)
			("default_attr_count", default_attr_count)
			("structure_str", structure_str)
			("structure", structure)
			("data", data)
			("parts", parts)
			("active_struct", active_struct)
			("struct_suggs", struct_suggs)
			("themes", themes)
			("theme_cursor", theme_cursor)
			("part_cursor", part_cursor)
			("prod_ideas", prod_ideas)
			("active_roles", active_roles)
			;
		for(int i = 0; i < IDEAPATH_COUNT; i++)
			json((String)"active_idea[" + IdeaPathString[i][1] + "]", active_idea[i]);
		
		for(int i = 0; i < IDEAPATH_COUNT; i++)
			json((String)"active_idea_clr[" + IdeaPathString[i][1] + "]", active_idea_clr[i]);
		
	}
	void FixPtrs() {
		{
			EditorPtrs::release = EditorPtrs::release;
			EditorPtrs::artist = EditorPtrs::artist;
			EditorPtrs::song = this;
		}
	}
};



#endif
