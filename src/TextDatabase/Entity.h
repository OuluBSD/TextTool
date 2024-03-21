#ifndef _TextDatabase_Entity_h_
#define _TextDatabase_Entity_h_


BEGIN_TEXTLIB_NAMESPACE


struct Entity :
	DataFile
{
	// Public
	String native_name;
	String english_name;
	int year_of_birth = 0;
	int year_of_career_begin = 0;
	String biography;
	String musical_style;
	String vibe_of_voice;
	String acoustic_instruments;
	String electronic_instruments;
	String vocalist_visual;
	VectorMap<String,String>	data;
	VectorMap<String,String>	phrases_nat;
	VectorMap<String,String>	phrases_eng;
	bool is_female = false;
	int language = 0;
	
	// Public (separate files)
	Array<Typeclass>	typecasts;
	Array<Snapshot> snaps;
	
	void Clear() {
		native_name.Clear();
		year_of_birth = 0;
		year_of_career_begin = 0;
		biography.Clear();
		musical_style.Clear();
		vibe_of_voice.Clear();
		acoustic_instruments.Clear();
		electronic_instruments.Clear();
		vocalist_visual.Clear();
		phrases_nat.Clear();
		phrases_eng.Clear();
	}
	void Store();
	void StoreScript();
	void LoadScript();
	void RealizeTypeclasss();
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
	Array<Snapshot>& GetSub() {return snaps;}
	const Array<Snapshot>& GetSub() const {return snaps;}
	void Serialize(Stream& s) {
		s	% native_name
			% english_name
			% year_of_birth
			% year_of_career_begin
			% biography
			% musical_style
			% vibe_of_voice
			% acoustic_instruments
			% electronic_instruments
			% vocalist_visual
			% snaps
			% data
			% phrases_nat
			% phrases_eng
			% is_female
			% language
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
			("musical_style", musical_style)
			("vibe_of_voice", vibe_of_voice)
			("acoustic_instruments", acoustic_instruments)
			("electronic_instruments", electronic_instruments)
			("vocalist_visual", vocalist_visual)
			("data", data)
			("phrases", phrases_nat)
			("phrases_eng", phrases_eng)
			("is_female", is_female)
			("language", language)
			;
		if (json.IsStoring()) {
			Vector<String> names;
			for (Snapshot& r : snaps) {r.Store(); names.Add(r.file_title);}
			json("snaps", names);
			
		}
		if (json.IsLoading()) {
			snaps.Clear();
			Vector<String> names;
			json("snaps", names);
			for (String n : names) snaps.Add().LoadTitle(n);
		}
	}
	
	bool operator()(const Entity& a, const Entity& b) const {
		if (a.year_of_birth != b.year_of_birth) return a.year_of_birth < b.year_of_birth;
		return a.native_name < b.native_name;
	}
	
	bool FindComponent(int& tc, int& arch, int& lyr_i, const String& scripts_file_title) const;
	
};


END_TEXTLIB_NAMESPACE


#endif
