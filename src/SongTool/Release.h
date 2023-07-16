#ifndef _SongTool_Release_h_
#define _SongTool_Release_h_


struct Release :
	DataFile,
	PatternSnap
{
	String			title;
	Date			date;
	Array<Song>		songs;
	Array<Song>		reversed_songs;
	
	
	
	void Store();
	void LoadTitle(String title);
	Song& RealizeReversed(Song& s);
	void Jsonize(JsonIO& json) {
		json
			("title", title)
			("date", date)
			;
		if (json.IsStoring()) {
			{
				Vector<String> names;
				for (Song& s : songs) {s.Store(); names.Add(s.file_title);}
				json("songs", names);
			}{
				Vector<String> names;
				for (Song& s : reversed_songs) {s.SetReversed(); s.Store(); names.Add(s.file_title);}
				json("reversed_songs", names);
			}
		}
		if (json.IsLoading()) {
			{
				songs.Clear();
				Vector<String> names;
				json("songs", names);
				for (String n : names) songs.Add().LoadTitle(n);
			}{
				reversed_songs.Clear();
				Vector<String> names;
				json("reversed_songs", names);
				for (String n : names) reversed_songs.Add().SetReversed().LoadTitle(n);
			}
		}
		PatternSnap::Jsonize(json);
	}
	void FixPtrs() {
		this->release = this;
		int id = 0;
		for (Song& s : songs) {
			static_cast<Ptrs&>(s) = *(Ptrs*)this;
			s.owner = this;
			s.SetId(id++);
			s.FixPtrs();
		}
		id = 0;
		for (Song& s : reversed_songs) {
			static_cast<Ptrs&>(s) = *(Ptrs*)this;
			s.owner = this;
			s.SetReversed();
			s.SetId(id++);
			s.FixPtrs();
		}
	}
	Array<Song>& GetSub() {return songs;}
	const Array<Song>& GetSub() const {return songs;}
	bool operator()(const Release& a, const Release& b) const {
		if (a.date != b.date) return a.date < b.date;
		return a.title < b.title;
	}
	
	
	PATTERNMASK_MACROS
};


#endif
