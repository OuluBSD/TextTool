#include "TextDatabase.h"


BEGIN_TEXTLIB_NAMESPACE


const char* GetPlatformProfileEnum(int i) {
	switch (i) {
		#define PLATFORM_PROFILE(x) case PLATFORM_PROFILE_##x: return #x;
		PLATFORM_PROFILE_LIST
		#undef PLATFORM_PROFILE
		default: return "error";
	}
}

String GetPlatformProfileKey(int i) {return KeyToName(GetPlatformProfileEnum(i));}




const char* GetPlatformAttrEnum(int i) {
	switch (i) {
		#define PLATFORM_ATTR(x) case PLATFORM_ATTR_##x: return #x;
		PLATFORM_ATTR_LIST
		#undef PLATFORM_ATTR
		default: return "error";
	}
}

String GetPlatformAttrKey(int i) {return KeyToName(GetPlatformAttrEnum(i));}



/*void Platform::AddRole(int i) {
	ASSERT(i >= 0 && i < SOCIETYROLE_COUNT); roles << i;
}*/

void Platform::SetAttr(String name, bool value) {
	if (name.Left(4) == "has_") name = name.Mid(4);
	name = ToUpper(name);
	for(int i = 0; i < PLATFORM_ATTR_COUNT; i++) {
		const char* key = GetPlatformAttrEnum(i);
		if (name == key) {
			attrs[i] = value;
			return;
		}
	}
	Panic("Internal error: key not found: " + name);
}



int PlatformComment::GetTotalComments() const {
	int t = 1;
	for (const PlatformComment& pc : responses)
		t += pc.GetTotalComments();
	return t;
}

int PlatformThread::GetTotalComments() const {
	int t = 0;
	for (const PlatformComment& pc : comments)
		t += pc.GetTotalComments();
	return t;
}




#define ENABLE(x) p.SetAttr(#x, true);

const Vector<Platform>& GetPlatforms() {
	static Vector<Platform> a;
	if (!a.IsEmpty())
		return a;
	a.SetCount(PLATFORM_COUNT);
	{
		Platform& p = a[PLATFORM_EMAIL];
		p.profile_type = PLATFORM_PROFILE_REAL_PERSON;
		p.name = "E-Mail";
		p.group = "Direct";
		p.description = "A platform for sending and receiving electronic mail messages.";
		ENABLE(has_title);
		ENABLE(has_message);
		ENABLE(has_link_promotion);
	}
	{
		Platform& p = a[PLATFORM_TWITTER];
		p.name = "Twitter";
		p.group = "Short message site";
		p.description = "A social media platform for microblogging and sharing short messages or \"tweets.\"";
		ENABLE(has_profile_image);
		ENABLE(has_description);
		ENABLE(has_comments);
		ENABLE(has_message);
		ENABLE(has_hashtags);
		ENABLE(has_video);
		ENABLE(has_reel);
		ENABLE(has_image);
		ENABLE(has_link_promotion);
		ENABLE(has_comment_self_promotion_hack);
		ENABLE(has_Q_and_A_hack);
		ENABLE(has_testimonial_hack);
	}
	{
		Platform& p = a[PLATFORM_THREADS];
		p.name = "Threads";
		p.group = "Short message site";
		p.description = "The Facebook owner Meta's clone of Twitter.";
		ENABLE(has_profile_image);
		ENABLE(has_description);
		ENABLE(has_comments);
		ENABLE(has_message);
		ENABLE(has_hashtags);
		ENABLE(has_video);
		ENABLE(has_reel);
		ENABLE(has_image);
		ENABLE(has_link_promotion);
		ENABLE(has_comment_self_promotion_hack);
		ENABLE(has_Q_and_A_hack);
		ENABLE(has_testimonial_hack);
	}
	{
		Platform& p = a[PLATFORM_FACEBOOK];
		p.name = "Facebook";
		p.group = "Full profile site";
		p.description = "A social networking platform for connecting with friends and sharing updates, photos, and other content.";
		ENABLE(has_profile_image);
		ENABLE(has_description);
		ENABLE(has_comments);
		ENABLE(has_message);
		ENABLE(has_hashtags);
		ENABLE(has_video);
		ENABLE(has_reel);
		ENABLE(has_image);
		ENABLE(has_link_promotion);
		ENABLE(has_comment_self_promotion_hack);
		ENABLE(has_Q_and_A_hack);
		ENABLE(has_testimonial_hack);
	}
	{
		Platform& p = a[PLATFORM_INSTAGRAM];
		p.name = "Instagram";
		p.group = "Image site";
		p.description = "A visual-based social networking platform for sharing photos and videos.";
		ENABLE(has_profile_image);
		ENABLE(has_description);
		ENABLE(has_comments);
		ENABLE(has_message);
		ENABLE(has_hashtags);
		ENABLE(has_video);
		ENABLE(has_reel);
		ENABLE(has_image);
		ENABLE(has_comment_self_promotion_hack);
		ENABLE(has_Q_and_A_hack);
		ENABLE(has_testimonial_hack);
	}
	{
		Platform& p = a[PLATFORM_TIKTOK];
		p.name = "TikTok";
		p.group = "Video site";
		p.description = "A video-sharing platform for creating short and creative videos.";
		ENABLE(has_profile_image);
		ENABLE(has_description);
		ENABLE(has_comments);
		ENABLE(has_message);
		ENABLE(has_hashtags);
		ENABLE(has_reel);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_SHALLOW_REEL_VIDEO_SITE); // TikTok,_Instagram
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_SHALLOW_REEL_VIDEO_SITE);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_SHALLOW_IMAGE_SITE); // Instagram
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_SHALLOW_IMAGE_SITE);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_SHALLOW_PUBLIC_MESSAGE_SITE); // Twitter
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_SHALLOW_PUBLIC_MESSAGE_SITE);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_SHALLOW_VIDEO_STREAMING_SITE); // Twitch
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_SHALLOW_VIDEO_STREAMING_SITE);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_FORUM_MUSIC);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_FORUM_MUSIC);
	}
	{
		Platform& p = a[PLATFORM_GETTR];
		p.name = "Gettr";
		p.group = "Short message";
		p.description = "A newer social media platform that promotes free speech and conservative viewpoints.";
		p.profile_type = PLATFORM_PROFILE_REAL_PERSON;
		ENABLE(has_profile_image);
		ENABLE(has_description);
		ENABLE(has_comments);
		ENABLE(has_message);
		ENABLE(has_hashtags);
		ENABLE(has_reel);
		ENABLE(has_image);
		ENABLE(has_link_promotion);
		ENABLE(has_comment_self_promotion_hack);
		ENABLE(has_Q_and_A_hack);
		ENABLE(has_testimonial_hack);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_FORUM_PROGRAMMING);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_FORUM_PROGRAMMING);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_REPUBLICAN_PUBLIC_MESSAGE_SITE);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_REPUBLICAN_PUBLIC_MESSAGE_SITE);
		// p.AddRole(SOCIETYROLE_MATERNAL_AUNT);
		// p.AddRole(SOCIETYROLE_PATERNAL_AUNT);
		// p.AddRole(SOCIETYROLE_MATERNAL_UNCLE);
		// p.AddRole(SOCIETYROLE_PATERNAL_UNCLE);
		// p.AddRole(SOCIETYROLE_MATERNAL_FEMALE_COUSIN);
		// p.AddRole(SOCIETYROLE_PATERNAL_FEMALE_COUSIN);
	}
	{
		Platform& p = a[PLATFORM_LINKEDIN];
		p.name = "LinkedIn";
		p.group = "Professional site";
		p.description = "A professional networking platform for connecting with colleagues and building a professional profile.";
		p.profile_type = PLATFORM_PROFILE_REAL_PERSON;
		ENABLE(has_profile_image);
		ENABLE(has_description);
		ENABLE(has_comments);
		ENABLE(has_message);
		ENABLE(has_hashtags);
		ENABLE(has_video);
		ENABLE(has_image);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_PROFSSIONAL_INDUSTRY_SITE); // LinkedIn
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_PROFSSIONAL_INDUSTRY_SITE);
		// p.AddRole(SOCIETYROLE_FEMALE_RECRUITER_FOR_WORK);
		// p.AddRole(SOCIETYROLE_MALE_RECRUITER_FOR_WORK);
		// p.AddRole(SOCIETYROLE_FEMALE_SUPERIOR_IN_MILITARY_SERVICE);
		// p.AddRole(SOCIETYROLE_MALE_SUPERIOR_IN_MILITARY_SERVICE);
		// p.AddRole(SOCIETYROLE_FEMALE_PEER_IN_MILITARY_SERVICE);
		// p.AddRole(SOCIETYROLE_MALE_PEER_IN_MILITARY_SERVICE);
		// p.AddRole(SOCIETYROLE_MATERNAL_GRANDMOTHER);
		// p.AddRole(SOCIETYROLE_PATERNAL_GRANDMOTHER);
		// p.AddRole(SOCIETYROLE_MATERNAL_GRANDFATHER);
		// p.AddRole(SOCIETYROLE_PATERNAL_GRANDFATHER);
	}
	{
		Platform& p = a[PLATFORM_SOUNDCLOUD];
		p.name = "Soundcloud";
		p.group = "Music artist site";
		p.description = "An audio-sharing platform for musicians and creators to share their work.";
		p.profile_type = PLATFORM_PROFILE_ARTIST;
		ENABLE(has_profile_image);
		ENABLE(has_description);
		ENABLE(has_comments);
		ENABLE(has_title);
		ENABLE(has_hashtags);
		ENABLE(has_audio);
		ENABLE(has_music);
		ENABLE(has_music_cover);
		ENABLE(has_comment_self_promotion_hack);
		ENABLE(has_Q_and_A_hack);
		ENABLE(has_testimonial_hack);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_SOCIAL_MUSIC_SITE); // soundcloud
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_SOCIAL_MUSIC_SITE); // soundcloud
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_FORUM_MUSIC);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_FORUM_MUSIC);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_MUSIC_COOPERATION_SITE);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_MUSIC_COOPERATION_SITE);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_PROGRAMMING_COOPERATION_SITE);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_PROGRAMMING_COOPERATION_SITE);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_WEBSITE_READER);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_WEBSITE_READER);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_CONSUMERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_COMPOSERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_PERFORMING_ARTISTS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_JOURNALISTS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_INDUSTRY);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_RECORD_ARCHIVERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_ARTIST_MANAGERS);
	}
	{
		Platform& p = a[PLATFORM_MUSIC_DISTRIBUTOR];
		p.name = "Music Distributor";
		p.group = "Professional music site";
		p.description = "A platform that helps musicians distribute their music to various streaming services and online stores.";
		ENABLE(has_title);
		ENABLE(has_music);
		ENABLE(has_image);
		ENABLE(has_music_cover);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_SOCIAL_MUSIC_SITE); // soundcloud
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_SOCIAL_MUSIC_SITE);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_MUSIC_COOPERATION_SITE);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_MUSIC_COOPERATION_SITE);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_FORUM_MUSIC);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_FORUM_MUSIC);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_WEBSITE_READER);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_WEBSITE_READER);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_FORUM_PROGRAMMING);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_FORUM_PROGRAMMING);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_JOURNALISTS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_EDUCATORS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_RECORD_ARCHIVERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_CONSUMERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_PRODUCERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_COMPOSERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_LYRICISTS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_PUBLISHERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_PERFORMING_ARTISTS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_RECORD_COMPANIES);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSICIANS_AND_MUSIC_WORKERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_INDUSTRY);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_RECORD_ARCHIVERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_ACCOUNTANTS_OF_MUSIC_PRODUCERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_ARTIST_MANAGERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_CONSUMERS);
	}
	{
		Platform& p = a[PLATFORM_YOUTUBE];
		p.name = "YouTube";
		p.group = "Video site";
		p.description = "A video-sharing platform for hosting and sharing user-generated content.";
		ENABLE(has_profile_image);
		ENABLE(has_description);
		ENABLE(has_comments);
		ENABLE(has_message);
		ENABLE(has_title);
		ENABLE(has_hashtags);
		ENABLE(has_video);
		ENABLE(has_reel);
		ENABLE(has_image);
		ENABLE(has_comment_self_promotion_hack);
		ENABLE(has_Q_and_A_hack);
		ENABLE(has_testimonial_hack);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_MODERATE_VIDEO_SITE); // YouTube
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_MODERATE_VIDEO_SITE); // YouTube
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_FORUM_MUSIC);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_FORUM_MUSIC);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_MUSIC_COOPERATION_SITE);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_MUSIC_COOPERATION_SITE);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_PROGRAMMING_COOPERATION_SITE);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_PROGRAMMING_COOPERATION_SITE);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_WEBSITE_READER);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_WEBSITE_READER);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_JOURNALISTS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_EDUCATORS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_RECORD_ARCHIVERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_CONSUMERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_PRODUCERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_COMPOSERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_LYRICISTS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_PUBLISHERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_PERFORMING_ARTISTS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_RECORD_COMPANIES);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSICIANS_AND_MUSIC_WORKERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_INDUSTRY);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_RECORD_ARCHIVERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_ACCOUNTANTS_OF_MUSIC_PRODUCERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_ARTIST_MANAGERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_CONSUMERS);
	}
	{
		Platform& p = a[PLATFORM_VK];
		p.name = "VK";
		p.group = "Full profile site";
		p.description = "A social networking platform popular in Russia and other Eastern European countries.";
		p.profile_type = PLATFORM_PROFILE_REAL_PERSON;
		ENABLE(has_profile_image);
		ENABLE(has_description);
		ENABLE(has_comments);
		ENABLE(has_message);
		ENABLE(has_hashtags);
		ENABLE(has_video);
		ENABLE(has_reel);
		ENABLE(has_image);
		ENABLE(has_link_promotion);
		ENABLE(has_comment_self_promotion_hack);
		ENABLE(has_Q_and_A_hack);
		ENABLE(has_testimonial_hack);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_SOCIAL_PROGRAMMING_SITE); // github
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_SOCIAL_PROGRAMMING_SITE); // github
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_FORUM_PROGRAMMING);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_FORUM_PROGRAMMING);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_MODERATE_FULL_PROFILE_SITE); // Facebook
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_MODERATE_FULL_PROFILE_SITE); // Facebook
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_SOCIAL_MUSIC_SITE); // soundcloud
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_SOCIAL_MUSIC_SITE); // soundcloud
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_PROGRAMMING_COOPERATION_SITE);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_PROGRAMMING_COOPERATION_SITE);
	}
	{
		Platform& p = a[PLATFORM_REDDIT];
		p.name = "Reddit";
		p.group = "Forum";
		p.description = "A platform for discussion and sharing links and content on a variety of topics.";
		p.profile_type = PLATFORM_PROFILE_REAL_PERSON;
		ENABLE(has_profile_image);
		ENABLE(has_description);
		ENABLE(has_comments);
		ENABLE(has_message);
		ENABLE(has_title);
		ENABLE(has_link_promotion);
		ENABLE(has_comment_self_promotion_hack);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_FORUM_PROGRAMMING);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_FORUM_PROGRAMMING);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_SOCIAL_PROGRAMMING_SITE); // github
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_SOCIAL_PROGRAMMING_SITE); // github
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_FORUM_MUSIC);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_FORUM_MUSIC);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_FORUM_PROGRAMMING);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_FORUM_PROGRAMMING);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_WEBSITE_READER);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_WEBSITE_READER);
	}
	{
		Platform& p = a[PLATFORM_FORUM];
		p.name = "Forum";
		p.group = "Forum";
		p.description = "An online discussion platform where users can share and exchange ideas and information on a specific topic.";
		ENABLE(has_profile_image);
		ENABLE(has_description);
		ENABLE(has_comments);
		ENABLE(has_message);
		ENABLE(has_title);
		ENABLE(has_link_promotion);
		ENABLE(has_comment_self_promotion_hack);
		ENABLE(has_Q_and_A_hack);
		ENABLE(has_testimonial_hack);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_FORUM_MUSIC);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_FORUM_MUSIC);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_FORUM_PROGRAMMING);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_FORUM_PROGRAMMING);
		// p.AddRole(SOCIETYROLE_FEMALE_PEER_IN_MILITARY_SERVICE);
		// p.AddRole(SOCIETYROLE_MALE_PEER_IN_MILITARY_SERVICE);
	}
	{
		Platform& p = a[PLATFORM_BLOGGER];
		p.name = "Blogger";
		p.group = "Personal";
		p.description = "A platform for creating and hosting personal or professional blogs.";
		ENABLE(has_profile_image);
		ENABLE(has_description);
		ENABLE(has_comments);
		ENABLE(has_message);
		ENABLE(has_title);
		ENABLE(has_hashtags);
		ENABLE(has_music_cover);
		ENABLE(has_audio);
		ENABLE(has_music);
		ENABLE(has_video);
		ENABLE(has_reel);
		ENABLE(has_image);
		ENABLE(has_link_promotion);
		ENABLE(has_comment_self_promotion_hack);
		ENABLE(has_Q_and_A_hack);
		ENABLE(has_testimonial_hack);
		// p.AddRole(SOCIETYROLE_WIFE);
		// p.AddRole(SOCIETYROLE_MOTHER);
		// p.AddRole(SOCIETYROLE_LITTLE_SISTER);
		// p.AddRole(SOCIETYROLE_BIG_SISTER);
		// p.AddRole(SOCIETYROLE_MATERNAL_GRANDMOTHER);
		// p.AddRole(SOCIETYROLE_PATERNAL_GRANDMOTHER);
		// p.AddRole(SOCIETYROLE_MATERNAL_AUNT);
		// p.AddRole(SOCIETYROLE_PATERNAL_AUNT);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_MODERATE_FULL_PROFILE_SITE); // Facebook
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_MODERATE_VIDEO_SITE); // YouTube
	}
	{
		Platform& p = a[PLATFORM_WEBSITE];
		p.name = "Website";
		p.group = "Personal";
		p.description = "An online platform used to share information, promote products or services, or express personal opinions or viewpoints.";
		ENABLE(has_profile_image);
		ENABLE(has_description);
		ENABLE(has_message);
		ENABLE(has_title);
		ENABLE(has_hashtags);
		ENABLE(has_audio);
		ENABLE(has_music);
		ENABLE(has_music_cover);
		ENABLE(has_video);
		ENABLE(has_image);
		ENABLE(has_link_promotion);
		ENABLE(has_comment_self_promotion_hack);
		ENABLE(has_Q_and_A_hack);
		ENABLE(has_testimonial_hack);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_PROFSSIONAL_INDUSTRY_SITE); // LinkedIn
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_PROFSSIONAL_INDUSTRY_SITE); // LinkedIn
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_MODERATE_FULL_PROFILE_SITE); // Facebook
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_MODERATE_FULL_PROFILE_SITE); // Facebook
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_FORUM_PROGRAMMING);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_FORUM_PROGRAMMING);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_SOCIAL_PROGRAMMING_SITE); // github
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_SOCIAL_PROGRAMMING_SITE); // github
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_MODERATE_VIDEO_SITE); // YouTube
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_MODERATE_VIDEO_SITE); // YouTube
	}
	{
		Platform& p = a[PLATFORM_TWITCH];
		p.name = "Twitch";
		p.group = "Live site";
		p.description = "A live streaming platform popular among gamers but also used for other forms of live streaming entertainment.";
		p.profile_type = PLATFORM_PROFILE_REAL_PERSON;
		ENABLE(has_profile_image);
		ENABLE(has_description);
		ENABLE(has_comments);
		ENABLE(has_video);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_SHALLOW_VIDEO_STREAMING_SITE); // Twitch
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_SHALLOW_VIDEO_STREAMING_SITE); // Twitch
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_MODERATE_VIDEO_SITE); // YouTube
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_MODERATE_VIDEO_SITE); // YouTube
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_SHALLOW_REEL_VIDEO_SITE); // TikTok,_Instagram
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_SHALLOW_REEL_VIDEO_SITE); // TikTok,_Instagram
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_FORUM_MUSIC);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_FORUM_MUSIC);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_INFLUENCER_FOR_ART_AND_MUSIC);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_INFLUENCER_FOR_ART_AND_MUSIC);
	}
	{
		Platform& p = a[PLATFORM_STUMBLE];
		p.name = "Stumble";
		p.group = "Link sharing site";
		p.description = "A platform for discovering and sharing web content with a focus on personalized recommendations.";
		ENABLE(has_link_promotion);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_WEBSITE_READER);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_WEBSITE_READER);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_MODERATE_FULL_PROFILE_SITE); // Facebook
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_MODERATE_FULL_PROFILE_SITE); // Facebook
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_SOCIAL_MUSIC_SITE); // soundcloud
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_SOCIAL_MUSIC_SITE); // soundcloud
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_FORUM_MUSIC);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_FORUM_MUSIC);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_MODERATE_VIDEO_SITE); // YouTube
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_MODERATE_VIDEO_SITE); // YouTube
	}
	{
		Platform& p = a[PLATFORM_GITHUB];
		p.name = "GitHub";
		p.group = "Professional programming site";
		p.description = "A platform for hosting and collaborating on software development projects.";
		ENABLE(has_profile_image);
		ENABLE(has_description);
		ENABLE(has_Q_and_A_hack);
		ENABLE(has_testimonial_hack);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_MODERATE_FULL_PROFILE_SITE); // Facebook
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_MODERATE_FULL_PROFILE_SITE); // Facebook
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_SOCIAL_PROGRAMMING_SITE); // github
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_SOCIAL_PROGRAMMING_SITE); // github
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_FORUM_PROGRAMMING);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_FORUM_PROGRAMMING);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_SOCIAL_MUSIC_SITE); // Soundcloud
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_SOCIAL_MUSIC_SITE); // Soundcloud
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_MUSIC_COOPERATION_SITE);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_MUSIC_COOPERATION_SITE);
	}
	{
		Platform& p = a[PLATFORM_MYSPACE];
		p.name = "MySpace";
		p.group = "Music artist site";
		p.description = "A social networking platform that was once popular for music sharing and networking but has now declined in popularity.";
		p.profile_type = PLATFORM_PROFILE_ARTIST;
		ENABLE(has_profile_image);
		ENABLE(has_description);
		ENABLE(has_comments);
		ENABLE(has_title);
		ENABLE(has_message);
		ENABLE(has_hashtags);
		ENABLE(has_music);
		ENABLE(has_image);
		ENABLE(has_link_promotion);
		ENABLE(has_comment_self_promotion_hack);
		ENABLE(has_Q_and_A_hack);
		ENABLE(has_testimonial_hack);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_SHALLOW_IMAGE_SITE); // Instagram
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_SHALLOW_IMAGE_SITE); // Instagram
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_SHALLOW_REEL_VIDEO_SITE); // TikTok,_Instagram
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_SHALLOW_REEL_VIDEO_SITE); // TikTok,_Instagram
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_SHALLOW_PUBLIC_MESSAGE_SITE); // Twitter
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_SHALLOW_PUBLIC_MESSAGE_SITE); // Twitter
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_SHALLOW_VIDEO_STREAMING_SITE); // Twitch
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_SHALLOW_VIDEO_STREAMING_SITE); // Twitch
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_MODERATE_FULL_PROFILE_SITE); // Facebook
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_MODERATE_FULL_PROFILE_SITE); // Facebook
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_PRODUCERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_COMPOSERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_LYRICISTS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_PUBLISHERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_PERFORMING_ARTISTS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_JOURNALISTS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_EDUCATORS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_RECORD_ARCHIVERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_CONSUMERS);
	}
	{
		Platform& p = a[PLATFORM_MIKSERINET];
		p.name = "Mikseri.net";
		p.group = "Music artist site";
		p.description = "A Finnish social networking platform for musicians and music fans.";
		p.profile_type = PLATFORM_PROFILE_ARTIST;
		ENABLE(has_profile_image);
		ENABLE(has_description);
		ENABLE(has_comments);
		ENABLE(has_title);
		ENABLE(has_message);
		ENABLE(has_hashtags);
		ENABLE(has_music);
		ENABLE(has_image);
		ENABLE(has_link_promotion);
		ENABLE(has_comment_self_promotion_hack);
		ENABLE(has_Q_and_A_hack);
		ENABLE(has_testimonial_hack);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_SOCIAL_MUSIC_SITE); // soundcloud
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_SOCIAL_MUSIC_SITE); // soundcloud
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_FORUM_MUSIC);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_FORUM_MUSIC);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_MUSIC_COOPERATION_SITE);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_MUSIC_COOPERATION_SITE);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_PROGRAMMING_COOPERATION_SITE);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_PROGRAMMING_COOPERATION_SITE);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_FORUM_PROGRAMMING);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_FORUM_PROGRAMMING);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_PRODUCERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_COMPOSERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_LYRICISTS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_PUBLISHERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_PERFORMING_ARTISTS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_JOURNALISTS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_EDUCATORS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_RECORD_ARCHIVERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_CONSUMERS);
	}
	{
		Platform& p = a[PLATFORM_IRCGALLERIA];
		p.name = "Irc-Galleria.net";
		p.group = "Image site";
		p.description = "A Finnish image and chat-based social networking platform.";
		p.profile_type = PLATFORM_PROFILE_REAL_PERSON;
		ENABLE(has_description);
		ENABLE(has_profile_image);
		ENABLE(has_comments);
		ENABLE(has_hashtags);
		ENABLE(has_image);
		ENABLE(has_link_promotion);
		ENABLE(has_comment_self_promotion_hack);
		ENABLE(has_music_cover);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_MODERATE_FULL_PROFILE_SITE); // Facebook
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_MODERATE_FULL_PROFILE_SITE); // Facebook
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_MUSIC_COOPERATION_SITE);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_MUSIC_COOPERATION_SITE);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_PROGRAMMING_COOPERATION_SITE);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_PROGRAMMING_COOPERATION_SITE);
		// p.AddRole(SOCIETYROLE_FEMALE_SUPERIOR_IN_MILITARY_SERVICE);
		// p.AddRole(SOCIETYROLE_MALE_SUPERIOR_IN_MILITARY_SERVICE);
		// p.AddRole(SOCIETYROLE_FEMALE_INFERIOR_IN_MILITARY_SERVICE);
		// p.AddRole(SOCIETYROLE_MALE_INFERIOR_IN_MILITARY_SERVICE);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_COMPUTER_PROGRAMMERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_ACCOUNTANTS_OF_COMPUTER_PROGRAMMERS);
	}
	{
		Platform& p = a[PLATFORM_DISCORD];
		p.name = "Discord";
		p.group = "Live site";
		p.description = "A platform for voice, video, and text communication primarily geared towards gamers but also used for other forms of online communities.";
		p.profile_type = PLATFORM_PROFILE_REAL_PERSON;
		ENABLE(has_profile_image);
		ENABLE(has_description);
		ENABLE(has_comments);
		ENABLE(has_hashtags);
		ENABLE(has_audio);
		ENABLE(has_image);
		ENABLE(has_link_promotion);
		ENABLE(has_comment_self_promotion_hack);
		ENABLE(has_Q_and_A_hack);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_FORUM_PROGRAMMING);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_FORUM_PROGRAMMING);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_SOCIAL_PROGRAMMING_SITE); // github
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_SOCIAL_PROGRAMMING_SITE); // github
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_SOCIAL_MUSIC_SITE); // soundcloud
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_SOCIAL_MUSIC_SITE); // soundcloud
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_SHALLOW_PUBLIC_MESSAGE_SITE); // Twitter
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_SHALLOW_PUBLIC_MESSAGE_SITE); // Twitter
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_MODERATE_VIDEO_SITE); // YouTube
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_MODERATE_VIDEO_SITE); // YouTube
	}
	{
		Platform& p = a[PLATFORM_MUKKEN];
		p.name = "Mukken";
		p.group = "Professional music site";
		p.description = "An European alternative for BandMix, which brings musicians, bands, music teachers, and producers together";
		p.profile_type = PLATFORM_PROFILE_REAL_PERSON;
		ENABLE(has_profile_image);
		ENABLE(has_description);
		ENABLE(has_comments);
		ENABLE(has_title);
		ENABLE(has_message);
		ENABLE(has_hashtags);
		ENABLE(has_music);
		ENABLE(has_image);
		ENABLE(has_link_promotion);
		ENABLE(has_comment_self_promotion_hack);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_MUSIC_COOPERATION_SITE);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_MUSIC_COOPERATION_SITE);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_PROGRAMMING_COOPERATION_SITE);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_PROGRAMMING_COOPERATION_SITE);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_SOCIAL_MUSIC_SITE); // soundcloud
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_SOCIAL_MUSIC_SITE); // soundcloud
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_FORUM_MUSIC);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_FORUM_MUSIC);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_WEBSITE_READER);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_WEBSITE_READER);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_PRODUCERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_COMPOSERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_LYRICISTS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_PUBLISHERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_PERFORMING_ARTISTS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSICIANS_AND_MUSIC_WORKERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_ARTIST_MANAGERS);
	}
	{
		Platform& p = a[PLATFORM_BANDCAMP];
		p.name = "Bandcamp";
		p.group = "Professional music site";
		p.description = "A platform for musicians to sell and stream their music online.";
		ENABLE(has_profile_image);
		ENABLE(has_description);
		ENABLE(has_comments);
		ENABLE(has_title);
		ENABLE(has_message);
		ENABLE(has_hashtags);
		ENABLE(has_music);
		ENABLE(has_image);
		ENABLE(has_link_promotion);
		ENABLE(has_comment_self_promotion_hack);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_SOCIAL_MUSIC_SITE); // soundcloud
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_SOCIAL_MUSIC_SITE);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_MUSIC_COOPERATION_SITE);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_MUSIC_COOPERATION_SITE);
		// p.AddRole(SOCIETYROLE_WIFE);
		// p.AddRole(SOCIETYROLE_BEST_MAN);
		// p.AddRole(SOCIETYROLE_MATERNAL_AUNT);
		// p.AddRole(SOCIETYROLE_PATERNAL_AUNT);
		// p.AddRole(SOCIETYROLE_MATERNAL_UNCLE);
		// p.AddRole(SOCIETYROLE_PATERNAL_UNCLE);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_JOURNALISTS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_CONSUMERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_PRODUCERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_COMPOSERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_LYRICISTS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_PUBLISHERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_PERFORMING_ARTISTS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_RECORD_COMPANIES);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSICIANS_AND_MUSIC_WORKERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_INDUSTRY);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_RECORD_ARCHIVERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_ACCOUNTANTS_OF_MUSIC_PRODUCERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_ARTIST_MANAGERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_CONSUMERS);
	}
	{
		Platform& p = a[PLATFORM_REMOTEMORE];
		p.name = "RemoteMore";
		p.group = "Professional programming site";
		p.description = "A hiring marketplace for employers and employees that specializes in remote work.";
		ENABLE(has_profile_image);
		ENABLE(has_description);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_MODERATE_FULL_PROFILE_SITE); // Facebook
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_MODERATE_FULL_PROFILE_SITE);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_MODERATE_VIDEO_SITE); // YouTube
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_MODERATE_VIDEO_SITE);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_PROFSSIONAL_INDUSTRY_SITE); // LinkedIn
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_PROFSSIONAL_INDUSTRY_SITE);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_SOCIAL_PROGRAMMING_SITE); // github
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_SOCIAL_PROGRAMMING_SITE);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_PROGRAMMING_COOPERATION_SITE);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_PROGRAMMING_COOPERATION_SITE);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_COMPUTER_PROGRAMMERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_ACCOUNTANTS_OF_COMPUTER_PROGRAMMERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_ACCOUNTANTS_OF_COMPUTER_PROGRAMMERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_SOFTWARE_COMPANIES);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_SOFTWARE_INDUSTRY);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_SOFTWARE_INDUSTRY);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_SOFTWARE_PROJECT_MANAGERS);
	}
	{
		Platform& p = a[PLATFORM_KUVAKENET];
		p.name = "Kuvake.net";
		p.group = "Image site";
		p.description = "A Finnish photo-sharing and social networking platform";
		ENABLE(has_profile_image);
		ENABLE(has_description);
		ENABLE(has_comments);
		ENABLE(has_hashtags);
		ENABLE(has_image);
		ENABLE(has_link_promotion);
		ENABLE(has_comment_self_promotion_hack);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_MODERATE_FULL_PROFILE_SITE); // Facebook
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_MODERATE_FULL_PROFILE_SITE); // Facebook
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_MUSIC_COOPERATION_SITE);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_MUSIC_COOPERATION_SITE);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_PROGRAMMING_COOPERATION_SITE);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_PROGRAMMING_COOPERATION_SITE);
		// p.AddRole(SOCIETYROLE_FEMALE_SUPERIOR_IN_MILITARY_SERVICE);
		// p.AddRole(SOCIETYROLE_MALE_SUPERIOR_IN_MILITARY_SERVICE);
		// p.AddRole(SOCIETYROLE_FEMALE_INFERIOR_IN_MILITARY_SERVICE);
		// p.AddRole(SOCIETYROLE_MALE_INFERIOR_IN_MILITARY_SERVICE);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_COMPUTER_PROGRAMMERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_ACCOUNTANTS_OF_COMPUTER_PROGRAMMERS);
	}
	{
		Platform& p = a[PLATFORM_REVERBNATION];
		p.name = "ReverbNation";
		p.group = "Professional music site";
		p.description = "A platform for musicians to promote and distribute their music.";
		ENABLE(has_profile_image);
		ENABLE(has_description);
		ENABLE(has_comments);
		ENABLE(has_title);
		ENABLE(has_hashtags);
		ENABLE(has_audio);
		ENABLE(has_music);
		ENABLE(has_image);
		ENABLE(has_comment_self_promotion_hack);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_PRODUCERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_COMPOSERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_LYRICISTS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_PUBLISHERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_PERFORMING_ARTISTS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_RECORD_COMPANIES);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_JOURNALISTS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSICIANS_AND_MUSIC_WORKERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_INDUSTRY);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_INTEREST_OF_MUSIC_RECORD_ARCHIVERS);
	}
	{
		Platform& p = a[PLATFORM_SONICBIDS];
		p.name = "SonicBids";
		p.group = "Professional music site";
		p.description = "A platform for musicians to find and apply for gigs and other opportunities.";
		ENABLE(has_profile_image);
		ENABLE(has_description);
		ENABLE(has_comments);
		ENABLE(has_title);
		ENABLE(has_hashtags);
		ENABLE(has_audio);
		ENABLE(has_music);
		ENABLE(has_image);
		ENABLE(has_comment_self_promotion_hack);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_PRODUCERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_COMPOSERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_LYRICISTS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_PUBLISHERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_JOURNALISTS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSICIANS_AND_MUSIC_WORKERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_INDUSTRY);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_EDUCATORS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_RECORD_ARCHIVERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_ACCOUNTANTS_OF_MUSIC_PRODUCERS);
	}
	{
		Platform& p = a[PLATFORM_MUSICGATEWAY];
		p.name = "MusicGateway";
		p.group = "Professional music site";
		p.description = "A platform for musicians and music professionals to network and collaborate.";
		ENABLE(has_profile_image);
		ENABLE(has_description);
		ENABLE(has_comments);
		ENABLE(has_title);
		ENABLE(has_hashtags);
		ENABLE(has_audio);
		ENABLE(has_music);
		ENABLE(has_image);
		ENABLE(has_comment_self_promotion_hack);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_PRODUCERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_COMPOSERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_LYRICISTS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_PUBLISHERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_PERFORMING_ARTISTS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_RECORD_COMPANIES);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_JOURNALISTS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSICIANS_AND_MUSIC_WORKERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_INDUSTRY);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_EDUCATORS);
	}
	{
		Platform& p = a[PLATFORM_INDIEONTHEMOVE];
		p.name = "Indie On The Move";
		p.group = "Professional music site";
		p.description = "A platform for musicians to find and book tour dates and accommodations.";
		ENABLE(has_profile_image);
		ENABLE(has_description);
		ENABLE(has_comments);
		ENABLE(has_title);
		ENABLE(has_hashtags);
		ENABLE(has_audio);
		ENABLE(has_music);
		ENABLE(has_image);
		ENABLE(has_comment_self_promotion_hack);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_FORUM_MUSIC);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_FORUM_MUSIC);
		// p.AddRole(SOCIETYROLE_FEMALE_IN_INTERNET_SOCIAL_MUSIC_SITE);
		// p.AddRole(SOCIETYROLE_MALE_IN_INTERNET_SOCIAL_MUSIC_SITE);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_PRODUCERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_COMPOSERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_LYRICISTS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_PUBLISHERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSICIANS_AND_MUSIC_WORKERS);
		// p.AddRole(SOCIETYROLE_REPRESENTATIVE_OF_THE_ORGANIZATION_FOR_RIGHTS_OF_MUSIC_INDUSTRY);
	}
	{
		Platform& p = a[PLATFORM_VOWAVE];
		p.name = "VoWave";
		p.group = "Professional music site";
		p.description = "A streaming and social networking platform focused on electronic and dance music.";
		ENABLE(has_profile_image);
		ENABLE(has_description);
		ENABLE(has_title);
		ENABLE(has_music);
		ENABLE(has_image);
	}
	{
		Platform& p = a[PLATFORM_AUDIUS];
		p.name = "Audius";
		p.group = "Professional music site";
		p.description = "A blockchain-based music streaming platform.";
		ENABLE(has_profile_image);
		ENABLE(has_description);
		ENABLE(has_title);
		ENABLE(has_music);
		ENABLE(has_image);
	}
	{
		Platform& p = a[PLATFORM_SONGTRADR];
		p.name = "SongTrader";
		p.group = "Professional music site";
		p.description = "A global music licensing platform for artists and filmmakers to connect and collaborate.";
		ENABLE(has_profile_image);
		ENABLE(has_description);
		ENABLE(has_title);
		ENABLE(has_music);
		ENABLE(has_image);
	}
	{
		Platform& p = a[PLATFORM_GROOVER];
		p.name = "Groover";
		p.group = "Professional music site";
		p.description = "A platform for artists to promote their music and connect with music professionals.";
		ENABLE(has_profile_image);
		ENABLE(has_description);
		ENABLE(has_title);
		ENABLE(has_profile_image);
		ENABLE(has_description);
		ENABLE(has_music);
	}
	{
		Platform& p = a[PLATFORM_AIRPLAYDIRECT];
		p.name = "AirplayDirect";
		p.group = "Professional music site";
		p.description = "A digital music delivery system for radio stations and DJs.";
		ENABLE(has_title);
		ENABLE(has_link_promotion);
		ENABLE(has_music);
		ENABLE(has_music_cover);
	}
	{
		Platform& p = a[PLATFORM_N1M];
		p.name = "N1M";
		p.group = "Professional music site";
		p.description = "A platform for independent musicians to promote and distribute their music.";
		ENABLE(has_description);
		ENABLE(has_profile_image);
		ENABLE(has_music_cover);
		ENABLE(has_music);
		ENABLE(has_title);
		ENABLE(has_message);
		ENABLE(has_link_promotion);
	}
	{
		Platform& p = a[PLATFORM_SOUNDBETTER];
		p.name = "SoundBetter";
		p.group = "Professional music site";
		p.description = "A platform for musicians and audio professionals to offer their services and collaborate.";
		ENABLE(has_description);
		ENABLE(has_profile_image);
		ENABLE(has_comments);
		ENABLE(has_Q_and_A);
		ENABLE(has_music);
		ENABLE(has_image);
		ENABLE(has_link_promotion);
	}
	{
		Platform& p = a[PLATFORM_ABOUTME];
		p.name = "About.me";
		p.group = "Personal";
		p.description = "A platform for creating a personal landing page and showcasing professional skills and projects.";
		ENABLE(has_description);
		ENABLE(has_profile_image);
		ENABLE(has_Q_and_A_hack);
		ENABLE(has_link_promotion);
	}
	{
		Platform& p = a[PLATFORM_FIVERR];
		p.name = "Fiverr";
		p.group = "Professional site";
		p.description = "A platform for freelancers to offer their services and connect with clients.";
		p.profile_type = PLATFORM_PROFILE_REAL_PERSON;
		ENABLE(has_description);
		ENABLE(has_profile_image);
		ENABLE(has_Q_and_A_hack);
		ENABLE(has_image);
		ENABLE(has_link_promotion);
		ENABLE(has_comments);
	}
	{
		Platform& p = a[PLATFORM_THEDOTS];
		p.name = "The Dots";
		p.group = "Professional site";
		p.description = "A platform for creative professionals to showcase their work and connect with other creatives.";
		p.profile_type = PLATFORM_PROFILE_REAL_PERSON;
		ENABLE(has_description);
		ENABLE(has_profile_image);
		ENABLE(has_Q_and_A);
		ENABLE(has_link_promotion);
	}
	{
		Platform& p = a[PLATFORM_CONSTANTCONTACT];
		p.name = "Constant Contact";
		p.group = "Direct";
		p.description = "A platform for email marketing and online surveys.";
		ENABLE(has_title);
		ENABLE(has_message);
		ENABLE(has_testimonial_hack);
		ENABLE(has_link_promotion);
		ENABLE(has_image);
	}
	{
		Platform& p = a[PLATFORM_MUUSIKOIDEN_NET];
		p.name = "Muusikoiden.net";
		p.group = "Forum";
		p.description = "A Finnish platform for musicians and music fans to connect and share content and information.";
		ENABLE(has_profile_image);
		ENABLE(has_description);
		ENABLE(has_comments);
		ENABLE(has_message);
		ENABLE(has_title);
		ENABLE(has_link_promotion);
		ENABLE(has_comment_self_promotion_hack);
		ENABLE(has_Q_and_A_hack);
		ENABLE(has_testimonial_hack);
	}
	{
		Platform& p = a[PLATFORM_PODCAST];
		p.name = "Podcast";
		p.group = "Radio";
		p.description = "A platform for hosting and distributing talking shows for listeners to download or stream.";
		ENABLE(has_profile_image);
		ENABLE(has_description);
		ENABLE(has_message);
		ENABLE(has_title);
		ENABLE(has_link_promotion);
		ENABLE(has_comment_self_promotion_hack);
		ENABLE(has_Q_and_A_hack);
		ENABLE(has_testimonial_hack);
	}
	
	
	
	return a;
}

void ProfileData::Jsonize(JsonIO& json) {
	json
		("platforms", platforms)
		;
}

void ProfileData::Load() {
	String dir = AppendFileName(MetaDatabase::GetUserDirectory(), "share");
	String fname = IntStr64(hash) + ".json";
	String path = AppendFileName(dir, fname);
	
	LoadFromJsonFileStandard(*this, path);
}

void ProfileData::Store() {
	String dir = AppendFileName(MetaDatabase::GetUserDirectory(), "share");
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
	ch.Do(p.owner->name);
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

