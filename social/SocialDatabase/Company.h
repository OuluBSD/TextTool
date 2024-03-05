#ifndef _SocialDatabase_Company_h_
#define _SocialDatabase_Company_h_


BEGIN_SOCIALLIB_NAMESPACE


struct Company : DataFile
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
	VectorMap<String,String>	phrases_nat;
	VectorMap<String,String>	phrases_eng;
	bool is_female = false;
	int language = 0;
	
	// Public (separate files)
	Array<Role>	roles;
	Array<Campaign> campaigns;
	
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
	void StoreStory();
	void LoadStory();
	void RealizeRoles();
	void LoadTitle(String title);
	String GetStoryDir() const;
	
	Array<Campaign>& GetSub() {return campaigns;}
	const Array<Campaign>& GetSub() const {return campaigns;}
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
			% campaigns
			% phrases_nat
			% phrases_eng
			% is_female
			% language
			;
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
			("phrases", phrases_nat)
			("phrases_eng", phrases_eng)
			("is_female", is_female)
			("language", language)
			;
		if (json.IsStoring()) {
			Vector<String> names;
			for (Campaign& r : campaigns) {r.Store(); names.Add(r.file_title);}
			json("campaigns", names);
			
		}
		if (json.IsLoading()) {
			campaigns.Clear();
			Vector<String> names;
			json("campaigns", names);
			for (String n : names) campaigns.Add().LoadTitle(n);
		}
	}
	
	bool operator()(const Company& a, const Company& b) const {
		if (a.year_of_birth != b.year_of_birth) return a.year_of_birth < b.year_of_birth;
		return a.native_name < b.native_name;
	}
	
	bool FindProgram(int& tc, int& arch, int& lyr_i, const String& story_file_title) const;
	
};


END_SOCIALLIB_NAMESPACE


#endif
