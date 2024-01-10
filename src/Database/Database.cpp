#include "Database.h"

int Database::trans_i = -1;


Database::Database() {
	
}

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
	StoreAsJsonFileStandard(*this, dir + DIR_SEPS "share" DIR_SEPS "db.json", true);
	//song_data.StoreJson();
}

void Database::Load() {
	Clear();
	
	lock.EnterWrite();
	LoadFromJsonFileStandard(*this, dir + DIR_SEPS "share" DIR_SEPS "db.json");
	//song_data.LoadJson();
	lock.LeaveWrite();
/*	bool initial = attr_groups.IsEmpty();
	attrs.Realize();
	if (!initial)
		RealizeAttrIds();*/
}

/*void Database::RealizeAttrIds() {
	for (Artist& a : artists) {
		a.ResolveId();
	}
	attrs.RealizeAttrIds();
}*/

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

String Database::Translate(const String& s) {
	if (trans_i < 0)
		return s;
	Translation& t = this->translation[trans_i];
	int i = t.data.Find(s);
	String o;
	if (i >= 0)
		o = t.data[i];
	if (i < 0 || o.IsEmpty()) {
		i = t.data.Find(ToLower(s));
		if (i >= 0)
			o = t.data[i];
		else
			t.data.Add(ToLower(s));
	}
	return o.IsEmpty() ? s : o;
}

