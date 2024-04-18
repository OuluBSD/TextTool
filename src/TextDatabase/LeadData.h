#ifndef _TextDatabase_LeadData_h_
#define _TextDatabase_LeadData_h_


BEGIN_TEXTLIB_NAMESPACE




struct LeadEntityAnalysis {
	
	
	
};


struct LeadDataAnalysis {
	ArrayMap<String, LeadEntityAnalysis> entities;
	
	// MapFiles
	
	
	
	// Cached data
	
	
	LeadDataAnalysis();
	LeadDataAnalysis(LeadDataAnalysis&) {}
	LeadDataAnalysis(LeadDataAnalysis&& o) {LOG("warning: TODO: LeadDataAnalysis(LeadDataAnalysis&& o)");}
	
	LeadEntityAnalysis& GetLeadEntityAnalysis(const String& name) {return entities.GetAdd(name);}
	
	/*void Load();
	String GetTokenTextString(const TokenText& txt) const;
	String GetTokenTypeString(const TokenText& txt) const;
	String GetWordString(const Vector<int>& words) const;
	WString GetWordPronounciation(const Vector<int>& words) const;
	String GetTypeString(const Vector<int>& word_classes) const;
	String GetActionString(const Vector<int>& actions) const;
	ComponentAnalysis& GetComponentAnalysis(int appmode, const String& name);
	
	void Jsonize(JsonIO& json) {
		json
			("packed_rhymes", packed_rhymes)
			(GetAppModeKey(AM_COMPONENT) + "_cache", cache)
			;
	}
	void Serialize(Stream& s) {
		s % packed_rhymes % cache;
	}
	
	
	
	void Jsonize(JsonIO& json) {
		ArrayMap<String, DatasetAnalysis> datasets;
		json
			("datasets", datasets)
			;
			
		TODO
	}
	
	
	void Serialize(Stream& s) {
		ArrayMap<String, DatasetAnalysis> datasets;
		s % datasets;
		TODO
	}
	void StoreJson();
	void LoadJson();
	void Store();
	void Load();*/
};



struct LeadData {
	LeadDataAnalysis a;
	
	
	LeadData();
	
	void Load();
	void Store();
	void Serialize(Stream& s);
	
};


END_TEXTLIB_NAMESPACE


#endif
