#ifndef _SocialDatabase_Database_h_
#define _SocialDatabase_Database_h_


BEGIN_SOCIALLIB_NAMESPACE


struct SocialDatabase {
	// Public
	Array<Company>	companies;
	
	// Local (separate)
	ProgramData		program_data;
	
	// Temp
	String			dir;
	RWMutex			lock;
	
	SocialDatabase();
	Array<Company>& GetSub() {return companies;}
	const Array<Company>& GetSub() const {return companies;}
	void Clear() {companies.Clear(); }
	void Store();
	void Load();
	void FindOrphaned();
	void Serialize(Stream& s) {
		s	% companies;
	}
	void Jsonize(JsonIO& json) {
		if (json.IsStoring()) {
			Vector<String> names;
			for (Company& a : companies) {a.Store(); names.Add(a.file_title);}
			json("companies", names);
		}
		if (json.IsLoading()) {
			Vector<String> names;
			json("companies", names);
			for (String n : names) companies.Add().LoadTitle(n);
			Sort(companies, Company());
		}
	}
	String GetCompanysDir() const;
	String GetCampaignsDir() const;
	String GetProgramsDir() const;
	
	
	
	static SocialDatabase& Single() {static SocialDatabase db; return db;}
	
};


END_SOCIALLIB_NAMESPACE


#endif