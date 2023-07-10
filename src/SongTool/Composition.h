#ifndef _SongTool_Composition_h_
#define _SongTool_Composition_h_


struct Composition {
	int year = 0;
	String title;
	int tempo = 0;
	String beat;
	String melody;
	String chord_progression;
	String key_and_mode;
	String texture;
	String genre_style;
	
	void Clear() {
		year = 0;
		title.Clear();
		tempo = 0;
		beat.Clear();
		melody.Clear();
		chord_progression.Clear();
		key_and_mode.Clear();
		texture.Clear();
		genre_style.Clear();
	}
	void Jsonize(JsonIO& json) {
		json
			("year", year)
			("title", title)
			("tempo", tempo)
			("beat", beat)
			("melody", melody)
			("chord_progression", chord_progression)
			("key_and_mode", key_and_mode)
			("texture", texture)
			("genre_style", genre_style)
			;
	}
	bool operator()(const Composition& a, const Composition& b) const {
		if (a.year != b.year) return a.year < b.year;
		return a.title < b.title;
	}
	
};


#endif
