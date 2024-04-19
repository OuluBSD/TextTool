#ifndef _TextDatabase_LeadData_h_
#define _TextDatabase_LeadData_h_


BEGIN_TEXTLIB_NAMESPACE




struct LeadOpportunity : Moveable<LeadOpportunity> {
	struct Genre : Moveable<Genre> {
		int id;
		String name;
		bool primary;
		void Jsonize(JsonIO& json);
		String ToString() const;
	};
	
	int leadsite = -1;
	String id;
	Time first_seen, last_seen;
	String name;
	Vector<String> links;
	Vector<Genre> genres;
	Vector<Genre> promoter_group_genres;
	String band_opportunity_type;
	String obj_class;
	bool request_entry_fee = 0;
	bool request_featured = 0;
	bool request_exclusive = 0;
	bool request_curated = 0;
	bool request_contest = 0;
	String request_comments;
	String request_first_name;
	String request_last_name;
	String request_email;
	String request_phone;
	String request_description;
	String request_opportunity_description;
	String request_band_description;
	String request_selection_description;
	int vanity_url_id = 0;
	String vanity_url_name;
	String status_text;
	String description;
	String band_opportunity_type_text;
	String local_event_end_datetime;
	bool is_accepting_entries = true;
	bool deleted = 0;
	String address_str;
	String public_image_url;
	String logo_image_url;
	String promoter_group_name;
	String promoter_group_main_image_url;
	String promoter_group_facebook_url;
	String promoter_group_twitter_url;
	String promoter_group_youtube_url;
	String promoter_group_instagram_url;
	String promoter_group_talent_description;
	String promoter_group_short_description;
	String promoter_group_talent_roster;
	int promoter_group_opportunity_frequency_count = 0;
	String promoter_group_opportunity_frequency;
	bool compensated = 0;
	int min_compensation = 0;
	int max_compensation = 0;
	bool pay_to_apply = 0;
	bool free_to_apply = 0;
	int entry_count = 0;
	int min_entry_price_cents = 0;
	String entry_end_datetime;
	String date_created;
	
	int GetCount() const;
	Value operator[](int i) const;
	const char* GetKey(int i) const;
	void Jsonize(JsonIO& json);
};


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
	
	Vector<LeadOpportunity> opportunities;
	
	LeadData();
	LeadData(LeadData&&) {}
	LeadData(const LeadData&) {}
	void Load();
	void Store();
	LeadOpportunity& GetAddOpportunity(int leadsite, String id);
	void Jsonize(JsonIO& json);
	
};


END_TEXTLIB_NAMESPACE


#endif
