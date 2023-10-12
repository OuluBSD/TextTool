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
struct StaticRhyme {
	Vector<String> source;
	//Vector<String> ai_source;
	Array<StaticSuggestion> suggestions;
	VectorMap<String,String> data;
	bool outdated_suggestions = true;
	void Jsonize(JsonIO& json) {
		json
			("source", source)
			//("ai_source", ai_source)
			("suggestion", suggestions)
			("data", data)
			("outdated_suggestions", outdated_suggestions)
			;
	}
	int GetBestSuggestion() const;
};

struct StaticPart {
	String name;
	String type; // abbreviation like V1, PC2, C
	Vector<String> source; // lines
	Vector<String> ai_source;
	Array<StaticRhyme> rhymes;
	String rhyme_scheme;
	VectorMap<String,String> data;
	String syllable_str;
	bool outdated_suggestions = true;
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("type", type)
			("source", source)
			("ai_source", ai_source)
			("rhymes", rhymes)
			("rhyme_scheme", rhyme_scheme)
			("data", data)
			("outdated_suggestions", outdated_suggestions)
			("syllable_str", syllable_str)
			;
	}
	
	// Temp
	Vector<int> valid_rhyme_schemes;
};



struct StaticSymbolism {
	String text;
	Color clr;
	
	
	void Jsonize(JsonIO& json) {
		json
			("text", text)
			("clr", clr)
			;
	}
};

struct StaticImagery {
	String text;
	Color clr;
	Array<StaticSymbolism> symbolisms;
	
	void Jsonize(JsonIO& json) {
		json
			("text", text)
			("clr", clr)
			("symbolisms", symbolisms)
			;
	}
	
};

struct StaticContentIdea {
	String text;
	Color clr;
	Array<StaticImagery> imageries;
	
	
	void Jsonize(JsonIO& json) {
		json
			("text", text)
			("clr", clr)
			("imageries", imageries)
			;
	}
};

struct StaticAllegoricalDevice {
	Array<StaticContentIdea> contents;
	String text;
	Color clr;
	
	
	void Jsonize(JsonIO& json) {
		json
			("text", text)
			("clr", clr)
			("contents", contents)
			;
	}
};

struct StaticToneSuggestion {
	Array<StaticAllegoricalDevice> allegories;
	String text;
	Color clr;
	
	void Jsonize(JsonIO& json) {
		json
			("text", text)
			("clr", clr)
			("allegories", allegories)
			;
	}
};

struct StaticIdea {
	Array<StaticToneSuggestion> tones;
	String text;
	Color clr;
	
	void Jsonize(JsonIO& json) {
		json
			("text", text)
			("clr", clr)
			("tones", tones)
			;
	}
};

struct StaticTheme {
	Array<StaticIdea> ideas;
	String text;
	Color clr;
	
	void Jsonize(JsonIO& json) {
		json
			("text", text)
			("clr", clr)
			("ideas", ideas)
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
		Vector<int> part_types;
		
		// Part types
		enum {
			SINGING,
			RAPPING,
			POETRY,
			DIALOG,
			SKIP
		};
		
		void operator=(const StructSuggestion& s) {
			name = s.name;
			parts <<= s.parts;
			attrs <<= s.attrs;
			part_types <<= s.part_types;
		}
		void Clear() {
			name.Clear();
			parts.Clear();
			attrs.Clear();
			part_types.Clear();
		}
		int GetEstimatedDuration(int bpm) const;
		void Jsonize(JsonIO& json) {
			json
				("name", name)
				("parts", parts)
				("attrs", attrs)
				("part_types", part_types)
				;
		}
	};
	
	// Public
	String						artist;
	String						native_title;
	String						english_title;
	String						prj_name;
	String						structure_str;
	Vector<String>				structure;
	MArr<String>				content;
	VectorMap<String,String>	data;
	Array<StaticPart>			parts;
	Array<StructSuggestion>		struct_suggs;
	Array<StaticTheme>			themes;
	StructSuggestion			active_struct;
	int							default_line_syllables = 0;
	int							default_attr_count = 7;
	
	// Temp
	Pipe*				pipe = 0;
	
	//Song();
	/*Impact* FindImpact(String impact_txt);
	Impact* FindImpactByText(String txt);
	Impact& GetAddImpact(String impact_txt, String brk_txt);
	void RealizeImpacts();*/
	void Store();
	void LoadTitle(String title);
	void ReloadStructure();
	void Serialize(Stream& s) {
		TODO
		/*s	% artist
			% title
			% prj_name
			% structure_str
			% structure
			% content
			
			% parts
			% tracks
			% headers
			% rev_impact
			% rev_common_mask_tasks
			% rev_separate_mask_tasks
			% rev_pattern_tasks
			;
		SnapContext::Serialize(s);*/
	}
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
			("content", content)
			("data", data)
			("parts", parts)
			("active_struct", active_struct)
			("struct_suggs", struct_suggs)
			("themes", themes)
			;
		
		//for(const SnapArg& a : HumanInputTextArgs())
		//	json("headers[" + a.SubscriptString() + "]", headers[a]);
		/*for(const SnapArg& a : AllArgs())
			json("headers[" + a.SubscriptString() + "]", headers[a]);
		
		// rev_pattern_tasks
		if (json.IsLoading()) {
			FixPtrs();
			
			{
				String hash;
				json("rev_impact", hash);
				rev_impact.LoadHash(StrInt64(hash));
			}{
				Vector<String> hashes;
				json("rev_common_mask_tasks", hashes);
				for (String h : hashes) rev_common_mask_tasks.Add().LoadHash(StrInt64(h));
			}{
				Vector<String> hashes;
				json("rev_separate_mask_tasks", hashes);
				for (String h : hashes) rev_separate_mask_tasks.Add().LoadHash(StrInt64(h));
			}{
				Vector<String> hashes;
				json("rev_pattern_tasks", hashes);
				for (String h : hashes) rev_pattern_tasks.Add().LoadHash(StrInt64(h));
			}
			RealizeTaskSnaps();
		}
		else {
			{
				String hash;
				rev_impact.Store();
				hash = IntStr64(rev_impact.GetHashValue());
				json("rev_impact", hash);
			}{
				Vector<String> hashes;
				for (ReverseTask& t : rev_common_mask_tasks) {t.Store(); hashes.Add(IntStr64(t.GetHashValue()));}
				json("rev_common_mask_tasks", hashes);
			}{
				Vector<String> hashes;
				for (ReverseTask& t : rev_separate_mask_tasks) {t.Store(); hashes.Add(IntStr64(t.GetHashValue()));}
				json("rev_separate_mask_tasks", hashes);
			}{
				Vector<String> hashes;
				for (ReverseTask& t : rev_pattern_tasks) {t.Store(); hashes.Add(IntStr64(t.GetHashValue()));}
				json("rev_pattern_tasks", hashes);
			}
		}
		SnapContext::Jsonize(json);*/
	}
	void FixPtrs() {
		{
			EditorPtrs::release = EditorPtrs::release;
			EditorPtrs::artist = EditorPtrs::artist;
			EditorPtrs::song = this;
		}
	}
	/*void MergeOwner() {
		for (Part& p : parts)
			p.MergeOwner();
	}*/
	/*String GetStructuredText(bool pretty, int indent = 0) const {
		String s;
		for(const Part& p : parts) {
			if (pretty) {
				s.Cat('\t', indent);
				s	<< "part " << p.name << " {\n";
				s	<< p.SnapContext::GetStructuredText(pretty, indent+1);
				s	<< "}\n";
				s.Cat('\t', indent);
			}
			else {
				s	<< "part " << p.name << "{";
				s	<< p.SnapContext::GetStructuredText(pretty, indent+1);
				s	<< "}";
			}
		}
		return s;
	}*/
};



#endif
