#ifndef _SongTool_Database_h_
#define _SongTool_Database_h_


struct Database {
	Array<Artist>	artists;
	Attributes		attrs;
	
	Artist*			active_artist = 0;
	Release*		active_release = 0;
	Song*			active_song = 0;
	Part*			active_part = 0;
	PatternSnap*	active_snap = 0;
	PartScore*		active_partscore = 0;
	
	String			dir;
	
	void Clear() {artists.Clear();}
	void Store();
	void Load();
	void FindOrphaned();
	void Jsonize(JsonIO& json) {
		json
			("attributes", attrs)
			;
		if (json.IsStoring()) {
			Vector<String> names;
			for (Artist& a : artists) {a.Store(); names.Add(a.file_title);}
			json("artists", names);
		}
		if (json.IsLoading()) {
			Vector<String> names;
			json("artists", names);
			for (String n : names) artists.Add().LoadTitle(n);
			Sort(artists, Artist());
			//FindOrphaned();
		}
	}
	String GetArtistsDir() const;
	String GetReleasesDir() const;
	String GetSongsDir() const;
	int GetActiveArtistIndex() const {return VectorFindPtr(active_artist, artists);}
	int GetActiveReleaseIndex() const {if (!active_artist) return -1; return VectorFindPtr(active_release, active_artist->releases);}
	int GetActiveSongIndex() const {if (!active_release) return -1; return VectorFindPtr(active_song, active_release->songs);}
	int GetActivePartIndex() const {if (!active_song) return -1; return VectorFindPtr(active_part, active_song->parts);}
	
	static Database& Single() {static Database db; return db;}
	
};


#endif
