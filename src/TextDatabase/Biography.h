#ifndef _TextDatabase_Biography_h_
#define _TextDatabase_Biography_h_


BEGIN_TEXTLIB_NAMESPACE

#define BIOCATEGORY_LIST \
	BIOCATEGORY(HOME) \
	BIOCATEGORY(HOME_KITCHEN) \
	BIOCATEGORY(HOME_WORKDESK) \
	BIOCATEGORY(HOME_BEDROOM) \
	BIOCATEGORY(HOME_COMPUTER) \
	BIOCATEGORY(PEOPLE_FRIENDS) \
	BIOCATEGORY(PEOPLE_CLOSE_FRIENDS) \
	BIOCATEGORY(PEOPLE_ENEMIES) \
	BIOCATEGORY(PEOPLE_PUBLIC_IN_GENERAL) \
	BIOCATEGORY(PEOPLE_SYSTEMIC_ENEMIES) \
	BIOCATEGORY(SEXUALITY_ADULT_ENTERNTAINMENT) \
	BIOCATEGORY(SEXUALITY_EXPERIENCES) \
	BIOCATEGORY(EDUCATION_CAMPUS) \
	BIOCATEGORY(EDUCATION_HOME) \
	BIOCATEGORY(EDUCATION_INTERNET) \
	BIOCATEGORY(EDUCATION_MUSIC_PRODUCTION) \
	BIOCATEGORY(FAMILY_PARENTAL) \
	BIOCATEGORY(FAMILY_OWN) \
	BIOCATEGORY(EVENTS_MUSIC) \
	BIOCATEGORY(EVENTS_SPORT) \
	BIOCATEGORY(EVENTS_FOOD) \
	BIOCATEGORY(EVENTS_OTHER) \
	BIOCATEGORY(POLITICAL) \
	BIOCATEGORY(POLITICAL_PIRATISM) \
	BIOCATEGORY(PERSONAL_TASTE_FOOD) \
	BIOCATEGORY(PERSONAL_TASTE_ENTERNTAINMENT) \
	BIOCATEGORY(PERSONAL_TASTE_VIDEOGAMES) \
	BIOCATEGORY(PERSONAL_TASTE_MOVIES) \
	BIOCATEGORY(PERSONAL_TASTE_TV) \
	BIOCATEGORY(PERSONAL_TASTE_BOOKS) \
	BIOCATEGORY(PERSONAL_TASTE_WORLD_INFORMATION) \
	BIOCATEGORY(PERSONAL_TASTE_HOBBIES) \
	BIOCATEGORY(PERSONAL_TASTE_PHYSICAL_EXERCISE) \
	BIOCATEGORY(PERSONAL_TASTE_ADVENTURES) \
	BIOCATEGORY(PERSONAL_TASTE_YOUTUBE) \
	BIOCATEGORY(PERSONAL_TASTE_MUSIC) \
	BIOCATEGORY(TRADING_BUYING) \
	BIOCATEGORY(TRADING_SELLING) \
	BIOCATEGORY(HEALTH) \
	BIOCATEGORY(HEALTH_SUPPLEMENTS) \
	BIOCATEGORY(HEALTH_MEDICATION) \
	BIOCATEGORY(HEALTH_DOCTOR) \
	BIOCATEGORY(HEALTH_OPERATIONS) \
	BIOCATEGORY(ANIMALS_PETS) \
	BIOCATEGORY(ANIMALS_WILD) \
	BIOCATEGORY(WORK_PROGRAMMING) \
	BIOCATEGORY(WORK_MUSIC_COLLABORATION) \
	BIOCATEGORY(WORK_MUSIC_PRODUCTION) \
	BIOCATEGORY(WORK_SONGWRITING) \
	BIOCATEGORY(BADGE_OF_HONOR) \
	BIOCATEGORY(BADGE_OF_SHAME) \
	BIOCATEGORY(BADGE_OF_JUDGEMENT) \
	BIOCATEGORY(POLITICAL_IDENTITY) \



enum {
	#define BIOCATEGORY(x) BIOCATEGORY_##x,
	BIOCATEGORY_LIST
	#undef BIOCATEGORY
	
	BIOCATEGORY_COUNT
};

String KeyToName(String s);
String GetBiographyCategoryEnum(int i);
String GetBiographyCategoryKey(int i);
const char* GetBiographyCategoryEnumCstr(int i);
int FindBiographyCategoryEnum(String s);


enum {
	TIME_ACCURACY_NONE,
	TIME_ACCURACY_DATE,
	TIME_ACCURACY_TIME,
	
	TIME_ACCURACY_COUNT,
};

struct BioImage {
	String keywords, text, native_text;
	String image_keywords, image_text;
	String location;
	String people;
	Time time;
	int time_accuracy = TIME_ACCURACY_NONE;
	int64 image_hash = 0;
	
	void Jsonize(JsonIO& json) {
		json
			("keywords", keywords)
			("text", text)
			("native_text", native_text)
			("image_keywords", image_keywords)
			("image_text", image_text)
			("location", location)
			("people", people)
			("time", time)
			("time_accuracy", time_accuracy)
			("image_hash", image_hash)
			;
	}
};

struct BioRange : Moveable<BioRange> {
	int off = 0, len = 0;
	BioRange() {}
	BioRange(BioRange&& r) {*this = r;}
	BioRange(const BioRange& r) {*this = r;}
	void operator=(const BioRange& r) {off = r.off; len = r.len;}
	hash_t GetHashValue() const {CombineHash c; c.Do(off).Do(len); return c;}
	bool operator==(const BioRange& r) const {return r.off == off && r.len == len;}
	void Jsonize(JsonIO& json) {json("off", off)("len", len);}
	bool operator()(const BioRange& a, const BioRange& b) const {
		int a0 = a.off + a.len - 1; // last item in range
		int b0 = b.off + b.len - 1;
		if (a0 != b0) return a0 < b0; // sort primarily by lesser last item
		return a.len < b.len; // otherwise sort by lesser range length
	}
};


struct BioYear {
	int year = 0;
	String keywords, text, native_text;
	Array<BioImage> images;
	ArrayMap<BioRange,BioImage> image_summaries;
	String image_text;
	
	void Jsonize(JsonIO& json) {
		json
			("year", year)
			("keywords", keywords)
			("native_text", native_text)
			("text", text)
			("images", images)
			("image_summaries", image_summaries)
			("image_text", image_text)
			;
	}
	bool operator()(const BioYear& a, const BioYear& b) const {return a.year < b.year;}
	void RealizeImageSummaries();
	BioImage& GetAddImageSummary(int begin_year, int years);
};

struct BiographyCategory {
	
	Array<BioYear> years;
	ArrayMap<BioRange,BioYear> summaries;
	
	
	
	BiographyCategory() {}
	
	void Jsonize(JsonIO& json) {
		json
			("years", years)
			("summaries", summaries)
			;
	}
	BioYear& GetAdd(int year);
	int GetFilledCount() const;
	int GetFilledImagesCount() const;
	void RealizeSummaries();
	BioYear& GetAddSummary(int begin_year, int years);
	
};

struct Biography {
private:
	ArrayMap<String, BiographyCategory> categories;
	
public:
	struct CatSorter {
		bool operator()(const String& a, const String& b) const {
			int ai = FindBiographyCategoryEnum(a);
			int bi = FindBiographyCategoryEnum(b);
			if (bi == -1) return true;
			if (ai == -1) return false;
			return ai < bi;
		}
	};
	
	void Jsonize(JsonIO& json) {
		json
			("categories", categories)
			;
		if (json.IsLoading()) {
			Sort();
		}
	}
	BiographyCategory& GetAdd(Owner& o, int enum_);
	void Sort() {
		ArrayMap<String, BiographyCategory> tmp;
		Swap(tmp, categories);
		for(int i = 0; i < BIOCATEGORY_COUNT; i++) {
			String key = GetBiographyCategoryEnumCstr(i);
			int j = tmp.Find(key);
			if (j >= 0) {
				BiographyCategory& cat = tmp[j];
				Swap(categories.Add(key), cat);
			}
			else categories.Add(key);
		}
	}
};



END_TEXTLIB_NAMESPACE


#endif
