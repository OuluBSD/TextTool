#include "TextDatabase.h"


BEGIN_TEXTLIB_NAMESPACE


int PlatformComment::GetTotalComments() const {
	int t = 1;
	for (const PlatformComment& pc : responses)
		t += pc.GetTotalComments();
	return t;
}

int Entry::GetTotalComments() const {
	int t = 0;
	for (const PlatformComment& pc : comments)
		t += pc.GetTotalComments();
	return t;
}






const Vector<Platform>& GetPlatforms() {
	static Vector<Platform> a;
	if (!a.IsEmpty())
		return a;
	a.SetCount(PLATFORM_COUNT);
	{
		Platform& p = a[PLATFORM_EMAIL];
		p.name = "E-Mail";
		p.has_title = true;
		p.has_message = true;
		p.has_link_promotion = true;
	}
	{
		Platform& p = a[PLATFORM_TWITTER];
		p.name = "Twitter";
		p.has_comments = true;
		p.has_message = true;
		p.has_hashtags = true;
		p.has_video = true;
		p.has_reel = true;
		p.has_image = true;
		p.has_link_promotion = true;
		p.has_comment_self_promotion_hack = true;
	}
	{
		Platform& p = a[PLATFORM_THREADS];
		p.name = "Threads";
		p.has_comments = true;
		p.has_message = true;
		p.has_hashtags = true;
		p.has_video = true;
		p.has_reel = true;
		p.has_image = true;
		p.has_link_promotion = true;
		p.has_comment_self_promotion_hack = true;
	}
	{
		Platform& p = a[PLATFORM_FACEBOOK];
		p.name = "Facebook";
		p.has_comments = true;
		p.has_message = true;
		p.has_hashtags = true;
		p.has_video = true;
		p.has_reel = true;
		p.has_image = true;
		p.has_link_promotion = true;
		p.has_comment_self_promotion_hack = true;
	}
	{
		Platform& p = a[PLATFORM_INSTAGRAM];
		p.name = "Instagram";
		p.has_comments = true;
		p.has_message = true;
		p.has_hashtags = true;
		p.has_video = true;
		p.has_reel = true;
		p.has_image = true;
	}
	{
		Platform& p = a[PLATFORM_TIKTOK];
		p.name = "TikTok";
		p.has_comments = true;
		p.has_message = true;
		p.has_hashtags = true;
		p.has_reel = true;
	}
	{
		Platform& p = a[PLATFORM_GETTR];
		p.name = "Gettr";
		p.has_comments = true;
		p.has_message = true;
		p.has_hashtags = true;
		p.has_reel = true;
		p.has_image = true;
		p.has_link_promotion = true;
		p.has_comment_self_promotion_hack = true;
	}
	{
		Platform& p = a[PLATFORM_LINKEDIN];
		p.name = "LinkedIn";
		p.has_comments = true;
		p.has_message = true;
		p.has_hashtags = true;
		p.has_video = true;
		p.has_image = true;
	}
	{
		Platform& p = a[PLATFORM_SOUNDCLOUD];
		p.name = "Soundcloud";
		p.has_comments = true;
		p.has_title = true;
		p.has_hashtags = true;
		p.has_audio = true;
		p.has_music = true;
		p.has_image = true;
		p.has_comment_self_promotion_hack = true;
	}
	{
		Platform& p = a[PLATFORM_MUSIC_DISTRIBUTOR];
		p.name = "Music Distributor";
		p.has_title = true;
		p.has_music = true;
		p.has_image = true;
	}
	{
		Platform& p = a[PLATFORM_YOUTUBE];
		p.name = "YouTube";
		p.has_comments = true;
		p.has_message = true;
		p.has_title = true;
		p.has_hashtags = true;
		p.has_video = true;
		p.has_reel = true;
		p.has_image = true;
	}
	{
		Platform& p = a[PLATFORM_VK];
		p.name = "VK";
		p.has_comments = true;
		p.has_message = true;
		p.has_hashtags = true;
		p.has_video = true;
		p.has_reel = true;
		p.has_image = true;
		p.has_link_promotion = true;
		p.has_comment_self_promotion_hack = true;
	}
	{
		Platform& p = a[PLATFORM_REDDIT];
		p.name = "Reddit";
		p.has_comments = true;
		p.has_message = true;
		p.has_title = true;
		p.has_link_promotion = true;
		p.has_comment_self_promotion_hack = true;
	}
	{
		Platform& p = a[PLATFORM_FORUM];
		p.name = "Forum";
		p.has_comments = true;
		p.has_message = true;
		p.has_title = true;
		p.has_link_promotion = true;
		p.has_comment_self_promotion_hack = true;
	}
	{
		Platform& p = a[PLATFORM_BLOGGER];
		p.name = "Blogger";
		p.has_comments = true;
		p.has_message = true;
		p.has_title = true;
		p.has_hashtags = true;
		p.has_audio = true;
		p.has_music = true;
		p.has_video = true;
		p.has_reel = true;
		p.has_image = true;
		p.has_link_promotion = true;
		p.has_comment_self_promotion_hack = true;
	}
	{
		Platform& p = a[PLATFORM_WEBSITE];
		p.name = "Website";
		p.has_message = true;
		p.has_title = true;
		p.has_hashtags = true;
		p.has_audio = true;
		p.has_music = true;
		p.has_video = true;
		p.has_image = true;
		p.has_link_promotion = true;
		p.has_comment_self_promotion_hack = true;
	}
	{
		Platform& p = a[PLATFORM_TWITCH];
		p.name = "Twitch";
		p.has_comments = true;
		p.has_video = true;
	}
	{
		Platform& p = a[PLATFORM_STUMBLE];
		p.name = "Stumble";
		p.has_link_promotion = true;
	}
	{
		Platform& p = a[PLATFORM_GITHUB];
		p.name = "GitHub";
	}
	{
		Platform& p = a[PLATFORM_MYSPACE];
		p.name = "MySpace";
		p.has_comments = true;
		p.has_title = true;
		p.has_message = true;
		p.has_hashtags = true;
		p.has_music = true;
		p.has_image = true;
		p.has_link_promotion = true;
		p.has_comment_self_promotion_hack = true;
	}
	{
		Platform& p = a[PLATFORM_MIKSERINET];
		p.name = "Mikseri.net";
		p.has_comments = true;
		p.has_title = true;
		p.has_message = true;
		p.has_hashtags = true;
		p.has_music = true;
		p.has_image = true;
		p.has_link_promotion = true;
		p.has_comment_self_promotion_hack = true;
	}
	{
		Platform& p = a[PLATFORM_IRCGALLERIA];
		p.name = "Irc-Galleria";
		p.has_comments = true;
		p.has_hashtags = true;
		p.has_image = true;
		p.has_link_promotion = true;
		p.has_comment_self_promotion_hack = true;
	}
	{
		Platform& p = a[PLATFORM_DISCORD];
		p.name = "Discord";
		p.has_comments = true;
		p.has_hashtags = true;
		p.has_audio = true;
		p.has_image = true;
		p.has_link_promotion = true;
		p.has_comment_self_promotion_hack = true;
	}
	
	
	
	return a;
}

void ProfileData::Jsonize(JsonIO& json) {
	json
		("platforms", platforms)
		;
}

void ProfileData::Load() {
	String dir = AppendFileName(MetaDatabase::Single().dir, "share-common");
	String fname = IntStr64(hash) + ".json";
	String path = AppendFileName(dir, fname);
	
	LoadFromJsonFileStandard(*this, path);
}

void ProfileData::Store() {
	String dir = AppendFileName(MetaDatabase::Single().dir, "share-common");
	String fname = IntStr64(hash) + ".json";
	String path = AppendFileName(dir, fname);
	
	RealizeDirectory(dir);
	StoreAsJsonFileStandard(*this, path);
}

Array<ProfileData>& ProfileData::GetAll() {
	static Array<ProfileData> a;
	return a;
}

ProfileData& ProfileData::Get(Profile& p) {
	Array<ProfileData>& a = GetAll();
	CombineHash ch;
	ch.Do(p.owner->file_title);
	ch.Do(p.name);
	hash_t h = ch;
	for (ProfileData& pd : a) {
		if (pd.hash == h) {
			ASSERT(pd.profile == &p);
			pd.platforms.SetCount(PLATFORM_COUNT);
			return pd;
		}
	}
	ProfileData& pd = a.Add();
	pd.hash = h;
	pd.Load();
	pd.platforms.SetCount(PLATFORM_COUNT);
	pd.profile = &p;
	return pd;
}

void ProfileData::StoreAll() {
	for (ProfileData& pd : GetAll())
		pd.Store();
}


END_TEXTLIB_NAMESPACE

