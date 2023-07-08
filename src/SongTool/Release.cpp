#include "SongTool.h"

void Release::Store() {
	String dir = Database::Single().GetReleasesDir();
	RealizeDirectory(dir);
	String json_path = dir + file_title + ".json";
	StoreAsJsonFile(*this, json_path, true);
}

void Release::LoadTitle(String title) {
	String dir = Database::Single().GetReleasesDir();
	file_title = title;
	String json_path = dir + file_title + ".json";
	LoadFromJsonFile(*this, json_path);
}
