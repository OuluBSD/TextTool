#ifndef _SongDatabase_DatabaseClass_h_
#define _SongDatabase_DatabaseClass_h_

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
	Array<TaskMgr>	pipes;
	SongData		song_data;
	
	// Temp
	String			dir;
	RWMutex			lock;
	
	Database();
	Array<Artist>& GetSub() {return artists;}
	const Array<Artist>& GetSub() const {return artists;}
	void Clear() {artists.Clear(); }
	void Store();
	void Load();
	void FindOrphaned();
	void Serialize(Stream& s) {
		s	% artists
			% translation
			/*% attrs
			% attrscores*/;
	}
	void Jsonize(JsonIO& json) {
		json ("translation", translation);
		if (json.IsStoring()) {
			Vector<String> names;
			for (Artist& a : artists) {a.Store(); names.Add(a.file_title);}
			json("artists", names);
		}
		if (json.IsLoading()) {
			String lng = GetCurrentLanguageString().Left(5);
			trans_i = translation.FindAdd(lng);
			
			Vector<String> names;
			json("artists", names);
			for (String n : names) artists.Add().LoadTitle(n);
			Sort(artists, Artist());
		}
	}
	String GetArtistsDir() const;
	String GetReleasesDir() const;
	String GetSongsDir() const;
	
	String Translate(const String& s);
	
	static Database& Single() {static Database db; return db;}
	
	static int trans_i; // active language translation index
	
};







#endif
