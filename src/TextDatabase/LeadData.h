#ifndef _TextDatabase_LeadData_h_
#define _TextDatabase_LeadData_h_


BEGIN_TEXTLIB_NAMESPACE


enum {
	LISTING_SONG_BOOLEAN_IS_ALLOWED_MALE,
	LISTING_SONG_BOOLEAN_IS_ALLOWED_FEMALE,
	LISTING_SONG_BOOLEAN_ALLOWED_DEMO_QUALITY,
	LISTING_SONG_BOOLEAN_MUST_BE_BROADCAST_QUALITY,
	LISTING_SONG_BOOLEAN_MONETARY_SIGNIFICANT_INCOME,
	LISTING_SONG_BOOLEAN_MONETARY_DIFFICULT_TO_DETERMINE,
	LISTING_SONG_BOOLEAN_MONETARY_COULD_BE_ROYALTIES,
	LISTING_SONG_BOOLEAN_MONETARY_FUTURE_COLLABS,
	LISTING_SONG_BOOLEAN_IS_EXCLUSIVE,
	LISTING_SONG_BOOLEAN_PLACEMENT_TV,
	LISTING_SONG_BOOLEAN_PLACEMENT_RADIO,
	LISTING_SONG_BOOLEAN_PLACEMENT_MOVIE,
	LISTING_SONG_BOOLEAN_PLACEMENT_AD,
	LISTING_SONG_BOOLEAN_PLACEMENT_PLAYLIST,
	LISTING_SONG_BOOLEAN_LISTING_WRITER_IS_DECISION_MAKER,
	LISTING_SONG_BOOLEAN_LISTING_WRITER_IS_COMPANY,
	LISTING_SONG_BOOLEAN_LISTING_WRITER_IS_PERSON,
	LISTING_SONG_BOOLEAN_MUSIC_IS_SPECIFIC_GENRE,
	LISTING_SONG_BOOLEAN_MUSIC_IS_SPECIFIC_TEMPO,
	LISTING_SONG_BOOLEAN_REVIEWER_IS_GIVING_FEEDBACK,
	LISTING_SONG_BOOLEAN_REVIEWER_IS_GIVING_RATING,
	LISTING_SONG_BOOLEAN_REVIEWER_IS_HAVING_KNOWN_TIMELINE,
	LISTING_SONG_BOOLEAN_REVIEWER_IS_SPECIFIED,
	LISTING_SONG_BOOLEAN_ARTIST_IS_FEATURED,
	LISTING_SONG_BOOLEAN_OPPORTUNITY_CONVEYING_HIGHEST_PROFESSIONALISM,
	LISTING_SONG_BOOLEAN_OPPORTUNITY_CONVEYING_APPRECIATION_TOWARDS_BEGINNERS,
	LISTING_SONG_BOOLEAN_OPPORTUNITY_CONVEYING_LACK_OF_CARE_FOR_THE_SONGWRITER,
	LISTING_SONG_BOOLEAN_OPPORTUNITY_CONVEYING_EASY_ACCEPTANCE,
	LISTING_SONG_BOOLEAN_OPPORTUNITY_CONVEYING_DIFFICULT_ACCEPTANCE,
	LISTING_SONG_BOOLEAN_CHANCES_IS_ACCEPTED,
	LISTING_SONG_BOOLEAN_CHANCES_IS_SUCCESSFUL_LAUNCH,
	LISTING_SONG_BOOLEAN_CHANCES_IS_HIT_SONG,
	LISTING_SONG_BOOLEAN_CHANCES_IS_RECORD_DEAL,
	LISTING_SONG_BOOLEAN_CHANCES_IS_RADIO_ROTATION,
	LISTING_SONG_BOOLEAN_CHANCES_IS_TV_ROTATION,
	LISTING_SONG_BOOLEAN_CHANCES_IS_RECEIVING_LEADS,
	LISTING_SONG_BOOLEAN_CHANCES_IS_COLLABORATION,
	LISTING_SONG_BOOLEAN_CHANCES_IS_PROMOTED,
	LISTING_SONG_BOOLEAN_CHANCES_IS_DISCOVERED_BY_PUBLISHERS,
	LISTING_SONG_BOOLEAN_CHANCES_IS_NETWORKING,
	
	LISTING_SONG_BOOLEAN_COUNT
};


inline String GetSongListingBooleanKey(int i) {
	switch (i) {
		case LISTING_SONG_BOOLEAN_IS_ALLOWED_MALE: return "the singer can be male";
		case LISTING_SONG_BOOLEAN_IS_ALLOWED_FEMALE: return "the singer can be female";
		case LISTING_SONG_BOOLEAN_ALLOWED_DEMO_QUALITY: return "demo quality is accepted";
		case LISTING_SONG_BOOLEAN_MUST_BE_BROADCAST_QUALITY: return "the song must be mastered and broadcast quality";
		case LISTING_SONG_BOOLEAN_MONETARY_SIGNIFICANT_INCOME: return "it is likely that the income could be significant";
		case LISTING_SONG_BOOLEAN_MONETARY_DIFFICULT_TO_DETERMINE: return "is the income difficult to determine";
		case LISTING_SONG_BOOLEAN_MONETARY_COULD_BE_ROYALTIES: return "could be opening up opportunities for royalties";
		case LISTING_SONG_BOOLEAN_MONETARY_FUTURE_COLLABS: return "could be opening future collaborations with music stars";
		case LISTING_SONG_BOOLEAN_IS_EXCLUSIVE: return "does the song have to be exclusive for this";
		case LISTING_SONG_BOOLEAN_PLACEMENT_TV: return "does the deal have a TV placement for the song";
		case LISTING_SONG_BOOLEAN_PLACEMENT_RADIO: return "does the deal have a radio placement for the song";
		case LISTING_SONG_BOOLEAN_PLACEMENT_MOVIE: return "does the deal have a movie placement for the song";
		case LISTING_SONG_BOOLEAN_PLACEMENT_AD: return "does the deal have an advertisement placement for the song";
		case LISTING_SONG_BOOLEAN_PLACEMENT_PLAYLIST: return "does the deal have a playlist placement for the song";
		case LISTING_SONG_BOOLEAN_LISTING_WRITER_IS_DECISION_MAKER: return "is the maker of this listing the decision maker for the song";
		case LISTING_SONG_BOOLEAN_LISTING_WRITER_IS_COMPANY: return "is the maker of this listing a company";
		case LISTING_SONG_BOOLEAN_LISTING_WRITER_IS_PERSON: return "is the maker of this listing a person with personal needs";
		case LISTING_SONG_BOOLEAN_MUSIC_IS_SPECIFIC_GENRE: return "does the song have to be in a specific genre";
		case LISTING_SONG_BOOLEAN_MUSIC_IS_SPECIFIC_TEMPO: return "does the song have to be in a specific tempo";
		case LISTING_SONG_BOOLEAN_REVIEWER_IS_GIVING_FEEDBACK: return "is the song reviewer giving feedback for the song";
		case LISTING_SONG_BOOLEAN_REVIEWER_IS_GIVING_RATING: return "is the song reviewer giving (star) rating for the song";
		case LISTING_SONG_BOOLEAN_REVIEWER_IS_HAVING_KNOWN_TIMELINE: return "is the timeline for the usage of the song known";
		case LISTING_SONG_BOOLEAN_REVIEWER_IS_SPECIFIED: return "is the reviewer specified in any way (e.g. being a team or a known person)";
		case LISTING_SONG_BOOLEAN_ARTIST_IS_FEATURED: return "is potential for artist to be featured";
		case LISTING_SONG_BOOLEAN_OPPORTUNITY_CONVEYING_HIGHEST_PROFESSIONALISM: return "is language and tone of listing conveying a highest level of professionalism";
		case LISTING_SONG_BOOLEAN_OPPORTUNITY_CONVEYING_APPRECIATION_TOWARDS_BEGINNERS: return "is language and tone of listing conveying appreciation towards beginners in songwriting";
		case LISTING_SONG_BOOLEAN_OPPORTUNITY_CONVEYING_LACK_OF_CARE_FOR_THE_SONGWRITER: return "is language and tone of listing conveying lacking care for the monetary needs of a songwriter";
		case LISTING_SONG_BOOLEAN_OPPORTUNITY_CONVEYING_EASY_ACCEPTANCE: return "is language and tone of listing conveying the message of easy acceptance for the song";
		case LISTING_SONG_BOOLEAN_OPPORTUNITY_CONVEYING_DIFFICULT_ACCEPTANCE: return "is language and tone of listing conveying the message of difficult acceptance for the song";
		case LISTING_SONG_BOOLEAN_CHANCES_IS_ACCEPTED: return "does this listing have increased chances of being accepted";
		case LISTING_SONG_BOOLEAN_CHANCES_IS_SUCCESSFUL_LAUNCH: return "does this listing have increased chances of launching a successful music career";
		case LISTING_SONG_BOOLEAN_CHANCES_IS_HIT_SONG: return "does this listing have increased chances of success for a hit song";
		case LISTING_SONG_BOOLEAN_CHANCES_IS_RECORD_DEAL: return "does this listing have increased chances of resulting in a record deal";
		case LISTING_SONG_BOOLEAN_CHANCES_IS_RADIO_ROTATION: return "does this listing have increased chances of getting an artist's song played on the radio";
		case LISTING_SONG_BOOLEAN_CHANCES_IS_TV_ROTATION: return "does this listing have increased chances of getting an artist's song placed in a TV show or movie";
		case LISTING_SONG_BOOLEAN_CHANCES_IS_RECEIVING_LEADS: return "does this listing have increased chances of receiving prime opportunities and leads in the music industry";
		case LISTING_SONG_BOOLEAN_CHANCES_IS_COLLABORATION: return "does this listing have increased chances of collaborating with successful writers and producers in the music industry";
		case LISTING_SONG_BOOLEAN_CHANCES_IS_PROMOTED: return "does this listing have increased chances of gaining exposure and promotion in the music industry";
		case LISTING_SONG_BOOLEAN_CHANCES_IS_DISCOVERED_BY_PUBLISHERS: return "does this listing have increased chances of being discovered by major record labels and publishers";
		case LISTING_SONG_BOOLEAN_CHANCES_IS_NETWORKING: return "does this listing have increased chances of receiving valuable networking opportunities in the music industry";
		default: return "";
	}
}


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
	
	Vector<int> analyzed_booleans;
	Vector<int> analyzed_string;
	Vector<int> analyzed_lists;
	Vector<int> analyzed_song_typecast;
	Vector<int> analyzed_song_content;
	Vector<int> analyzed_song_attrs;
	Vector<int> analyzed_song_colors;
	
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
