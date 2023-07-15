#ifndef _SongTool_Song_h_
#define _SongTool_Song_h_


struct Song : DataFile {
	/*struct Attr : Moveable<Attr> {
		String group, item;
		int group_i = -1, item_i = -1;
		
		void Jsonize(JsonIO& json) {
			json
				("group", group)
				("item", item)
				;
			if (json.IsLoading() || (json.IsStoring() && group_i >= 0)) {
				json
					("group_i", group_i)
					("item_i", item_i)
					;
			}
		}
	};*/
	struct Line : Moveable<Line> {
		String line;
		
		void Jsonize(JsonIO& json) {json("line", line);}
	};
	
	String			artist;
	String			title;
	String			prj_name;
	String			structure_str;
	VectorMap<String,String> data;
	
	// Imported lyrics
	String									content;
	VectorMap<String, Vector<SnapAttrStr>>	unique_lines;
	Vector<String>							structure;
	ArrayMap<String, Part>					parts;
	
	// Local data only (not shared)
	Array<ReverseTask>						rev_tasks;
	
	RWMutex			lock;
	
	void Store();
	void LoadTitle(String title);
	void ReloadStructure();
	void RealizeTaskSnaps(bool force=false);
	void Jsonize(JsonIO& json) {
		json
			("artist", artist)
			("title", title)
			("prj_name", prj_name)
			("structure_str", structure_str)
			("data", data)
			
			("content", content)
			("unique_lines", unique_lines)
			("structure", structure)
			("parts", parts)
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
	}
	void FixPtrs() {
		for(int i = 0; i < parts.GetCount(); i++) {
			Part& p = parts[i];
			p.name = parts.GetKey(i);
			p.snap.SetId(i);
			p.rev_snap.SetId(i);
			p.FixPtrs();
		}
	}
	bool operator()(const Song& a, const Song& b) const {
		return a.file_title < b.file_title;
	}
	void GetSnapsLevel(int level, Vector<PatternSnap*>& level_snaps) {
		for (Part& p : parts.GetValues())
			p.snap.GetSnapsLevel(level, level_snaps);
	}
	void GetAttributes(Index<SnapAttrStr>& attrs) const {
		for (const Part& p : parts.GetValues())
			p.snap.GetAttributes(attrs);
	}
	void MergeOwner() {
		for (Part& p : parts.GetValues())
			p.snap.MergeOwner();
	}
	String GetStructuredText(bool pretty, int indent = 0) const {
		String s;
		for(const Part& p : parts.GetValues()) {
			if (pretty) {
				s.Cat('\t', indent);
				s	<< "part " << p.name << " {\n";
				s	<< p.snap.GetStructuredText(pretty, indent+1);
				s	<< "}\n";
				s.Cat('\t', indent);
			}
			else {
				s	<< "part " << p.name << "{";
				s	<< p.snap.GetStructuredText(pretty, indent+1);
				s	<< "}";
			}
		}
		return s;
	}
	void GetLineSnapshots(const String& txt_line, Vector<PatternSnap*>& snaps) {
		ASSERT(!txt_line.IsEmpty());
		for (Part& p : parts.GetValues())
			p.snap.GetLineSnapshots(txt_line, snaps);
	}
};


#endif
