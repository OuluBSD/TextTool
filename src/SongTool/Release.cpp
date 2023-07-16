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

Song& Release::RealizeReversed(Song& s) {
	for (Song& r : reversed_songs)
		if (r.linked_uniq == s.uniq)
			return r;
	Song& r = reversed_songs.Add();
	r.linked_uniq = s.uniq;
	r.artist = s.artist;
	r.title = s.title;
	r.owner = this;
	r.SetReversed();
	r.SetId(reversed_songs.GetCount()-1);
	r.FixPtrs();
	return r;
}
