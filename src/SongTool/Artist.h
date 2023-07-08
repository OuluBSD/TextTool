#ifndef _SongTool_Artist_h_
#define _SongTool_Artist_h_


struct Artist : DataFile {
	String name;
	int year_of_birth = 0;
	int year_of_career_begin = 0;
	String biography;
	String musical_style;
	String vibe_of_voice;
	String acoustic_instruments;
	String electronic_instruments;
	
	Array<Release> releases;
	
	void Clear() {
		name.Clear();
		year_of_birth = 0;
		year_of_career_begin = 0;
		biography.Clear();
		musical_style.Clear();
		vibe_of_voice.Clear();
		acoustic_instruments.Clear();
		electronic_instruments.Clear();
	}
	void Store();
	void LoadTitle(String title);
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("year_of_birth", year_of_birth)
			("year_of_career_begin", year_of_career_begin)
			("biography", biography)
			("musical_style", musical_style)
			("vibe_of_voice", vibe_of_voice)
			("acoustic_instruments", acoustic_instruments)
			("electronic_instruments", electronic_instruments)
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
			Sort(releases, Release());
		}
	}
	
	bool operator()(const Artist& a, const Artist& b) const {
		if (a.year_of_birth != b.year_of_birth) return a.year_of_birth < b.year_of_birth;
		return a.name < b.name;
	}
	
};


#endif
