#ifndef _SongTool_Release_h_
#define _SongTool_Release_h_


struct Release :
	DataFile,
	SnapContext
{
	String			title;
	Date			date;
	Array<Song>		songs;
	
	
	
	void Store();
	void LoadTitle(String title);
	//Song& RealizeReversed(Song& s);
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
		SnapContext::Jsonize(json);
	}
	void FixPtrs() {
		SetReleasePtr(this);
		int id = 0;
		for (Song& s : songs) {
			s.CopyPtrs(*this);
			s.SetOwner(*this);
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
