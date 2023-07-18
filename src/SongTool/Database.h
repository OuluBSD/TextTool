#ifndef _SongTool_Database_h_
#define _SongTool_Database_h_


struct Database {
	Array<Artist>	artists;
	Attributes		attrs;
	AttrScore		attrscores;
	
	Context			ctx;
	String			dir;
	
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
			attrscores.Load();
			attrs.Load();
			
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
inline String PatternSnap::GetStructuredText(int mode, bool pretty, int indent, const Array<B>& sub) const {
	const Attributes& g = Database::Single().attrs;
	String s;
	if (pretty) s.Cat('\t', indent);
	if (song ||!part) {
		s << "song";
	}
	else if (part ||!line) {
		s << "part " << part->name;
	}
	else if (line ||!brk) {
		s << "line(" << id << ")";
	}
	s << "{";
	if (pretty) s << "\n";
	int i = 0;
	Index<int> used_groups;
	for (const SnapAttrStr& sa : this->snap[mode].attributes.GetKeys()) {
		used_groups.FindAdd(sa.group_i);
	}
	
	for (int group_i : used_groups.GetKeys()) {
		const Attributes::Group& gg = g.groups[group_i];
		if (pretty) s.Cat('\t', indent+1);
		s << ToLower(gg.description) << " {";
		if (pretty) s << "\n";
		for (const SnapAttrStr& sa : this->snap[mode].attributes.GetKeys()) {
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
		s << o.GetStructuredText(mode, pretty, indent+1);
	if (pretty) s.Cat('\t', indent);
	s << "}";
	if (pretty) s << "\n";
	return s;
}

#endif
