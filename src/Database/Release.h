#ifndef _SongTool_Release_h_
#define _SongTool_Release_h_


struct Release :
	DataFile
{
	// Public
	String						native_title;
	String						english_title;
	Date						date;
	VectorMap<String,String>	data;
	Array<SongIdea>				ideas;
	int							year_of_content = 0;
	
	// Public (separate files)
	Array<Song>		songs;
	
	
	
	void Store();
	void LoadTitle(String title);
	//Song& RealizeReversed(Song& s);
	void Serialize(Stream& s) {
		s	% native_title
			% english_title
			% date
			% ideas
			% year_of_content
			% songs;
	}
	void Jsonize(JsonIO& json) {
		json
			("title", native_title)
			("english_title", english_title)
			("date", date)
			("data", data)
			("year_of_content", year_of_content)
			("ideas", ideas)
			;
		if (json.IsStoring()) {
			{
				Vector<String> names;
				for (Song& s : songs) {s.Store(); names.Add(s.file_title);}
				json("songs", names);
			}
		}
		if (json.IsLoading()) {
			{
				songs.Clear();
				Vector<String> names;
				json("songs", names);
				for (String n : names) songs.Add().LoadTitle(n);
			}
		}
	}
	
	Array<Song>& GetSub() {return songs;}
	const Array<Song>& GetSub() const {return songs;}
	bool operator()(const Release& a, const Release& b) const {
		if (a.date != b.date) return a.date < b.date;
		return a.native_title < b.native_title;
	}
	
	
	
};


#endif
