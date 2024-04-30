#include "TextDatabase.h"


BEGIN_TEXTLIB_NAMESPACE


Platform::Platform() {}


const Array<Platform>& GetPlatforms() {
	static Array<Platform> a;
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


END_TEXTLIB_NAMESPACE

