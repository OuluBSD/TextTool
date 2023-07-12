#include "SongTool.h"

String Database::GetArtistsDir() const {
	return dir + DIR_SEPS "share" DIR_SEPS "artists" DIR_SEPS;
}

String Database::GetReleasesDir() const {
	return dir + DIR_SEPS "share" DIR_SEPS "releases" DIR_SEPS;
}

String Database::GetSongsDir() const {
	return dir + DIR_SEPS "share" DIR_SEPS "songs" DIR_SEPS;
}

void Database::Store() {
	StoreAsJsonFile(*this, dir + DIR_SEPS "share" DIR_SEPS "db.json", true);
}

void Database::Load() {
	Clear();
	LoadFromJsonFile(*this, dir + DIR_SEPS "share" DIR_SEPS "db.json");
	
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

