#ifndef _SocialDatabase_Campaign_h_
#define _SocialDatabase_Campaign_h_


struct Release : DataFile
{
	// Public
	String						native_title;
	String						english_title;
	Date						date;
	VectorMap<String,String>	data;
	Array<ProgramIdea>				ideas;
	int							year_of_content = 0;
	
	// Public (separate files)
	Array<Program> programs;
	
	
	
	void Store();
	void LoadTitle(String title);
	void Serialize(Stream& s) {
		s	% native_title
			% english_title
			% date
			% ideas
			% year_of_content
			% programs;
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
				for (Program& s : programs) {s.Store(); names.Add(s.file_title);}
				json("programs", names);
			}
		}
		if (json.IsLoading()) {
			{
				programs.Clear();
				Vector<String> names;
				json("programs", names);
				for (String n : names) programs.Add().LoadTitle(n);
			}
		}
	}
	
	Array<Program>& GetSub() {return programs;}
	const Array<Program>& GetSub() const {return programs;}
	bool operator()(const Release& a, const Release& b) const {
		if (a.date != b.date) return a.date < b.date;
		return a.native_title < b.native_title;
	}
	
	
};


#endif
