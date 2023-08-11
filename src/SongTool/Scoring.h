#ifndef _SongTool_Scoring_h_
#define _SongTool_Scoring_h_


struct AttrScoreGroup {
	Vector<SnapAttrStr> attrs;
	Vector<int> scores;
	String name;
	
	void Serialize(Stream& s) {
		s	% attrs
			% scores
			% name;
	}
	void Jsonize(JsonIO& json) {
		json
			("attrs", attrs)
			("scores", scores)
			("name", name)
			;
		/*if (json.IsLoading()) {
			Clean();
			Realize();
		}*/
	}
	
	void Clean() {
		for(int i = 0; i < attrs.GetCount(); i++) {
			const SnapAttrStr& sa = attrs[i];
			if (attrs[i].group.IsEmpty() || attrs[i].item.IsEmpty())
				attrs.Remove(i--);
		}
	}
	void Clean(Pipe& pipe) {
		Clean();
		for(int i = 0; i < attrs.GetCount(); i++) {
			const SnapAttrStr& sa = attrs[i];
			if (!sa.RealizeId(pipe))
				attrs.Remove(i--);
		}
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
	String ToString(Pipe& pipe) const;
	
};

struct AttrScore : DataFile {
	Array<AttrScoreGroup> score_groups;
	//VectorMap<String, Vector<int>> presets;
	
	// Temp
	Vector<Vector<int>> attr_to_score;
	
	
	AttrScore();
	void Store();
	void Load();
	AttrScoreGroup& GetAdd(const Vector<int>& scores) {
		for (AttrScoreGroup& g : score_groups)
			if (g.Is(scores))
				return g;
		AttrScoreGroup& g = score_groups.Add();
		g.scores <<= scores;
		return g;
	}
	void Clear() {
		score_groups.Clear();
	}
	void Serialize(Stream& s) {
		s	% score_groups
			//% presets
			% attr_to_score;
	}
	void Jsonize(JsonIO& json) {
		json
			("score_groups", score_groups)
			("attr_to_score", attr_to_score)
			//("presets", presets)
		;
		if (json.IsLoading()) {
			for (AttrScoreGroup& g : score_groups)
				g.Clean();
		}
	}
	bool UpdateGroupsToScoring();
	int GetActiveScoreGroupIndex() const;
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
	/*void Jsonize(JsonIO& json) {
		json
			("structure", structure)
			("parts", parts)
			("unique_parts", unique_parts)
			;
	}*/
	/*bool operator()(const PatternScore& a, const PatternScore& b) const {
		return a.file_title < b.file_title;
	}*/
};
#endif

#endif
