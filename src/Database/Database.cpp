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

String Database::GetLyricsDir() const {
	return dir + DIR_SEPS "share" DIR_SEPS "lyrics" DIR_SEPS;
}

void Database::Store() {
	StoreAsJsonFileStandard(*this, dir + DIR_SEPS "share" DIR_SEPS "db.json", true);
	
	for (auto& tc : typecasts) {
		for (auto& at : tc.archetypes) {
			for (auto& l : at.lyrics) {
				l.Store();
			}
		}
	}
}

void Database::Load() {
	Clear();
	
	lock.EnterWrite();
	LoadFromJsonFileStandard(*this, dir + DIR_SEPS "share" DIR_SEPS "db.json");
	lock.LeaveWrite();
	
	
	{
		RealizeTypecasts();
		FindFile ff(AppendFileName(GetLyricsDir(), "*.json"));
		do {
			String path = ff.GetPath();
			String title = UPP::GetFileTitle(path);
			Vector<String> parts = Split(title, "_");
			if (parts.GetCount() >= 3) {
				int tc = ScanInt(parts[0]);
				int ac = ScanInt(parts[1]);
				if (tc >= 0 && tc < typecasts.GetCount()) {
					Typecast& t = typecasts[tc];
					if (ac >= 0 && ac < t.archetypes.GetCount()) {
						Lyrics& lyr = t.archetypes[ac].lyrics.Add();
						lyr.LoadTitle(title);
					}
				}
			}
		}
		while (ff.Next());
	}
}

void Database::RealizeTypecasts() {
	const auto& tcs = GetTypecasts();
	const auto& cons = GetContrasts();
	if (typecasts.GetCount() != tcs.GetCount()) {
		typecasts.SetCount(tcs.GetCount());
		for(int i = 0; i < tcs.GetCount(); i++) {
			auto& tc = typecasts[i];
			tc.archetypes.SetCount(cons.GetCount());
		}
	}
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

