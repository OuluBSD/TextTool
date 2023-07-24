#ifndef _SongTool_Song_h_
#define _SongTool_Song_h_


struct SongHeader {
	VectorMap<String,String>				data;
	String									content;
	VectorMap<String, Vector<SnapAttrStr>>	unique_lines;
	Analysis								analysis;
	
	void Jsonize(JsonIO& json) {
		json
			("data", data)
			("content", content)
			("unique_lines", unique_lines)
			("analysis", analysis)
			;
	}
};

struct Song :
	DataFile,
	SnapContext
{
	String				artist;
	String				title;
	String				prj_name;
	String				structure_str;
	Vector<String>		structure;
	Array<Part>			parts;
	Array<Track>		tracks;
	SongHeader			headers[PTR_COUNT];
	
	//Array<Impact>		impacts;
	
	// Local data only (not shared)
	Array<ReverseTask>	rev_mask_tasks;
	Array<ReverseTask>	rev_pattern_tasks;
	
	RWMutex				lock;
	
	//Song();
	/*Impact* FindImpact(String impact_txt);
	Impact* FindImpactByText(String txt);
	Impact& GetAddImpact(String impact_txt, String brk_txt);
	void RealizeImpacts();*/
	PatternSnap* FindSnapByImpact(String impact_txt);
	void Store();
	void LoadTitle(String title);
	void ReloadStructure();
	void RealizeProduction();
	void RealizeTaskSnaps(bool force=false);
	int GetPartIdx(const Part& p) const;
	void Jsonize(JsonIO& json) {
		json
			("artist", artist)
			("title", title)
			("prj_name", prj_name)
			("structure_str", structure_str)
			("structure", structure)
			("parts", parts)
			("tracks", tracks)
			//("impacts", impacts)
			;
		
		for(int i = 0; i < PTR_COUNT; i++)
			json("headers[" + IntStr(i) + "]", headers[i]);
		
		// rev_pattern_tasks
		if (json.IsLoading()) {
			FixPtrs();
			
			{
				Vector<String> hashes;
				json("rev_pattern_tasks", hashes);
				for (String h : hashes) rev_pattern_tasks.Add().LoadHash(StrInt64(h));
			}{
				Vector<String> hashes;
				json("rev_mask_tasks", hashes);
				for (String h : hashes) rev_mask_tasks.Add().LoadHash(StrInt64(h));
			}
			RealizeTaskSnaps();
		}
		else {
			{
				Vector<String> hashes;
				for (ReverseTask& t : rev_pattern_tasks) {t.Store(); hashes.Add(IntStr64(t.GetHashValue()));}
				json("rev_pattern_tasks", hashes);
			}{
				Vector<String> hashes;
				for (ReverseTask& t : rev_mask_tasks) {t.Store(); hashes.Add(IntStr64(t.GetHashValue()));}
				json("rev_mask_tasks", hashes);
			}
		}
		SnapContext::Jsonize(json);
	}
	void FixPtrs() {
		SetSongPtr(this);
		int id = 0;
		for (Part& p : parts) {
			p.CopyPtrs(*this);
			p.SetOwner(*this);
			p.SetId(id++);
			p.FixPtrs();
		}
	}
	bool operator()(const Song& a, const Song& b) const {
		return a.file_title < b.file_title;
	}
	/*void MergeOwner() {
		for (Part& p : parts)
			p.MergeOwner();
	}*/
	Array<Part>& GetSub() {return parts;}
	const Array<Part>& GetSub() const {return parts;}
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
	int FindPartIdx(const String& name) const {
		int i = 0;
		for (const Part& p : parts) {
			if (p.name == name)
				return i;
			i++;
		}
		return -1;
	}
	const Part* FindPart(const String& name) const {
		for (const Part& p : parts)
			if (p.name == name)
				return &p;
		return 0;
	}
	Part* FindPart(const String& name) {
		for (Part& p : parts)
			if (p.name == name)
				return &p;
		return 0;
	}
	Part& GetAddPart(String name) {
		Part* ptr = FindPart(name);
		if (ptr) return *ptr;
		Part& p = parts.Add();
		p.name = name;
		FixPtrs();
		return p;
	}
	int GetLength(int mode) const;
	
	PATTERNMASK_MACROS
};



#endif
