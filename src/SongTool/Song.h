#ifndef _SongTool_Song_h_
#define _SongTool_Song_h_


struct SongHeader {
	VectorMap<String,String>				data;
	String									content;
	VectorMap<String, Vector<SnapAttrStr>>	unique_lines;
	
	void Jsonize(JsonIO& json) {
		json
			("data", data)
			("content", content)
			("unique_lines", unique_lines)
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
	
	// Local data only (not shared)
	Array<ReverseTask>	rev_tasks;
	
	RWMutex				lock;
	
	//Song();
	void Store();
	void LoadTitle(String title);
	void ReloadStructure();
	void RealizeProduction();
	void RealizeTaskSnaps(bool force=false);
	void Jsonize(JsonIO& json) {
		json
			("artist", artist)
			("title", title)
			("prj_name", prj_name)
			("structure_str", structure_str)
			("structure", structure)
			("parts", parts)
			("tracks", tracks)
			;
		
		for(int i = 0; i < PTR_COUNT; i++)
			json("headers[" + IntStr(i) + "]", headers[i]);
		
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
