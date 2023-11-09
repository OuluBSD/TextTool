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
	struct RhymeLocationLine : Moveable<RhymeLocationLine> {
		Vector<String> words;
		Vector<bool> rhyming;
		
		void Serialize(Stream& s) {
			s % words % rhyming;
		}
		void Jsonize(JsonIO& json) {
			json
				("words", words)
				("rhyming", rhyming)
				;
		}
	};
	String name;
	Vector<Rhyme> rhymes;
	VectorMap<String, Vector<String>> word_groups;
	Vector<Vector<Role>> positive_roles, negative_roles;
	Vector<RhymeLocationLine> rhyme_locations;
	
	void Serialize(Stream& s){
		s % name % rhymes % word_groups % positive_roles % negative_roles % rhyme_locations;
	}
	
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("rhymes", rhymes)
			("word_groups", word_groups)
			("positive_roles", positive_roles)
			("negative_roles", negative_roles)
			("rhyme_locations", rhyme_locations)
			;
	}
	String AsString() const;
};

struct ArtistAnalysis : Moveable<ArtistAnalysis> {
	int total_words = 0;
	VectorMap<String, int> word_counts;
	Array<LyricsAnalysis> songs;
	
	void Jsonize(JsonIO& json) {
		json
			("total_words", total_words)
			("word_counts", word_counts)
			("songs", songs)
			;
	}
	void Serialize(Stream& s) {
		s % total_words % word_counts % songs;
	}
};

struct WordGroupAnalysis {
	VectorMap<String, int> values;
	bool is_wordlist = false;
	
	void Jsonize(JsonIO& json) {
		json
			("values", values)
			("is_wordlist", is_wordlist)
			;
	}
	void Serialize(Stream& s) {
		s % values % is_wordlist;
	}
};

struct WordAnalysis : Moveable<WordAnalysis> {
	int count = 0;
	Index<int> group_is;
	
	void Jsonize(JsonIO& json) {
		json
			("count", count)
			("group_is", group_is)
			;
	}
	void Serialize(Stream& s) {
		s % count % group_is;
	}
};

struct DatasetAnalysis {
	ArrayMap<String, ArtistAnalysis> artists;
	ArrayMap<String, WordGroupAnalysis> groups;
	ArrayMap<String, WordAnalysis> words;
	
	void Jsonize(JsonIO& json) {
		json
			("artists", artists)
			("groups", groups)
			("word", words)
			;
	}
	
	
	void Serialize(Stream& s) {
		s % artists % groups % words;
	}
};

struct SongDataAnalysis {
	ArrayMap<String, DatasetAnalysis> datasets;
	
	void Jsonize(JsonIO& json) {
		json
			("datasets", datasets)
			;
	}
	
	
	void Serialize(Stream& s) {
		s % datasets;
	}
	void StoreJson();
	void LoadJson();
	void Store();
	void Load();
};

struct SongData {
	// Binary data
	Vector<ArtistDataset> artists_en;
	Vector<ArtistDataset> artists_fi;
	
	SongDataAnalysis a;
	
	
	int GetCount() const {return 2;}
	Vector<ArtistDataset>& operator[](int i) {
		switch (i) {
			case 0: return artists_en;
			case 1: return artists_fi;
			default: Panic("error");
		}
		return Single<Vector<ArtistDataset>>();
	}
	String GetKey(int i) const {
		switch (i) {
			case 0: return "en";
			case 1: return "fi";
			default: Panic("error");
		}
		return "";
	}
	void Store();
	void Load();
	void Serialize(Stream& s);
	bool IsEmpty() const {return artists_en.IsEmpty() || artists_fi.IsEmpty();}
	
};


#endif
