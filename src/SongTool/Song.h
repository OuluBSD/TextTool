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

struct Song :
	DataFile,
	EditorPtrs
{
	struct Suggestion {
		String style, content;
		int score = 0;
		void Jsonize(JsonIO& json) {
			json
				("style", style)
				("content", content)
				("score", score)
				;
		}
	};
	struct Rhyme {
		Vector<String> source;
		Array<Suggestion> suggestions;
		VectorMap<String,String> data;
		bool outdated_suggestions = true;
		void Jsonize(JsonIO& json) {
			json
				("source", source)
				("suggestion", suggestions)
				("data", data)
				("outdated_suggestions", outdated_suggestions)
				;
		}
	};
	struct SongPart {
		String name;
		Vector<String> source; // lines
		Array<Rhyme> rhymes;
		String rhyme_scheme;
		VectorMap<String,String> data;
		bool outdated_suggestions = true;
		void Jsonize(JsonIO& json) {
			json
				("name", name)
				("source", source)
				("rhymes", rhymes)
				("rhyme_scheme", rhyme_scheme)
				("data", data)
				("outdated_suggestions", outdated_suggestions)
				;
		}
		
		// Temp
		Vector<int> valid_rhyme_schemes;
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
	Array<SongPart>				parts;
	
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
			("structure_str", structure_str)
			("structure", structure)
			("content", content)
			("data", data)
			("parts", parts)
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
