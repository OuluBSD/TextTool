#ifndef _SongTool_Scoring_h_
#define _SongTool_Scoring_h_

/*struct PartScore {
	Vector<Vector<int>> values;
	
	void Jsonize(JsonIO& json) {
		json
			("values", values)
			;
	}
	int GetLen() const {return values.GetCount() ? values[0].GetCount() : 0;}
};*/

struct AttrScoreGroup {
	Vector<SnapAttr> attrs;
	Vector<int> scores;
	String name;
	
	void Jsonize(JsonIO& json) {
		json
			("attrs", attrs)
			("scores", scores)
			("name", name)
			;
		if (json.IsLoading())
			Realize();
	}
	void Realize();
	String GetName() const {
		if (!name.IsEmpty())
			return name;
		String s;
		for (const int& i : scores) {
			switch (i) {
				#if 0
				case 0: s.Cat('0'); break;
				case 1: s.Cat('+'); break;
				case -1: s.Cat('-'); break;
				#else
				case 0: s.Cat('-'); break;
				case 1: s.Cat('^'); break;
				case -1: s.Cat('_'); break;
				#endif
				default: s << IntStr(i); break;
			}
		}
		return s;
	}
	bool Is(const Vector<int>& s) const {
		if (s.GetCount() != scores.GetCount()) return false;
		for(int i = 0; i < s.GetCount(); i++)
			if (s[i] != scores[i])
				return false;
		return true;
	}
	String ToString() const;
	
};

struct AttrScore : DataFile {
	Array<AttrScoreGroup> groups;
	VectorMap<String, Vector<int>> presets;
	
	// Temp
	Vector<Vector<int>> attr_to_score;
	
	
	AttrScore();
	void Store();
	void Load();
	AttrScoreGroup& GetAdd(const Vector<int>& scores) {
		for (AttrScoreGroup& g : groups)
			if (g.Is(scores))
				return g;
		AttrScoreGroup& g = groups.Add();
		g.scores <<= scores;
		return g;
	}
	void Clear() {
		groups.Clear();
	}
	void Jsonize(JsonIO& json) {
		json
			("groups", groups)
			("presets", presets)
		;
	}
	bool UpdateGroupsToScoring();
};

#if 0
struct PatternScore {
	String						structure;
	Vector<String>				parts;
	ArrayMap<String, PartScore>	unique_parts;
	
	void Clear() {
		structure.Clear();
		parts.Clear();
		unique_parts.Clear();
	}
	void Jsonize(JsonIO& json) {
		json
			("structure", structure)
			("parts", parts)
			("unique_parts", unique_parts)
			;
	}
	/*bool operator()(const PatternScore& a, const PatternScore& b) const {
		return a.file_title < b.file_title;
	}*/
};
#endif

#endif
