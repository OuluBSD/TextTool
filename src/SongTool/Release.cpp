#include "SongTool.h"

void Release::Store() {
	String dir = Database::Single().GetReleasesDir();
	RealizeDirectory(dir);
	String json_path = dir + file_title + ".json";
	StoreAsJsonFileStandard(*this, json_path, true);
}

void Release::LoadTitle(String title) {
	String dir = Database::Single().GetReleasesDir();
	file_title = title;
	String json_path = dir + file_title + ".json";
	LoadFromJsonFileStandard(*this, json_path);
}

/*Song& Release::RealizeReversed(Song& s) {
	for (Song& r : reversed_songs)
		if (r.linked_uniq == s.uniq)
			return r;
	Song& r = reversed_songs.Add();
	r.file_title = s.file_title;
	r.linked_uniq = s.uniq;
	r.artist = s.artist;
	r.title = s.title;
	r.prj_name = s.prj_name;
	r.structure_str = s.structure_str;
	r.data <<= s.data;
	r.content = s.content;
	r.owner = this;
	r.SetReversed();
	r.SetId(reversed_songs.GetCount()-1);
	r.FixPtrs();
	return r;
}*/
