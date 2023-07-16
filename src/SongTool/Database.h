#ifndef _SongTool_Database_h_
#define _SongTool_Database_h_


struct Database {
	Array<Artist>	artists;
	Attributes		attrs;
	AttrScore		attrscores;
	
	Ptrs			active, active_rev;
	AttrScoreGroup*	active_scoregroup = 0;
	bool			active_wholesong = false;
	
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
		}
	}
	String GetArtistsDir() const;
	String GetReleasesDir() const;
	String GetSongsDir(bool reversed) const;
	int GetActiveArtistIndex() const {return VectorFindPtr(active.artist, artists);}
	int GetActiveReleaseIndex() const {if (!active.artist) return -1; return VectorFindPtr(active.release, active.artist->releases);}
	int GetActiveSongIndex() const {if (!active.release) return -1; return VectorFindPtr(active.song, active.release->songs);}
	int GetActivePartIndex() const {if (!active.song) return -1; return VectorFindPtr(active.part, active.song->parts);}
	int GetActiveScoreGroupIndex() const {return VectorFindPtr(active_scoregroup, attrscores.groups);}
	
	static Database& Single() {static Database db; return db;}
	
};









template <class B>
inline String PatternSnap::GetStructuredText(bool pretty, int indent, const Array<B>& sub) const {
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
	for (const SnapAttrStr& sa : this->attributes.GetKeys()) {
		used_groups.FindAdd(sa.group_i);
	}
	
	for (int group_i : used_groups.GetKeys()) {
		const Attributes::Group& gg = g.groups[group_i];
		if (pretty) s.Cat('\t', indent+1);
		s << ToLower(gg.description) << " {";
		if (pretty) s << "\n";
		for (const SnapAttrStr& sa : this->attributes.GetKeys()) {
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
		s << o.GetStructuredText(pretty, indent+1);
	if (pretty) s.Cat('\t', indent);
	s << "}";
	if (pretty) s << "\n";
	return s;
}

#endif
