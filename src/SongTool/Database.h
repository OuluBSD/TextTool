#ifndef _SongTool_Database_h_
#define _SongTool_Database_h_


struct Database {
	Array<Artist>	artists;
	Attributes		attrs;
	AttrScore		attrscores;
	
	Context			ctx;
	String			dir;
	
	Database();
	Array<Artist>& GetSub() {return artists;}
	const Array<Artist>& GetSub() const {return artists;}
	void Clear() {artists.Clear();}
	void Store();
	void Load();
	void RealizeAttrIds();
	void FindOrphaned();
	void Jsonize(JsonIO& json) {
		/*json
			;*/
		if (json.IsStoring()) {
			Vector<String> names;
			for (Artist& a : artists) {a.Store(); names.Add(a.file_title);}
			json("artists", names);
			attrscores.Store();
			attrs.Store();
		}
		if (json.IsLoading()) {
			Vector<String> names;
			json("artists", names);
			for (String n : names) artists.Add().LoadTitle(n);
			Sort(artists, Artist());
			//FindOrphaned();
			attrs.Load();
			attrscores.Load();
			
			if (attrs.groups.IsEmpty())
				for (Artist& a : artists)
					a.ClearAttrs();
		}
	}
	String GetArtistsDir() const;
	String GetReleasesDir() const;
	String GetSongsDir() const;
	int GetActiveScoreGroupIndex() const {return VectorFindPtr(ctx.active_scoregroup, attrscores.groups);};
	
	static Database& Single() {static Database db; return db;}
	
};









template <class B>
inline String PatternSnap::GetStructuredText(const SnapArg& a, bool pretty, int indent, const Array<B>& sub) const {
	const Attributes& g = Database::Single().attrs;
	String s;
	if (pretty) s.Cat('\t', indent);
	if (song && !part) {
		s << "song";
	}
	else if (part && !line) {
		s << "part " << part->name;
	}
	else if (line && !brk) {
		
		String storyline = /*part->snap[mode].*/data.Get("storyline", "");
		
		s << "line(" << id;
		
		if (storyline.GetCount()) {
			s << ", storyline: \"" << storyline << "\"";
		}
		
		s << ")";
	}
	else if (brk) {
		//String impact = /*brk->snap[mode].*/impact;
		if (impact.GetCount()) {
			s << "(impact: \"" << impact << "\") ";
		}
	}
	s << "{";
	if (pretty) s << "\n";
	int i = 0;
	Index<int> used_groups;
	for (const SnapAttrStr& sa : /*this->snap[mode].*/attributes.GetKeys()) {
		used_groups.FindAdd(sa.group_i);
	}
	
	for (int group_i : used_groups.GetKeys()) {
		const Attributes::Group& gg = g.groups[group_i];
		if (pretty) s.Cat('\t', indent+1);
		s << ToLower(gg.description) << " {";
		if (pretty) s << "\n";
		for (const SnapAttrStr& sa : /*this->snap[mode].*/attributes.GetKeys()) {
			ASSERT(sa.has_id);
			if (sa.group_i != group_i)
				continue;
			if (pretty) s.Cat('\t', indent+2);
			s	<< ToLower(gg.values[sa.item_i])
				<< ";"
				;
			if (pretty) s << "\n";
			i++;
		}
		if (pretty) s.Cat('\t', indent+1);
		s << "}";
		if (pretty) s << "\n";
	}
	for (const B& o : sub)
		s << o.GetStructuredText(a, pretty, indent+1);
	if (pretty) s.Cat('\t', indent);
	s << "}";
	if (pretty) s << "\n";
	return s;
}

template <class T>
void CalculateWeightedGenderDifference(Vector<double>& values, const Vector<T>& score, const Vector<T>& other_score) {
	Attributes& g = Database::Single().attrs;
	int c = g.scorings.GetCount();
	
	ASSERT(score.GetCount() == c);
	ASSERT(other_score.GetCount() == c);
	
	int c0 = min(c, min(
		other_score.GetCount(),
		score.GetCount()));
	
	if (0) {
		double av = 0;
		for (auto& val : score) av += val;
		av /= score.GetCount();
		
		double other_av = 0;
		for (auto& val : other_score) other_av += val;
		other_av /= other_score.GetCount();
		
		for(int k = 0; k < c0; k++) {
			double av_diff = score[k] - av;
			double other_av_diff = other_score[k] - other_av;
			
			values[k] = (
				av_diff -
				other_av_diff
				);
		}
	}
	else {
		double av = 0;
		for (auto& val : score) av += fabs(val);
		av /= score.GetCount();
		
		double other_av = 0;
		for (auto& val : other_score) other_av += fabs(val);
		other_av /= other_score.GetCount();
		
		double f = av / other_av;
		
		for(int k = 0; k < c0; k++) {
			double val = score[k];
			double other_weighted_val = other_score[k] * f;
			
			values[k] = (
				val -
				other_weighted_val
				);
		}
	}

}

#endif
