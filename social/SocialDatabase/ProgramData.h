#ifndef _ToolBase_ProgramData_h_
#define _ToolBase_ProgramData_h_


struct StoryDataset : Moveable<StoryDataset> {
	String name;
	String text;
	
	void Serialize(Stream& s) {
		s % name % text;
	}
};

struct CompanyDataset : Moveable<CompanyDataset> {
	String name;
	Vector<StoryDataset> stories;
	
	void Serialize(Stream& s) {
		s % name % stories;
	}
};

struct PackedImpactHeader : Moveable<PackedImpactHeader> {
	int syllable_count;
	int color_group;
	int attr;
	
	hash_t GetHashValue() const {
		CombineHash ch;
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
	bool operator==(const PackedImpactHeader& b) const {
		return	syllable_count == b.syllable_count &&
				color_group == b.color_group &&
				attr == b.attr;
	}
	bool operator()(const PackedImpactHeader& a, const PackedImpactHeader& b) const {
		if (a.syllable_count != b.syllable_count) return a.syllable_count < b.syllable_count;
		if (a.color_group != b.color_group) return a.color_group < b.color_group;
		return a.attr < b.attr;
	}
};

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
	int link = -1;
	
	String StoreToString() {
		StringDumper d;
		d % spelling % phonetic % count % clr % class_count;
		for(int i = 0; i < class_count; i++)
			d % classes[i];
		d % link;
		return d;
	}
	void LoadFromString(const String& s) {
		StringParser p(s);
		// V2
		p % spelling % phonetic % count % clr % class_count;
		for(int i = 0; i < class_count; i++)
			p % classes[i];
		p % link;
	}
	void CopyFrom(const ExportWord& wa) {
		spelling.Clear();
		phonetic.Clear();
		spelling = wa.spelling;
		phonetic = wa.phonetic;
		count = wa.count;
		clr = wa.clr;
		class_count = wa.class_count;
		for(int i = 0; i < class_count; i++)
			classes[i] = wa.classes[i];
		// Don't copy link
		// link = wa.link;
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
	Vector<int> roles;
	Vector<int> generics;
	int scores[SCORE_COUNT] = {0,0,0,0,0,0,0,0,0,0};
	
	bool HasScores() const {
		for(int i = 0; i < SCORE_COUNT; i++)
			if (scores[i] != 0)
				return true;
		return false;
	}
	String StoreToString() {
		StringDumper d;
		d % words.GetCount();
		for (int w_i : words)
			d % w_i;
		d % tt_i % virtual_phrase_part % attr % clr;
		d % actions.GetCount();
		for (int ah_i : actions)
			d % ah_i;
		for(int i = 0; i < SCORE_COUNT; i++)
			d % scores[i];
		d % roles.GetCount();
		for (int tc_i : roles)
			d % tc_i;
		d % generics.GetCount();
		for (int i : generics)
			d % i;
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
		for(int i = 0; i < SCORE_COUNT; i++)
			p % scores[i];
		p % tc;
		if (tc < 0 || tc > 60) tc = 0;
		roles.SetCount(tc);
		for (int& tc_i : roles)
			p % tc_i;
		p % tc;
		generics.SetCount(tc);
		for (int& tc_i : generics)
			p % tc_i;
	}
	
	hash_t GetHashValue() const {
		CombineHash c;
		for (int w_i : words)
			c.Do(w_i).Put(1);
		return c;
	}
};

struct ExportAttr : Moveable<ExportAttr> {
	int simple_attr = -1, unused = -1;
	int positive = -1, link = -1;
	int count = 0;
	
	String StoreToString() {
		StringDumper d;
		d % simple_attr % unused % positive % link % count;
		return d;
	}
	void LoadFromString(const String& s) {
		StringParser p(s);
		p % simple_attr % unused % positive % link % count;
		positive = min(1, max(0, positive));
	}
	
};

struct ExportAction : Moveable<ExportAction> {
	int attr = -1;
	Color clr;
	int count = 0;
	
	String StoreToString() {
		StringDumper d;
		d % attr % clr % count;
		return d;
	}
	void LoadFromString(const String& s) {
		StringParser p(s);
		p % attr % clr % count;
	}
	
};

struct ExportParallel : Moveable<ExportParallel> {
	int count = 0, score_sum= 0;
	
	String StoreToString() {
		StringDumper d;
		d % count % score_sum;
		return d;
	}
	void LoadFromString(const String& s) {
		StringParser p(s);
		p % count % score_sum;
	}
	
};

struct ExportTransition : Moveable<ExportTransition> {
	int count = 0, score_sum= 0;
	
	String StoreToString() {
		StringDumper d;
		d % count % score_sum;
		return d;
	}
	void LoadFromString(const String& s) {
		StringParser p(s);
		p % count % score_sum;
	}
	
};

struct ExportDepActionPhrase : Moveable<ExportDepActionPhrase> {
	Vector<int> actions;
	Vector<int> next_phrases;
	Vector<Vector<int>> next_scores;
	int first_lines = 0;
	int attr = -1;
	Color clr = Black();
	
	String StoreToString() {
		StringDumper d;
		d.Do(actions.GetCount());
		for (int a : actions) d.Do(a);
		int c = next_phrases.GetCount();
		d.Do(c);
		for (int a : next_phrases) d.Do(a);
		for(int i = 0; i < c; i++) {
			auto& v = next_scores[i];
			d.Do(v.GetCount());
			for (int& s : v)
				d.Do(s);
		}
		d % first_lines % attr % clr;
		return d;
	}
	void LoadFromString(const String& s) {
		StringParser p(s);
		int ac = 0;
		p % ac;
		actions.SetCount(ac);
		for (int& a : actions) p % a;
		int c = 0;
		p % c;
		next_phrases.SetCount(c);
		next_scores.SetCount(c);
		for (int& i : next_phrases) p % i;
		for (auto& v : next_scores) {
			int sc = 0;
			p % sc;
			v.SetCount(sc);
			for (int& s : v)
				p.Do(s);
		}
		p % first_lines % attr % clr;
	}
	
};

struct ExportWordnet : Moveable<ExportWordnet> {
	static const int MAX_WORDS = 64;
	int word_count = 0;
	int words[MAX_WORDS] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
	int word_clr_count = 0;
	Color word_clrs[MAX_WORDS];
	int main_class = -1;
	int attr = -1;
	Color clr;
	int scores[SCORE_COUNT] = {0,0,0,0,0,0,0,0,0,0};
	
	String StoreToString() {
		StringDumper d;
		d % word_count;
		for(int i = 0; i < word_count; i++)
			d % words[i];
		d % word_clr_count;
		for(int i = 0; i < word_clr_count; i++)
			d % word_clrs[i];
		d % main_class % attr % clr;
		for(int i = 0; i < SCORE_COUNT; i++)
			d % scores[i];
		return d;
	}
	void LoadFromString(const String& s) {
		StringParser p(s);
		p % word_count;
		for(int i = 0; i < word_count; i++)
			p % words[i];
		p % word_clr_count;
		for(int i = 0; i < word_clr_count; i++)
			p % word_clrs[i];
		p % main_class % attr % clr;
		for(int i = 0; i < SCORE_COUNT; i++)
			p % scores[i];
	}
	
};

struct ExportSimpleAttr : Moveable<ExportSimpleAttr> {
	int attr_i0 = -1, attr_i1 = -1;
	
	String StoreToString() {
		StringDumper d;
		d % attr_i0 % attr_i1;
		return d;
	}
	void LoadFromString(const String& s) {
		StringParser p(s);
		p % attr_i0 % attr_i1;
	}
	
};

struct ProgramCandidateCache {
	struct Part : Moveable<Part> {
		void Serialize(Stream& s) {}
		void Jsonize(JsonIO& json) {
			
		}
	};
	
	Vector<Part> parts;
	 
	
	void Realize(Story& l);
	void Serialize(Stream& s) {s % parts;}
	void Jsonize(JsonIO& json) {
		json
			("parts", parts)
			;
	}
};


struct DatasetAnalysis;

struct PhraseComb : Moveable<PhraseComb> {
	Vector<int> phrase_parts;
	
	String StoreToString() {
		StringDumper d;
		for (int i : phrase_parts)
			d % i;
		return d;
	}
	void LoadFromString(const String& s) {
		Vector<String> parts = Split(s, " ");
		phrase_parts.Clear();
		for (String& p : parts)
			phrase_parts << ScanInt(p);
	}
};

struct StorySuggestions : Moveable<StorySuggestions> {
	VectorMap<String,Vector<String>> lines;
	int rank = -1;
	
	String StoreToString() {
		StringDumper d;
		d % lines % rank;
		return d;
	}
	void LoadFromString(const String& s) {
		StringParser p(s);
		p % lines;
		p % rank;
	}
	String GetText() const;
};

struct ProgramAnalysis {
	DatasetAnalysis* da = 0;
	
	MapFile<hash_t,PhrasePart> phrase_parts[ContrastType::PART_COUNT];
	IntIndexFile source_pool[ContrastType::PART_COUNT];
	MapFile<hash_t,PhraseComb> phrase_combs[ContrastType::PART_COUNT];
	MapFile<hash_t,StorySuggestions> story_suggs;
	
	void Load(const String& dir);
};

struct DatasetAnalysis {
	ArrayMap<String, ProgramAnalysis> programs;
	
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
	MapMapFile<int,int,ExportParallel> parallel;
	MapMapFile<int,int,ExportTransition> trans;
	MapFile<String,ExportDepActionPhrase> action_phrases;
	MapFile<String,String> translations;
	MapFile<hash_t,ExportWordnet> wordnets;
	MapFile<String,String> diagnostics;
	MapFile<String,ExportSimpleAttr> simple_attrs;
	
	// Cached data
	VectorMap<PackedImpactHeader, Vector<PackedRhymeContainer>> packed_rhymes;
	ArrayMap<String, ProgramCandidateCache> program_cache;
	
	// Temp
	int ds_i = -1;
	
	
	DatasetAnalysis();
	DatasetAnalysis(DatasetAnalysis&) {}
	DatasetAnalysis(DatasetAnalysis&& o) {LOG("warning: TODO: DatasetAnalysis(DatasetAnalysis&& o)");}
	void Load(int ds_i, const String& ds_key);
	String GetTokenTextString(const TokenText& txt) const;
	String GetTokenTypeString(const TokenText& txt) const;
	String GetWordString(const Vector<int>& words) const;
	WString GetWordPronounciation(const Vector<int>& words) const;
	String GetTypeString(const Vector<int>& word_classes) const;
	String GetActionString(const Vector<int>& actions) const;
	ProgramAnalysis& GetProgramAnalysis(const String& name);
	
	void Jsonize(JsonIO& json) {
		json
			("packed_rhymes", packed_rhymes)
			("program_cache", program_cache)
			;
	}
	void Serialize(Stream& s) {
		s % packed_rhymes % program_cache;
	}
	
};

struct ProgramDataAnalysis {
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

struct ProgramData {
	// Binary data
	Vector<CompanyDataset> companies_en;
	Vector<CompanyDataset> companies_fi;
	
	ProgramDataAnalysis a;
	
	
	ProgramData();
	int GetCount() const {return 2;}
	Vector<CompanyDataset>& operator[](int i) {
		switch (i) {
			case 0: return companies_en;
			case 1: return companies_fi;
			default: Panic("error");
		}
		return Single<Vector<CompanyDataset>>();
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
	bool IsEmpty() const {return companies_en.IsEmpty() || companies_fi.IsEmpty();}
	
};


#endif
