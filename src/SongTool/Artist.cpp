#include "SongTool.h"

void Artist::Store() {
	String dir = Database::Single().GetArtistsDir();
	RealizeDirectory(dir);
	String json_path = dir + file_title + ".json";
	StoreAsJsonFile(*this, json_path, true);
}

void Artist::LoadTitle(String title) {
	String dir = Database::Single().GetArtistsDir();
	file_title = title;
	String json_path = dir + file_title + ".json";
	LoadFromJsonFile(*this, json_path);
}
