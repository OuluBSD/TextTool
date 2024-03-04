#include "SocialDatabase.h"


void Release::Store() {
	String dir = SocialDatabase::Single().GetReleasesDir();
	RealizeDirectory(dir);
	String json_path = dir + file_title + ".json";
	StoreAsJsonFileStandard(*this, json_path, true);
}

void Release::LoadTitle(String title) {
	String dir = SocialDatabase::Single().GetReleasesDir();
	file_title = title;
	String json_path = dir + file_title + ".json";
	LoadFromJsonFileStandard(*this, json_path);
}

