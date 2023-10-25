#ifndef _SongTool_Artist_h_
#define _SongTool_Artist_h_


struct Artist :
	DataFile,
	EditorPtrs
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
	
	// Public (separate files)
	Array<Release> releases;
	
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
	void LoadTitle(String title);
	/*void FixPtrs() {
		SetArtistPtr(this);
		int id = 0;
		for (Release& r : releases) {
			r.CopyPtrs(*this);
			r.SetOwner(*this);
			r.SetId(id++);
			r.FixPtrs();
		}
	}*/
	Array<Release>& GetSub() {return releases;}
	const Array<Release>& GetSub() const {return releases;}
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
			% releases
			% data
			% phrases_nat
			% phrases_eng
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
			;
		if (json.IsStoring()) {
			Vector<String> names;
			for (Release& r : releases) {r.Store(); names.Add(r.file_title);}
			json("releases", names);
		}
		if (json.IsLoading()) {
			releases.Clear();
			Vector<String> names;
			json("releases", names);
			for (String n : names) releases.Add().LoadTitle(n);
			//Sort(releases, Release());
			
			FixPtrs();
		}
		//SnapContext::Jsonize(json);
	}
	
	void FixPtrs() {
		for (Release& r : releases) {
			r.EditorPtrs::artist = this;
			r.FixPtrs();
		}
	}
	bool operator()(const Artist& a, const Artist& b) const {
		if (a.year_of_birth != b.year_of_birth) return a.year_of_birth < b.year_of_birth;
		return a.native_name < b.native_name;
	}
	
	
};


#endif
