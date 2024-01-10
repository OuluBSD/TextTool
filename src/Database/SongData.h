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


#if 0
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

double GetActionMatching(const Vector<ActionHeader>& a, const Vector<ActionHeader>& b);

struct ActionPhrase : Moveable<ActionPhrase> {
	struct Score : Moveable<Score> {
		int scores[SCORE_MODE_COUNT][SCORE_ATTR_COUNT] = {{0,0,0,0,0},{0,0,0,0,0}};
		void Jsonize(JsonIO& json) {
			for(int i = 0; i < SCORE_MODE_COUNT; i++)
				for(int j = 0; j < SCORE_ATTR_COUNT; j++)
					json(GetScoreKey(i,j), scores[i][j]);
		}
		void Serialize(Stream& s) {
			for(int i = 0; i < SCORE_MODE_COUNT; i++)
				for(int j = 0; j < SCORE_ATTR_COUNT; j++)
					s % scores[i][j];
		}
	};
	union {
		hash_t hash;
		int i32[2];
	};
	String txt;
	Vector<ActionHeader> actions;
	Vector<int> next_phrases;
	Vector<Score> next_scores;
	int first_lines = 0;
	
	String GetActionText() const;
	void Jsonize(JsonIO& json) {
		json
			("hash0", i32[0])
			("hash1", i32[1])
			("txt", txt)
			("actions", actions)
			("next_phrases", next_phrases)
			("next_scores", next_scores)
			("first_lines", first_lines)
			;
	}
	void Serialize(Stream& s) {
		s % hash % txt % actions % next_phrases % next_scores % first_lines;
	}
};

struct ActionTemplate : Moveable<ActionTemplate> {
	union {
		hash_t hash;
		int i32[2];
	};
	int tp_i; // TemplatePhrase idx in dataset
	Vector<ActionHeader> actions;
	
	void Jsonize(JsonIO& json) {
		json
			("hash0", i32[0])
			("hash1", i32[1])
			("tp_i", tp_i)
			("actions", actions)
			;
	}
	void Serialize(Stream& s) {
		s % hash % tp_i % actions;
	}
};

struct TemplatePhrase : Moveable<TemplatePhrase> {
	union {
		hash_t hash;
		int i32[2];
	};
	Vector<String> parts;
	Vector<Vector<String>> words;
	byte scores[SCORE_MODE_COUNT][SCORE_ATTR_COUNT] = {{0,0,0,0,0},{0,0,0,0,0}};
	
	// From the first LyricsAnalysis::Phrase
	String group, value;
	Color clr;
	
	String GetText() const;
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
		for(int i = 0; i < SCORE_MODE_COUNT; i++)
			for(int j = 0; j < SCORE_ATTR_COUNT; j++)
				json(GetScoreKey(i,j), scores[i][j]);
	}
	void Serialize(Stream& s) {
		s % hash % parts % words % group % value % clr;
		for(int i = 0; i < SCORE_MODE_COUNT; i++)
			for(int j = 0; j < SCORE_ATTR_COUNT; j++)
				s % scores[i][j];
	}
};

struct Wordnet : Moveable<Wordnet> {
	Vector<String> words;
	String main_class;
	String group, value;
	Color clr;
	int clr_group = -1;
	byte scores[SCORE_MODE_COUNT][SCORE_ATTR_COUNT] = {{0,0,0,0,0},{0,0,0,0,0}};
	
	void Jsonize(JsonIO& json) {
		json
			("words", words)
			("main_class", main_class)
			("group", group)
			("value", value)
			("clr", clr)
			("clr_group", clr_group)
			;
		for(int i = 0; i < SCORE_MODE_COUNT; i++)
			for(int j = 0; j < SCORE_ATTR_COUNT; j++)
				json(GetScoreKey(i,j), scores[i][j]);
	}
	void Serialize(Stream& s) {
		s % words % main_class % group % value % clr % clr_group;
		for(int i = 0; i < SCORE_MODE_COUNT; i++)
			for(int j = 0; j < SCORE_ATTR_COUNT; j++)
				s % scores[i][j];
	}
};

struct ColorWordnet : Moveable<ColorWordnet> {
	union {
		hash_t hash;
		int i32[2];
	};
	Vector<String> words;
	Vector<Color> colors;
	String main_class;
	String src_word;
	Color clr;
	int clr_group = -1;
	byte scores[SCORE_MODE_COUNT][SCORE_ATTR_COUNT] = {{0,0,0,0,0},{0,0,0,0,0}};
	
	void Jsonize(JsonIO& json) {
		json
			("hash0", i32[0])
			("hash1", i32[1])
			("words", words)
			("colors", colors)
			("main_class", main_class)
			("src_word", src_word)
			("clr", clr)
			("clr_group", clr_group)
			;
		for(int i = 0; i < SCORE_MODE_COUNT; i++)
			for(int j = 0; j < SCORE_ATTR_COUNT; j++)
				json(GetScoreKey(i,j), scores[i][j]);
	}
	void Serialize(Stream& s) {
		s % hash % words % colors % main_class % src_word % clr % clr_group;
		for(int i = 0; i < SCORE_MODE_COUNT; i++)
			for(int j = 0; j < SCORE_ATTR_COUNT; j++)
				s % scores[i][j];
	}
};

#endif

struct PackedRhymeHeader : Moveable<PackedRhymeHeader> {
	int syllable_count;
	int color_group;
	int attr;
	
	hash_t GetHashValue() const {
		CombineHash ch;
		ch.Do(syllable_count);
		ch.Do(color_group);
		ch.Do(attr);
		return ch;
	}
	void Jsonize(JsonIO& json) {
		json
			("sc", syllable_count)
			("cg", color_group)
			("at", attr);
	}
	void Serialize(Stream& s) {
		s % syllable_count % color_group % attr;
	}
	bool operator==(const PackedRhymeHeader& b) const {
		return	syllable_count == b.syllable_count &&
				color_group == b.color_group &&
				attr == b.attr;
	}
	bool operator()(const PackedRhymeHeader& a, const PackedRhymeHeader& b) const {
		if (a.syllable_count != b.syllable_count) return a.syllable_count < b.syllable_count;
		if (a.color_group != b.color_group) return a.color_group < b.color_group;
		return a.attr < b.attr;
	}
};

#if 0

struct ActionAttrs : Moveable<ActionAttrs> {
	Color clr;
	String group, value;
	void operator=(const ActionAttrs& a) {
		clr = a.clr;
		group = a.group;
		value = a.value;
	}
	void Jsonize(JsonIO& json) {
		json("clr", clr)("group",group)("value",value);
	}
	void Serialize(Stream& s) {
		s % clr % group % value;
	}
};

struct ActionParallel : Moveable<ActionParallel> {
	int count = 0;
	int score_sum = 0;
	void Jsonize(JsonIO& json) {
		json("count", count)("score_sum",score_sum);
	}
	void Serialize(Stream& s) {
		s % count % score_sum;
	}
};

struct ActionTransition : Moveable<ActionTransition> {
	int count = 0;
	int score_sum = 0;
	void Jsonize(JsonIO& json) {
		json("count", count)("score_sum",score_sum);
	}
	void Serialize(Stream& s) {
		s % count % score_sum;
	}
};

struct StructurePhrase : Moveable<StructurePhrase> {
	Vector<String> sent_parts;
	int type;
	void Jsonize(JsonIO& json) {
		json
			("sent_parts", sent_parts)
			("type", type)
			;
	}
	void Serialize(Stream& s) {
		s % sent_parts % type;
	}
};

struct StructureType : Moveable<StructureType> {
	Vector<String> part_types;
	String struct_type;
	Vector<int> transition_to;
	Color clr;
	Vector<int> phrases;
	
	String GetActionText() const;
	void Jsonize(JsonIO& json) {
		json
			("part_types", part_types)
			("struct_type", struct_type)
			("transition_to", transition_to)
			("clr", clr)
			("phrases", phrases)
			;
	}
	void Serialize(Stream& s) {
		s % part_types % struct_type % transition_to % clr % phrases;
	}
};
#endif


struct Token : Moveable<Token> {
	mutable int word_ = -1;
	
	String StoreToString() {
		StringDumper d;
		d % word_;
		return d;
	}
	void LoadFromString(const String& s) {
		StringParser p(s);
		p % word_;
	}
};

struct TokenText : Moveable<TokenText> {
	Vector<int> tokens;
	int virtual_phrase = -1;
	
	String StoreToString() {
		StringDumper d;
		d % tokens.GetCount();
		for (int t : tokens)
			d % t;
		d % virtual_phrase;
		return d;
	}
	void LoadFromString(const String& s) {
		StringParser p(s);
		int tc = 0;
		p % tc;
		tokens.SetCount(tc);
		for (int& t : tokens)
			p % t;
		p % virtual_phrase;
	}
};

struct ExportWord : Moveable<ExportWord> {
	static const int MAX_CLASS_COUNT = 8;
	
	String spelling;
	WString phonetic;
	int count = 0;
	Color clr;
	int class_count = 0;
	int classes[MAX_CLASS_COUNT];
	
	
	String StoreToString() {
		StringDumper d;
		d % spelling % phonetic % count % clr % class_count;
		for(int i = 0; i < class_count; i++)
			d % classes[i];
		return d;
	}
	void LoadFromString(const String& s) {
		StringParser p(s);
		// V2
		p % spelling % phonetic % count % clr % class_count;
		for(int i = 0; i < class_count; i++)
			p % classes[i];
	}
	
};

struct WordPairType : Moveable<WordPairType> {
	int from = -1, to = -1; // word index
	int from_type = -1, to_type = -1; // word class index
	
	String StoreToString() {
		StringDumper d;
		d % from % to % from_type % to_type;
		return d;
	}
	void LoadFromString(const String& s) {
		StringParser p(s);
		p % from % to % from_type % to_type;
	}
	
	hash_t GetHashValue() const {
		CombineHash c;
		c.Do(from).Put(1).Do(to);
		return c;
	}
};

struct VirtualPhrase : Moveable<VirtualPhrase> {
	Vector<int> word_classes;
	int virtual_phrase_struct = -1;
	
	String StoreToString() {
		StringDumper d;
		d % word_classes.GetCount();
		for (int wc_i : word_classes)
			d % wc_i;
		d % virtual_phrase_struct;
		return d;
	}
	void LoadFromString(const String& s) {
		StringParser p(s);
		int tc = 0;
		p % tc;
		word_classes.SetCount(tc);
		for (int& wc_i : word_classes)
			p % wc_i;
		p % virtual_phrase_struct;
	}
	
	hash_t GetHashValue() const {
		CombineHash c;
		for (int wc_i : word_classes)
			c.Do(wc_i).Put(1);
		return c;
	}
};

struct VirtualPhrasePart : Moveable<VirtualPhrasePart> {
	Vector<int> word_classes;
	int struct_part_type = -1;
	int count = 0;
	
	String StoreToString() {
		StringDumper d;
		d % word_classes.GetCount();
		for (int wc_i : word_classes)
			d % wc_i;
		d % struct_part_type % count;
		return d;
	}
	void LoadFromString(const String& s) {
		StringParser p(s);
		int tc = 0;
		p % tc;
		word_classes.SetCount(tc);
		for (int& wc_i : word_classes)
			p % wc_i;
		p % struct_part_type % count;
	}
	
	hash_t GetHashValue() const {
		CombineHash c;
		for (int wc_i : word_classes)
			c.Do(wc_i).Put(1);
		return c;
	}
};

struct VirtualPhraseStruct : Moveable<VirtualPhraseStruct> {
	Vector<int> virtual_phrase_parts;
	int struct_type = -1;
	int count = 0;
	
	String StoreToString() {
		StringDumper d;
		d % virtual_phrase_parts.GetCount();
		for (int part : virtual_phrase_parts)
			d % part;
		d % struct_type % count;
		return d;
	}
	void LoadFromString(const String& s) {
		StringParser p(s);
		int tc = 0;
		p % tc;
		virtual_phrase_parts.SetCount(tc);
		for (int& part : virtual_phrase_parts)
			p % part;
		p % struct_type % count;
	}
	
	hash_t GetHashValue() const {
		CombineHash c;
		for (int part : virtual_phrase_parts)
			c.Do(part).Put(1);
		return c;
	}
};

struct PhrasePart : Moveable<PhrasePart> {
	Vector<int> words;
	int tt_i = -1;
	int virtual_phrase_part = -1;
	int attr = -1;
	Color clr = Black();
	Vector<int> actions;
	
	String StoreToString() {
		StringDumper d;
		d % words.GetCount();
		for (int w_i : words)
			d % w_i;
		d % tt_i % virtual_phrase_part % attr % clr;
		d % actions.GetCount();
		for (int ah_i : actions)
			d % ah_i;
		return d;
	}
	void LoadFromString(const String& s) {
		StringParser p(s);
		int tc = 0;
		p % tc;
		words.SetCount(tc);
		for (int& w_i : words)
			p % w_i;
		p % tt_i % virtual_phrase_part;
		p % attr % clr;
		int ac = 0;
		p % ac;
		actions.SetCount(ac);
		for (int& ah_i : actions)
			p % ah_i;
	}
	
	hash_t GetHashValue() const {
		CombineHash c;
		for (int w_i : words)
			c.Do(w_i).Put(1);
		return c;
	}
};

struct ExportAttr : Moveable<ExportAttr> {
	
	String StoreToString() {
		return String();
	}
	void LoadFromString(const String& s) {
		StringParser p(s);
		
	}
	
};

struct ExportAction : Moveable<ExportAction> {
	int attr = -1;
	Color clr;
	
	String StoreToString() {
		StringDumper d;
		d % attr % clr;
		return d;
	}
	void LoadFromString(const String& s) {
		StringParser p(s);
		p % attr % clr;
	}
	
};

struct DatasetAnalysis {
	MapFile<String,Token> tokens;
	MapFile<hash_t,TokenText> token_texts;
	IndexFile word_classes;
	MapFile<String,ExportWord> words;
	MapFile<hash_t,WordPairType> ambiguous_word_pairs;
	MapFile<hash_t,VirtualPhrase> virtual_phrases;
	MapFile<hash_t,VirtualPhrasePart> virtual_phrase_parts;
	MapFile<hash_t,VirtualPhraseStruct> virtual_phrase_structs;
	MapFile<hash_t,PhrasePart> phrase_parts;
	IndexFile struct_part_types;
	IndexFile struct_types;
	MapFile<AttrHeader,ExportAttr> attrs;
	MapFile<ActionHeader,ExportAction> actions;
	
	// Cached data
	VectorMap<PackedRhymeHeader, Vector<PackedRhymeContainer>> packed_rhymes;
	
	
	DatasetAnalysis();
	DatasetAnalysis(DatasetAnalysis&) {}
	void Load(int ds_i, const String& ds_key);
	String GetTokenTextString(const TokenText& txt) const;
	String GetTokenTypeString(const TokenText& txt) const;
	String GetWordString(const Vector<int>& words) const;
	String GetTypeString(const Vector<int>& word_classes) const;
	String GetActionString(const Vector<int>& actions) const;
	
	
	void Jsonize(JsonIO& json) {
		json
			("packed_rhymes", packed_rhymes)
			;
	}
	void Serialize(Stream& s) {
		s % packed_rhymes;
	}
	
	#if 0
	VectorMap<String, ArtistAnalysis> artists;
	VectorMap<String, WordGroupAnalysis> groups;
	Vector<WordAnalysis> words;
	Vector<TemplatePhrase> tmpl_phrases;
	Vector<Wordnet> wordnets;
	Vector<ColorWordnet> clr_wordnets;
	Vector<ActionPhrase> action_phrases;
	Vector<StructurePhrase> structure_phrases;
	VectorMap<int64, StructureType> structure_types;
	Vector<ActionTemplate> action_tmpls;
	VectorMap<ActionHeader, ActionAttrs> action_attrs;
	VectorMap<ActionHeader, VectorMap<ActionHeader, ActionParallel>> action_parallel;
	VectorMap<ActionHeader, VectorMap<ActionHeader, ActionTransition>> action_trans;
	
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
	#endif
	
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
	
	
	SongData();
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
	void Load();
	void Store();
	void Serialize(Stream& s);
	/*void LoadJson();
	void StoreJson();
	void Jsonize(JsonIO& json);*/
	bool IsEmpty() const {return artists_en.IsEmpty() || artists_fi.IsEmpty();}
	
};


#endif
