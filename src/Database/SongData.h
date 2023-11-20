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

struct LyricsAnalysis : Moveable<LyricsAnalysis> {
	struct Phrase : Moveable<Phrase> {
		String phrase, group, value;
		Color clr;
		union {
			hash_t tmpl_hash;
			int tmpl_i32[2];
		};
		
		void Serialize(Stream& s) {s % phrase % group % value % clr % tmpl_hash;}
		void Jsonize(JsonIO& json) {
			json
				("phrase", phrase)
				("group", group)
				("value", value)
				("clr", clr)
				("tmpl0", tmpl_i32[0])
				("tmpl1", tmpl_i32[1])
				;
		}
	};
	struct Role : Moveable<Role> {
		String group, value;
		int percent;
		
		Role() {}
		Role(const Role& r) {*this = r;}
		void operator=(const Role& r) {group = r.group; value = r.value; percent = r.percent;}
		void Serialize(Stream& s) {s % group % value % percent;}
		void Jsonize(JsonIO& json) {
			json
				("group", group)
				("value", value)
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
	Vector<Vector<Role>> positive_roles, negative_roles;
	Vector<Phrase> phrases;
	
	VectorMap<String, Vector<String>> word_groups; // UNUSED
	Vector<RhymeLocationLine> rhyme_locations; // UNUSED
	
	void Serialize(Stream& s){
		s % name % phrases % word_groups % positive_roles % negative_roles % rhyme_locations;
	}
	
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("phrases", phrases)
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

struct WordGroupAnalysis : Moveable<WordGroupAnalysis> {
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
	//Index<int> group_is;
	union {
		hash_t hash;
		int i32[2];
	};
	int count = 0;
	String txt;
	String main_class;
	String translation;
	String spelling;
	WString phonetic;
	Color clr, main_class_clr = Black();
	
	void Jsonize(JsonIO& json) {
		json
			("hash0", i32[0])
			("hash1", i32[1])
			("txt", txt)
			("count", count)
			//("group_is", group_is)
			("main_class", main_class)
			("translation", translation)
			("clr", clr)
			("main_class_clr", main_class_clr)
			("spelling", spelling)
			("phonetic", phonetic)
			;
	}
	void Serialize(Stream& s) {
		s % hash % txt % count % main_class % translation % clr % main_class_clr % spelling % phonetic;
	}
};

struct SongId : Moveable<SongId> {
	byte dataset;
	uint16 artist;
	uint16 song;
	
	void Jsonize(JsonIO& json) {
		int dataset = this->dataset;
		int artist = this->artist;
		int song = this->song;
		json
			("dataset", dataset)
			("artist", artist)
			("song", song)
			;
		this->dataset = dataset;
		this->artist = artist;
		this->song = song;
	}
	void Serialize(Stream& s) {
		s % dataset % artist % song;
	}
};

struct PhraseAnalysis : Moveable<PhraseAnalysis> {
	Vector<SongId> songs;
	Vector<int> word_ids;
	Color clr;
	
	void Jsonize(JsonIO& json) {
		json
			("songs", songs)
			("word_ids", word_ids)
			("clr", clr)
			;
	}
	void Serialize(Stream& s) {
		s % songs % word_ids % clr;
	}
};

struct VirtualPhraseAnalysis : Moveable<VirtualPhraseAnalysis> {
	Vector<int> group_ids;
	Index<int> phrases;
	
	void Jsonize(JsonIO& json) {
		json
			("group_ids", group_ids)
			("phrases", phrases)
			;
	}
	void Serialize(Stream& s) {
		s % group_ids % phrases;
	}
};

struct TemplatePhrase : Moveable<TemplatePhrase> {
	union {
		hash_t hash;
		int i32[2];
	};
	Vector<String> parts;
	Vector<Vector<String>> words;
	
	// From the first LyricsAnalysis::Phrase
	String group, value;
	Color clr;
	
	void Jsonize(JsonIO& json) {
		json
			("hash0", i32[0])
			("hash1", i32[1])
			("parts", parts)
			("words", words)
			("group", group)
			("value", value)
			("clr", clr)
			;
	}
	void Serialize(Stream& s) {
		s % hash % parts % words % group % value % clr;
	}
};

struct DatasetAnalysis {
	VectorMap<String, ArtistAnalysis> artists;
	VectorMap<String, WordGroupAnalysis> groups;
	Vector<WordAnalysis> words;
	Vector<TemplatePhrase> tmpl_phrases;
	
	// deprecated
	VectorMap<String, PhraseAnalysis> unique_phrases;
	VectorMap<String, VirtualPhraseAnalysis> virtual_phrases;
	
	int FindWord(const String& w) const {
		int i = 0;
		hash_t h = w.GetHashValue();
		for (const WordAnalysis& wa : words) {
			if (wa.hash == h)
				return i;
			i++;
		}
		return -1;
	}
	WordAnalysis& GetAddWord(const String& w) {
		int i = FindWord(w);
		if (i >= 0) return words[i];
		WordAnalysis& wa = words.Add();
		wa.hash = w.GetHashValue();
		wa.txt = w;
		return wa;
	}
	void Jsonize(JsonIO& json) {
		/*	
		if (json.IsLoading()) {
			VectorMap<String, WordAnalysis> tmp;
			json
				("artists", artists)
				("groups", groups)
				("word", tmp)
				("tmpl_phrases", tmpl_phrases)
				("unique_phrases", unique_phrases)
				("virtual_phrases", virtual_phrases)
				;
			words.SetCount(tmp.GetCount());
			for(int i = 0; i < tmp.GetCount(); i++) {
				String key = tmp.GetKey(i);
				WordAnalysis& src = tmp[i];
				WordAnalysis& dst = words[i];
				Swap(src, dst);
				dst.txt = key;
				dst.hash = key.GetHashValue();
			}
		}
		else*/ {
			json
				("artists", artists)
				("groups", groups)
				("word", words)
				("tmpl_phrases", tmpl_phrases)
				("unique_phrases", unique_phrases)
				("virtual_phrases", virtual_phrases)
				;
		}
	}
	
	
	void Serialize(Stream& s) {
		s % artists % groups % words % tmpl_phrases % unique_phrases % virtual_phrases;
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
