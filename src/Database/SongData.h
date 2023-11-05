#ifndef _Database_SongData_h_
#define _Database_SongData_h_


struct LyricsDataset : Moveable<LyricsDataset> {
	String name;
	String text;
	
	void Serialize(Stream& s) {
		s % name % text;
	}
};

struct ArtistDataset : Moveable<ArtistDataset> {
	String name;
	Vector<LyricsDataset> lyrics;
	
	void Serialize(Stream& s) {
		s % name % lyrics;
	}
};

struct LyricsAnalysis {
	struct Rhyme : Moveable<Rhyme> {
		Vector<String> words;
		int score;
		
		void Serialize(Stream& s) {s % words % score;}
		void Jsonize(JsonIO& json) {
			json
				("words", words)
				("score", score)
				;
		}
	};
	struct Role : Moveable<Role> {
		String subject;
		int percent;
		
		void Serialize(Stream& s) {s % subject % percent;}
		void Jsonize(JsonIO& json) {
			json
				("subject", subject)
				("percent", percent)
				;
		}
	};
	
	String name;
	Vector<Rhyme> rhymes;
	VectorMap<String, Vector<String>> word_groups;
	Vector<Vector<Role>> positive_roles, negative_roles;
	
	void Serialize(Stream& s) {s % name % rhymes % word_groups % positive_roles % negative_roles;}
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("rhymes", rhymes)
			("word_groups", word_groups)
			("positive_roles", positive_roles)
			("negative_roles", negative_roles)
			;
	}
	String AsString() const;
};

struct SongData {
	// Binary data
	Vector<ArtistDataset> artists_en;
	Vector<ArtistDataset> artists_fi;
	
	// Json data
	ArrayMap<String, Array<LyricsAnalysis>> active_songs;
	
	void Jsonize(JsonIO& json) {
		#if 0
		if (json.IsLoading()) {
			VectorMap<String, Vector<String>> m;
			json
				("active_songs", m)
				;
			for(int i = 0; i < m.GetCount(); i++) {
				String key = m.GetKey(i);
				const auto& v = m[i];
				auto& ov = active_songs.Add(key);
				for(int j = 0; j < v.GetCount(); j++) {
					ov.Add().name = v[j];
				}
			}
		}
		else
		#endif
		{
			json
				("active_songs", active_songs)
				;
		}
	}
	void Store();
	void Load();
	void StoreJson();
	void LoadJson();
	void Serialize(Stream& s);
	bool IsEmpty() const {return artists_en.IsEmpty() || artists_fi.IsEmpty();}
	
};


#endif
