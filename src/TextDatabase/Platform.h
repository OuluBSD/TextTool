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
	PLATFORM_BANDMIX,
	PLATFORM_BANDCAMP,
	PLATFORM_REMOTEMORE,
	PLATFORM_KUVAKENET,
	PLATFORM_REVERBNATION,
	PLATFORM_SONICBIDS,
	PLATFORM_MUSICGATEWAY,
	PLATFORM_INDIEONTHEMOVE,
	PLATFORM_VOWAVE, // vowave.com
	PLATFORM_SOUNDMASH, // soundmash.me
	PLATFORM_AUDIUS, // audius.co
	PLATFORM_INSTRUMENTAL, // https://weareinstrumental.com/
	PLATFORM_SONGTRADR, // songtradr.com
	PLATFORM_DISCOGS, // discogs.com
	PLATFORM_ALLMUSIC, // allmusic.com
	PLATFORM_GROOVER, // groover.co
	PLATFORM_AIRPLAYDIRECT,
	PLATFORM_N1M, // n1m.com
	PLATFORM_SOUNDBETTER, // soundbetter.com
	PLATFORM_ABOUTME, // about.me
	PLATFORM_FIVERR, // fiverr.com
	PLATFORM_THEDOTS, // the-dots.com
	
	PLATFORM_COUNT
};



struct Platform : Moveable<Platform> {
	String name;
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
	bool has_comment_self_promotion_hack = false;
	
	
	Vector<int> roles;
	
	void AddRole(int i);
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


END_TEXTLIB_NAMESPACE


#endif
