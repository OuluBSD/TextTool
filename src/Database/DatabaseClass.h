#ifndef _SongTool_DatabaseClass_h_
#define _SongTool_DatabaseClass_h_

struct Translation : Moveable<Translation> {
	VectorMap<String, String> data;
	
	Translation& Add(String key, String value) {data.GetAdd(key) = value; return *this;}
	void Serialize(Stream& s) {
		s % data;
	}
	void Jsonize(JsonIO& json) {
		if (json.IsStoring())
			SortByKey(data, StdLess<String>());
		json
			("data", data)
			;
		if (json.IsLoading()) {
			VectorMap<String, String> tmp;
			for(int i = 0; i < data.GetCount(); i++)
				tmp.Add(ToLower(data.GetKey(i)), ToLower(data[i]));
			Swap(tmp, data);
		}
	}
};



struct Database {
	// Public
	Array<Artist>	artists;
	VectorMap<String, Translation> translation;
	
	// Local (separate)
	Array<Pipe>		pipes;
	SongData		song_data;
	
	// Temp
	Context			ctx;
	String			dir;
	RWMutex			lock;
	
	Database();
	Array<Artist>& GetSub() {return artists;}
	const Array<Artist>& GetSub() const {return artists;}
	void Clear() {artists.Clear();}
	void Store();
	void Load();
	//void RealizeAttrIds();
	void FindOrphaned();
	void Serialize(Stream& s) {
		s	% artists
			% translation
			/*% attrs
			% attrscores*/;
	}
	void Jsonize(JsonIO& json) {
		//if (json.IsLoading())
		//	json("song_data", song_data);
		json ("translation", translation);
		if (json.IsStoring()) {
			Vector<String> names;
			for (Artist& a : artists) {a.Store(); names.Add(a.file_title);}
			json("artists", names);
			//attrscores.Store();
			//attrs.Store();
		}
		if (json.IsLoading()) {
			String lng = GetCurrentLanguageString().Left(5);
			trans_i = translation.FindAdd(lng);
			
			Vector<String> names;
			json("artists", names);
			for (String n : names) artists.Add().LoadTitle(n);
			Sort(artists, Artist());
			//FindOrphaned();
			/*
			attrs.Load();
			attrscores.Load();
			if (attr_groups.IsEmpty())
				for (Artist& a : artists)
					a.ClearAttrs();*/
		}
	}
	String GetArtistsDir() const;
	String GetReleasesDir() const;
	String GetSongsDir() const;
	
	String Translate(const String& s);
	
	static Database& Single() {static Database db; return db;}
	
	static int trans_i; // active language translation index
	
};









template <class B>
inline String PatternSnap::GetStructuredText(const SnapArg& a, bool pretty, int indent, const Array<B>& sub) const {
	const Attributes& g = *pipe;
	String s;
	if (pretty) s.Cat('\t', indent);
	if (pipe && !part) {
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
		const Attr::Group& gg = g.attr_groups[group_i];
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
	TODO
	#if 0
	Attributes& g = *snap.pipe;
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
	#endif
}

#endif
