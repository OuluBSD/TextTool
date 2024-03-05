#include "SocialDatabase.h"


BEGIN_SOCIALLIB_NAMESPACE


void Campaign::Store() {
	String dir = SocialDatabase::Single().GetCampaignsDir();
	RealizeDirectory(dir);
	String json_path = dir + file_title + ".json";
	StoreAsJsonFileStandard(*this, json_path, true);
}

void Campaign::LoadTitle(String title) {
	String dir = SocialDatabase::Single().GetCampaignsDir();
	file_title = title;
	String json_path = dir + file_title + ".json";
	LoadFromJsonFileStandard(*this, json_path);
}


END_SOCIALLIB_NAMESPACE

