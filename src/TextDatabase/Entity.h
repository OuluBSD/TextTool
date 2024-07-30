#ifndef _TextDatabase_Entity_h_
#define _TextDatabase_Entity_h_


BEGIN_TEXTLIB_NAMESPACE


#define USE_IMPROVED_ELEMENTS 0

#if USE_IMPROVED_ELEMENTS
	#define ELEMENTS_VAR	improved_elements
#else
	#define ELEMENTS_VAR	elements
#endif

struct ConceptStory : Moveable<ConceptStory> {
	struct Element : Moveable<Element> {
		String key, value;
		Color clr;
		byte scores[SCORE_COUNT] = {0,0,0,0,0, 0,0,0,0,0};
		void Serialize(Stream& s) {s % key % value % clr; for(int i = 0; i < SCORE_COUNT; i++) s % scores[i];}
		void Jsonize(JsonIO& json) {json("k",key)("v",value)("clr",clr); for(int i = 0; i < SCORE_COUNT; i++) json("s" + IntStr(i),scores[i]);}
		void ResetScore() {memset(scores, 0, sizeof(scores));}
		double GetAverageScore() const;
	};
	
	hash_t hash = 0;
	String desc;
	Vector<Element> elements;
	#if USE_IMPROVED_ELEMENTS
	Vector<Element> improved_elements;
	#endif
	int src = 0;
	int typeclass = -1;
	int content = -1;
	
	int FindElement(const String& key) const;
	int FindImprovedElement(const String& key) const;
	void Serialize(Stream& s) {
		s % hash % desc % elements % src;
		#if USE_IMPROVED_ELEMENTS
		s % improved_elements;
		#endif
	}
	void Jsonize(JsonIO& json) {
		json
			("hash", (int64&)hash)
			("desc", desc)
			("elements", elements)
			("src", src)
			("typeclass", typeclass)
			("content", content)
			;
		#if USE_IMPROVED_ELEMENTS
		json("improved_elements", improved_elements);
		#endif
	}
	
	double AvSingleScore(int i) const;
	Color GetAverageColor() const;
	double GetAverageScore() const;
	bool operator()(const ConceptStory& a, const ConceptStory& b) const;
	
};

struct Concept {
	int64 belief_uniq = 0;
	String name;
	int snap_rev = -1;
	Time created;
	Vector<ConceptStory> stories;
	
	int FindStory(hash_t h) const;
	ConceptStory& GetAddStory(hash_t h);
	void Serialize(Stream& s) {
		s % belief_uniq % name % snap_rev % created % stories;
	}
	void Jsonize(JsonIO& json) {
		json
			("belief_uniq", belief_uniq)
			("name", name)
			("snap_rev", snap_rev)
			("created", created)
			("stories", stories)
			;
	}
};


struct Entity :
	DataFile, ContentVisionOwner
{
	Profile* profile = 0;
	
	// Public
	String native_name;
	String english_name;
	int year_of_birth = 0;
	int year_of_career_begin = 0;
	String biography;
	String text_style;
	String vibe_of_text;
	String natural_tools;
	String electronic_tools;
	String speaker_visually;
	VectorMap<String,String>	data;
	VectorMap<String,String>	phrases_nat;
	VectorMap<String,String>	phrases_eng;
	bool is_female = false;
	int language = 0;
	
	// Public (separate files)
	Array<Script>		scripts;
	Array<Snapshot>		snaps;
	Array<Concept>		concepts;
	
	void Clear() {
		native_name.Clear();
		year_of_birth = 0;
		year_of_career_begin = 0;
		biography.Clear();
		text_style.Clear();
		vibe_of_text.Clear();
		natural_tools.Clear();
		electronic_tools.Clear();
		speaker_visually.Clear();
		phrases_nat.Clear();
		phrases_eng.Clear();
	}
	void Store();
	void StoreScript();
	void LoadScript();
	//void RealizeTypeclasses(int appmode);
	void LoadTitle(String title);
	String GetScriptDir() const;
	/*void FixPtrs() {
		SetEntityPtr(this);
		int id = 0;
		for (Snapshot& r : snaps) {
			r.CopyPtrs(*this);
			r.SetOwner(*this);
			r.SetId(id++);
			r.FixPtrs();
		}
	}*/
	Snapshot& GetAddSnapshot(String name);
	Array<Snapshot>& GetSub() {return snaps;}
	const Array<Snapshot>& GetSub() const {return snaps;}
	void Serialize(Stream& s) {
		s	% native_name
			% english_name
			% year_of_birth
			% year_of_career_begin
			% biography
			% text_style
			% vibe_of_text
			% natural_tools
			% electronic_tools
			% speaker_visually
			% snaps
			% data
			% phrases_nat
			% phrases_eng
			% is_female
			% language
			% concepts
			;
		//SnapContext::Serialize(s);
	}
	void Jsonize(JsonIO& json) {
		json
			("name", native_name)
			("english_name", english_name)
			("year_of_birth", year_of_birth)
			("year_of_career_begin", year_of_career_begin)
			("biography", biography)
			(GetAppModeLabel(AML_FIELD_TEXT_STYLE), text_style)
			(GetAppModeLabel(AML_FIELD_VIBE_OF_TEXT), vibe_of_text)
			(GetAppModeLabel(AML_FIELD_NATURAL_TOOLS), natural_tools)
			(GetAppModeLabel(AML_FIELD_ELECTRONIC_TOOLS), electronic_tools)
			(GetAppModeLabel(AML_FIELD_PERSON_VISUALLY), speaker_visually)
			("data", data)
			("phrases", phrases_nat)
			("phrases_eng", phrases_eng)
			("is_female", is_female)
			("language", language)
			("ideas", ideas)
			("concepts", concepts)
			;
		if (json.IsStoring()) {
			Vector<String> names;
			for (Snapshot& r : snaps) {r.Store(*this); names.Add(r.file_title);}
			json(__snaps, names);
			
		}
		if (json.IsLoading()) {
			snaps.Clear();
			Vector<String> names;
			json(__snaps, names);
			for (String n : names) snaps.Add().LoadTitle(*this, n);
		}
	}
	
	bool operator()(const Entity& a, const Entity& b) const {
		if (a.year_of_birth != b.year_of_birth) return a.year_of_birth < b.year_of_birth;
		return a.native_name < b.native_name;
	}
	
	int FindScript(const String& scripts_file_title) const;
	Script& GetAddScript(String name);
	
	static bool FileExists(const String& title);
};


END_TEXTLIB_NAMESPACE


#endif
