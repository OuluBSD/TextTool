#ifndef _SongTool_Song_h_
#define _SongTool_Song_h_


struct Song :
	DataFile,
	PatternSnap
{
	int										uniq = 0;
	int										linked_uniq = 0;
	String									artist;
	String									title;
	String									prj_name;
	String									structure_str;
	VectorMap<String,String>				data;
	String									content;
	VectorMap<String, Vector<SnapAttrStr>>	unique_lines;
	Vector<String>							structure;
	Array<Part>								parts;
	Array<Track>							tracks;
	
	// Local data only (not shared)
	Array<ReverseTask>						rev_tasks;
	
	bool									reversed = false;
	RWMutex									lock;
	
	Song();
	void Store();
	void LoadTitle(String title);
	void ReloadStructure();
	void RealizeProduction();
	void RealizeTaskSnaps(bool force=false);
	Song& SetReversed() {reversed = true; return *this;}
	void Jsonize(JsonIO& json) {
		json
			("uniq", uniq)
			("linked_uniq", linked_uniq)
			("artist", artist)
			("title", title)
			("prj_name", prj_name)
			("structure_str", structure_str)
			("data", data)
			("content", content)
			("unique_lines", unique_lines)
			("structure", structure)
			("parts", parts)
			("tracks", tracks)
			;
		
		// rev_tasks
		if (json.IsLoading()) {
			FixPtrs();
			
			Vector<String> hashes;
			json("rev_tasks", hashes);
			for (String h : hashes) rev_tasks.Add().LoadHash(StrInt64(h));
			
			RealizeTaskSnaps();
		}
		else {
			Vector<String> hashes;
			for (ReverseTask& t : rev_tasks) {t.Store(); hashes.Add(IntStr64(t.GetHashValue()));}
			json("rev_tasks", hashes);
		}
		PatternSnap::Jsonize(json);
	}
	void FixPtrs() {
		this->song = this;
		int id = 0;
		for (Part& p : parts) {
			static_cast<Ptrs&>(p) = *(Ptrs*)this;
			p.owner = this;
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
				s	<< p.PatternSnap::GetStructuredText(pretty, indent+1);
				s	<< "}\n";
				s.Cat('\t', indent);
			}
			else {
				s	<< "part " << p.name << "{";
				s	<< p.PatternSnap::GetStructuredText(pretty, indent+1);
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
	
	
	PATTERNMASK_MACROS
};


#endif
