#include "SongTool.h"

String Database::GetArtistsDir() const {
	return dir + DIR_SEPS "share" DIR_SEPS "artists" DIR_SEPS;
}

String Database::GetReleasesDir() const {
	return dir + DIR_SEPS "share" DIR_SEPS "releases" DIR_SEPS;
}

String Database::GetSongsDir(bool reversed) const {
	if (!reversed)
		return dir + DIR_SEPS "share" DIR_SEPS "songs" DIR_SEPS;
	else
		return dir + DIR_SEPS "share" DIR_SEPS "re_songs" DIR_SEPS;
}

void Database::Store() {
	StoreAsJsonFile(*this, dir + DIR_SEPS "share" DIR_SEPS "db.json", true);
}

void Database::Load() {
	Clear();
	LoadFromJsonFile(*this, dir + DIR_SEPS "share" DIR_SEPS "db.json");
	bool initial = attrs.groups.IsEmpty();
	attrs.Realize();
	if (!initial)
		RealizeAttrIds();
}

void Database::RealizeAttrIds() {
	for (Artist& a : artists) {
		a.ResolveId();
	}
	attrs.RealizeAttrIds();
}

void Database::FindOrphaned() {
	{
		String dir = GetArtistsDir();
		String search = dir + "*.json";
		
		FindFile ff;
		if (ff.Search(search)) do {
			if (ff.IsFile()) {
				String path = ff.GetPath();
				String title = GetFileTitle(path);
				bool found = false;
				for (Artist& a : artists)
					if (a.file_title == title)
						found = true;
				if (!found) {
					Artist& a = artists.Add();
					a.LoadTitle(title);
				}
			}
		}
		while (ff.Next());
	}
}

