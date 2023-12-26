#ifndef _ToolCore_Nana_h_
#define _ToolCore_Nana_h_


class RhymeContainer {
	
public:
	
	struct Syllable : Moveable<Syllable> {
		int consonant_importance;
		int vocal_importance;
		bool long_ = false;
		bool at_beat = false;
		
		Syllable() {}
		Syllable(const Syllable& s) {*this = s;}
		void operator=(const Syllable& s) {consonant_importance = s.consonant_importance; vocal_importance = s.vocal_importance; long_ = s.long_; at_beat = s.at_beat;}
		void Jsonize(JsonIO& json) {json("c", consonant_importance)("v", vocal_importance)("l", long_)("b", at_beat);}
		String AsString() const;
	};
	
	struct Word : Moveable<Word> {
		Vector<Syllable> syllables;
		
		Word() {}
		Word(const Word& s) {*this = s;}
		void operator=(const Word& s) {syllables <<= s.syllables;}
		void Jsonize(JsonIO& json) {json("syllables", syllables);}
	};
	
	struct Line : Moveable<Line> {
		Vector<Word> words;
		
		Line() {}
		Line(const Line& s) {*this = s;}
		void operator=(const Line& s) {words <<= s.words;}
		void Jsonize(JsonIO& json) {json("words", words);}
		String AsNana() const;
	};
	
protected:
	friend class MockupPhraseParser;
	Vector<Line> lines;
	
public:
	typedef RhymeContainer CLASSNAME;
	RhymeContainer() {}
	RhymeContainer(const RhymeContainer& rc) {*this = rc;}
	
	const Vector<Line>& Get() const {return lines;}
	
	void Clear() {lines.Clear();}
	void operator=(const RhymeContainer& rc) {lines <<= rc.lines;}
	
	String ToString() const;
	String AsNana() const;
	void Dump() {LOG(ToString());}
	void Jsonize(JsonIO& json) {
		json("lines", lines);
	}
};

class MockupPhraseParser {
	String err;
	bool fail = false;
	Vector<Vector<String>> tokens;
	
	void SetError(String s) {fail = true; err = s;}
public:
	
	static bool IsVowel(int chr);
	static bool IsConsonant(int chr);
	
public:
	typedef MockupPhraseParser CLASSNAME;
	MockupPhraseParser();
	
	bool Parse(String txt);
	bool Process(RhymeContainer& rc);
	
	String GetError() const {return err;}
	
	
};


class PhoneticNanaAnalyser {
	const char* error = 0;
	Vector<int> tmp, enums, vowel_vec, consonant_vec;
	VectorMap<int, Tuple2<double,int>> vowels, consonants;
	Vector<Vector<int>> vowel_clusters, consonant_clusters;
	Vector<byte> nana_out;
	
	// How far away the phoneme is allowed to be in the same cluster [0-1]
	// See tables vowel_distance and consonant_distance
	double cluster_phoneme_distance_limit = 0.38197097; // golden ratio
	
public:
	
	bool Parse(wchar* phon, int len);
	void WritePackedNana(byte* nana, int len);
	
	const char* GetError() const {return error;}
	
};


enum : byte {
	NANA_IMPORTANCE_0 = 0,
	NANA_IMPORTANCE_1,
	NANA_IMPORTANCE_2,
	NANA_IMPORTANCE_3,
	NANA_IMPORTANCE_4,
	NANA_IMPORTANCE_5,
	NANA_IMPORTANCE_6,
	NANA_IMPORTANCE_SPECIFIC,
	
	NANA_MEANINGLESS = NANA_IMPORTANCE_6,
	
	// Bits 0-2
	NANA_VOWEL_0 = 0,
	NANA_VOWEL_1,
	NANA_VOWEL_2,
	NANA_VOWEL_3,
	NANA_VOWEL_4,
	NANA_VOWEL_5,
	NANA_VOWEL_6,
	NANA_VOWEL_7,
	
	
	// Bits 3-5
	NANA_CONSONANT_0 = 0 << 3,
	NANA_CONSONANT_1 = 1 << 3,
	NANA_CONSONANT_2 = 1 << 3,
	NANA_CONSONANT_3 = 1 << 3,
	NANA_CONSONANT_4 = 1 << 3,
	NANA_CONSONANT_5 = 1 << 3,
	NANA_CONSONANT_6 = 1 << 3,
	NANA_CONSONANT_7 = 1 << 3,
	
	NANA_CONTINUE = 1 << 6,
	NANA_LONG = 1 << 7,
	
	NANA_VOWEL_MEANINGLESS = NANA_VOWEL_6,
	NANA_CONSONANT_MEANINGLESS = NANA_CONSONANT_6,
	
	NANA_VOWEL_SPECIFIC = NANA_VOWEL_7,
	NANA_CONSONANT_SPECIFIC = NANA_CONSONANT_7,
	
	NANA_VOWEL_MASK = 0x7,
	NANA_CLUSTER_MASK = 0x38,
};

class NanaCompare {
	Index<int> seen_v0, seen_c0, seen_v1, seen_c1;
	
public:
	int GetDistance(const byte* nana0, int nana0_len, const byte* nana1, int nana1_len);
	
};

#endif
