#ifndef _TextDatabase_BiographyAnalysis_h_
#define _TextDatabase_BiographyAnalysis_h_


BEGIN_TEXTLIB_NAMESPACE


/*	These have been made only for male-perspective because of the lack of time.
	Also, making the profiles for roles is really exhausting manual labor...
*/


#define SOCIETYROLE_LIST \
	SOCIETYROLE(WIFE) \
	SOCIETYROLE(BEST_MAN) \
	SOCIETYROLE(MOTHER) \
	SOCIETYROLE(FATHER) \
	SOCIETYROLE(LITTLE_SISTER) \
	SOCIETYROLE(LITTLE_BROTHER) \
	SOCIETYROLE(BIG_SISTER) \
	SOCIETYROLE(BIG_BROTHER) \
	SOCIETYROLE(MATERNAL_GRANDMOTHER) \
	SOCIETYROLE(MATERNAL_GRANDFATHER) \
	SOCIETYROLE(PATERNAL_GRANDMOTHER) \
	SOCIETYROLE(PATERNAL_GRANDFATHER) \
	SOCIETYROLE(MATERNAL_AUNT) \
	SOCIETYROLE(MATERNAL_UNCLE) \
	SOCIETYROLE(PATERNAL_AUNT) \
	SOCIETYROLE(PATERNAL_UNCLE) \
	SOCIETYROLE(MATERNAL_FEMALE_COUSIN) \
	SOCIETYROLE(MATERNAL_MALE_COUSIN) \
	SOCIETYROLE(PATERNAL_FEMALE_COUSIN) \
	SOCIETYROLE(PATERNAL_MALE_COUSIN) \
	\
	SOCIETYROLE(FEMALE_IN_INTERNET_SHALLOW_IMAGE_SITE) \
	SOCIETYROLE(  MALE_IN_INTERNET_SHALLOW_IMAGE_SITE) \
	SOCIETYROLE(FEMALE_IN_INTERNET_SHALLOW_REEL_VIDEO_SITE) \
	SOCIETYROLE(  MALE_IN_INTERNET_SHALLOW_REEL_VIDEO_SITE) \
	SOCIETYROLE(FEMALE_IN_INTERNET_SHALLOW_PUBLIC_MESSAGE_SITE) \
	SOCIETYROLE(  MALE_IN_INTERNET_SHALLOW_PUBLIC_MESSAGE_SITE) \
	SOCIETYROLE(FEMALE_IN_INTERNET_SHALLOW_VIDEO_STREAMING_SITE) \
	SOCIETYROLE(  MALE_IN_INTERNET_SHALLOW_VIDEO_STREAMING_SITE) \
	SOCIETYROLE(FEMALE_IN_INTERNET_MODERATE_FULL_PROFILE_SITE) \
	SOCIETYROLE(  MALE_IN_INTERNET_MODERATE_FULL_PROFILE_SITE) \
	SOCIETYROLE(FEMALE_IN_INTERNET_MODERATE_VIDEO_SITE) \
	SOCIETYROLE(  MALE_IN_INTERNET_MODERATE_VIDEO_SITE) \
	SOCIETYROLE(FEMALE_IN_INTERNET_PROFSSIONAL_INDUSTRY_SITE) \
	SOCIETYROLE(  MALE_IN_INTERNET_PROFSSIONAL_INDUSTRY_SITE) \
	SOCIETYROLE(FEMALE_IN_INTERNET_SOCIAL_MUSIC_SITE) \
	SOCIETYROLE(  MALE_IN_INTERNET_SOCIAL_MUSIC_SITE) \
	SOCIETYROLE(FEMALE_IN_INTERNET_SOCIAL_PROGRAMMING_SITE) \
	SOCIETYROLE(  MALE_IN_INTERNET_SOCIAL_PROGRAMMING_SITE) \
	SOCIETYROLE(FEMALE_IN_INTERNET_FORUM_MUSIC) \
	SOCIETYROLE(  MALE_IN_INTERNET_FORUM_MUSIC) \
	SOCIETYROLE(FEMALE_IN_INTERNET_FORUM_PROGRAMMING) \
	SOCIETYROLE(  MALE_IN_INTERNET_FORUM_PROGRAMMING) \
	SOCIETYROLE(FEMALE_IN_INTERNET_WEBSITE_READER) /* a person who reads a website */ \
	SOCIETYROLE(  MALE_IN_INTERNET_WEBSITE_READER) \
	SOCIETYROLE(FEMALE_IN_INTERNET_MUSIC_COOPERATION_SITE) \
	SOCIETYROLE(  MALE_IN_INTERNET_MUSIC_COOPERATION_SITE) \
	SOCIETYROLE(FEMALE_IN_INTERNET_PROGRAMMING_COOPERATION_SITE) \
	SOCIETYROLE(  MALE_IN_INTERNET_PROGRAMMING_COOPERATION_SITE) \
	\
	SOCIETYROLE(FEMALE_PEER_IN_MILITARY_SERVICE) \
	SOCIETYROLE(  MALE_PEER_IN_MILITARY_SERVICE) \
	SOCIETYROLE(FEMALE_SUPERIOR_IN_MILITARY_SERVICE) \
	SOCIETYROLE(  MALE_SUPERIOR_IN_MILITARY_SERVICE) \
	SOCIETYROLE(FEMALE_INFERIOR_IN_MILITARY_SERVICE) \
	SOCIETYROLE(  MALE_INFERIOR_IN_MILITARY_SERVICE) \
	\
	SOCIETYROLE(DAUGHTER) \
	SOCIETYROLE(SON) \
	SOCIETYROLE(FEMALE_RECRUITER_FOR_WORK) \
	SOCIETYROLE(  MALE_RECRUITER_FOR_WORK) \
	SOCIETYROLE(FEMALE_IN_INTERNET_REPUBLICAN_PUBLIC_MESSAGE_SITE) \
	SOCIETYROLE(  MALE_IN_INTERNET_REPUBLICAN_PUBLIC_MESSAGE_SITE) \
	SOCIETYROLE(FEMALE_IN_INTERNET_INFLUENCER_FOR_ART_AND_MUSIC) \
	SOCIETYROLE(  MALE_IN_INTERNET_INFLUENCER_FOR_ART_AND_MUSIC) \



enum {
	#define SOCIETYROLE(x) SOCIETYROLE_##x,
	SOCIETYROLE_LIST
	#undef SOCIETYROLE
	
	SOCIETYROLE_COUNT
};

String GetSocietyRoleEnum(int i);
String GetSocietyRoleKey(int i);
String GetSocietyRoleDescription(int i);


struct RoleProfile {
	String name, profile;
	
	
	void Set(String name, String profile) {this->name = name; this->profile = profile;}
	
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			;
	}
};

const Array<RoleProfile>& GetRoleProfile(int role_i);


enum {
	BIOSCORE_EMPATHY,
	BIOSCORE_ATTRACTION,
	BIOSCORE_VALUE,
	BIOSCORE_LEADERSHIP,
	
	BIOSCORE_COUNT
};

struct BiographyProfileAnalysis {
	struct Response : Moveable<Response> {
		int year, category;
		String text, keywords;
		double score[BIOSCORE_COUNT] = {0,0,0,0};
		void Jsonize(JsonIO& json) {
			json
				("year", year)
				("cat", category)
				("txt", text)
				("keyw", keywords)
				;
			for(int i = 0; i < BIOSCORE_COUNT; i++)
				json("sc" + IntStr(i), score[i]);
		}
	};
	
	Vector<Response> responses;
	VectorMap<int, String> categories;
	String biography_reaction;
	
	void Jsonize(JsonIO& json) {
		json
			("responses", responses)
			("categories", categories)
			("biography_reaction", biography_reaction)
			;
	}
};

struct BiographyAnalysis {
	Array<Array<BiographyProfileAnalysis>> profiles;
	
	void Realize();
	void Jsonize(JsonIO& json) {
		json
			("profiles", profiles)
			;
	}
	//BiographyCategory& GetAdd(Owner& o, int enum_);
	
};


END_TEXTLIB_NAMESPACE


#endif