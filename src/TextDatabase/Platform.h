#ifndef _TextDatabase_Platform_h_
#define _TextDatabase_Platform_h_


BEGIN_TEXTLIB_NAMESPACE


enum {
	PLATFORM_EMAIL,
	PLATFORM_TWITTER,
	PLATFORM_THREADS,
	PLATFORM_FACEBOOK,
	PLATFORM_INSTAGRAM,
	
	PLATFORM_TIKTOK,
	PLATFORM_GETTR,
	PLATFORM_LINKEDIN,
	PLATFORM_SOUNDCLOUD,
	PLATFORM_MUSIC_DISTRIBUTOR, // e.g. distrokid
	
	
	PLATFORM_YOUTUBE,
	PLATFORM_VK,
	PLATFORM_REDDIT,
	PLATFORM_FORUM,
	PLATFORM_BLOGGER,
	
	PLATFORM_WEBSITE,
	PLATFORM_TWITCH,
	PLATFORM_STUMBLE,
	PLATFORM_GITHUB,
	PLATFORM_MYSPACE,
	
	
	PLATFORM_MIKSERINET,
	PLATFORM_IRCGALLERIA,
	PLATFORM_DISCORD,
	PLATFORM_MUKKEN,
	PLATFORM_BANDCAMP,
	
	PLATFORM_REMOTEMORE,
	PLATFORM_KUVAKENET,
	PLATFORM_REVERBNATION,
	PLATFORM_SONICBIDS,
	PLATFORM_MUSICGATEWAY,
	
	
	PLATFORM_INDIEONTHEMOVE,
	PLATFORM_VOWAVE, // vowave.com
	PLATFORM_AUDIUS, // audius.co
	PLATFORM_SONGTRADR, // songtradr.com
	PLATFORM_GROOVER, // groover.co
	
	PLATFORM_AIRPLAYDIRECT,
	PLATFORM_N1M, // n1m.com
	PLATFORM_SOUNDBETTER, // soundbetter.com
	PLATFORM_ABOUTME, // about.me
	PLATFORM_FIVERR, // fiverr.com
	
	
	PLATFORM_THEDOTS, // the-dots.com
	PLATFORM_CONSTANTCONTACT, // constantcontact.com
	PLATFORM_MUUSIKOIDEN_NET,
	PLATFORM_PODCAST,
	PLATFORM_TINDER,
	
	PLATFORM_PATREON,
	PLATFORM_LOCALS,
	
	
	// PLATFORM_SOUNDMASH, // soundmash.me
	// PLATFORM_INSTRUMENTAL, // https://weareinstrumental.com/
	// PLATFORM_DISCOGS, // discogs.com
	// PLATFORM_ALLMUSIC, // allmusic.com
	
	
	PLATFORM_COUNT
};



#define PLATFORM_PROFILE_LIST \
	PLATFORM_PROFILE(ANY) \
	PLATFORM_PROFILE(MUSIC_ARTIST) \
	PLATFORM_PROFILE(VISUAL_ARTIST) \
	PLATFORM_PROFILE(PHOTOGRAPHER) \
	PLATFORM_PROFILE(REAL_PERSON) \
	
enum {
	#define PLATFORM_PROFILE(x) PLATFORM_PROFILE_##x,
	PLATFORM_PROFILE_LIST
	#undef PLATFORM_PROFILE
	
	PLATFORM_PROFILE_COUNT,
};

const char* GetPlatformProfileEnum(int i);
String GetPlatformProfileKey(int i);

#define PLATFORM_ATTR_LIST \
	PLATFORM_ATTR(TITLE) \
	PLATFORM_ATTR(MESSAGE) \
	PLATFORM_ATTR(HASHTAGS) \
	PLATFORM_ATTR(AUDIO) \
	PLATFORM_ATTR(MUSIC) \
	PLATFORM_ATTR(VIDEO) \
	PLATFORM_ATTR(REEL) \
	PLATFORM_ATTR(IMAGE) \
	PLATFORM_ATTR(LINK_PROMOTION) \
	PLATFORM_ATTR(COMMENTS) \
	PLATFORM_ATTR(COMMENT_SELF_PROMOTION_HACK) \
	PLATFORM_ATTR(DESCRIPTION) \
	PLATFORM_ATTR(PROFILE_IMAGE) \
	PLATFORM_ATTR(MUSIC_COVER) \
	PLATFORM_ATTR(Q_AND_A) \
	PLATFORM_ATTR(Q_AND_A_HACK) \
	PLATFORM_ATTR(TESTIMONIAL_HACK) \


enum {
	#define PLATFORM_ATTR(x) PLATFORM_ATTR_##x,
	PLATFORM_ATTR_LIST
	#undef PLATFORM_ATTR
	
	PLATFORM_ATTR_COUNT,
};

const char* GetPlatformAttrEnum(int i);
String GetPlatformAttrKey(int i);

struct Platform : Moveable<Platform> {
	const char* group = 0;
	const char* name = 0;
	const char* description = 0;
	int profile_type = PLATFORM_PROFILE_ANY;
	bool attrs[PLATFORM_ATTR_COUNT];
	Vector<String> functions;
	
	Platform() {memset(attrs, 0, sizeof(attrs));}
	
	void SetAttr(String name, bool value);
	
	Platform& operator << (const char* fn) {functions << fn; return *this;}
	
	/*String name;
	bool has_title = false;
	bool has_message = false;
	bool has_hashtags = false;
	bool has_audio = false;
	bool has_music = false;
	bool has_video = false;
	bool has_reel = false;
	bool has_image = false;
	bool has_link_promotion = false;
	bool has_comments = false;
	bool has_comment_self_promotion_hack = false; // it's a unofficial policy to self-promote in comments
	
	bool has_description = false;
	bool has_profile_image = false;
	bool has_music_cover = false;
	bool has_Q_and_A = false;
	bool has_Q_and_A_hack = false;
	bool has_policy_real_person = false;
	bool has_policy_artist_only = false;
	bool has_testimonial_hack = false; // real testimonials, but injected inside normal text
	
	Vector<int> roles;
	
	void AddRole(int i);*/
};

const Vector<Platform>& GetPlatforms();


struct ProfileData {
	hash_t hash = 0;
	Array<PlatformData> platforms;
	
	Profile* profile = 0;
	
	
	void Jsonize(JsonIO& json);
	void Load();
	void Store();
	
	static void StoreAll();
	static Array<ProfileData>& GetAll();
	static ProfileData& Get(Profile& p);
	
	
};


struct PlatformAnalysis {
	Index<int> roles;
	VectorMap<String,String> epk_text_fields;
	VectorMap<String,PlatformAnalysisPhoto> epk_photos;
	
	
	void Jsonize(JsonIO& json) {
		json
			("roles", roles)
			("epk_text_fields", epk_text_fields)
			("epk_photos", epk_photos)
			;
	}
	
	int GetRoleScoreSum(int score_i) const;
	double GetRoleScoreSumWeighted(int score_i) const;
	
};


END_TEXTLIB_NAMESPACE


#endif
