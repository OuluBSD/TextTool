#ifndef _SongTool_Release_h_
#define _SongTool_Release_h_


struct Release :
	DataFile,
	EditorPtrs
{
	// Public
	String						native_title;
	String						english_title;
	Date						date;
	VectorMap<String,String>	data;
	Array<SongIdea>				ideas;
	
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
			% songs;
	}
	void Jsonize(JsonIO& json) {
		json
			("title", native_title)
			("english_title", english_title)
			("date", date)
			("data", data)
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
			FixPtrs();
		}
	}
	void FixPtrs() {
		for (Song& s : songs) {
			s.EditorPtrs::release = this;
			s.EditorPtrs::artist = EditorPtrs::artist;
			s.FixPtrs();
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
