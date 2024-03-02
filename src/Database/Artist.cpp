#include "Database.h"

void Artist::Store() {
	String dir = Database::Single().GetArtistsDir();
	RealizeDirectory(dir);
	String json_path = dir + file_title + ".json";
	StoreAsJsonFileStandard(*this, json_path, true);
	
	StoreLyrics();
}

void Artist::LoadTitle(String title) {
	String dir = Database::Single().GetArtistsDir();
	file_title = title;
	String json_path = dir + file_title + ".json";
	LoadFromJsonFileStandard(*this, json_path);
	
	LoadLyrics();
}

void Artist::StoreLyrics() {
	for (auto& tc : typecasts) {
		for (auto& at : tc.archetypes) {
			for (auto& l : at.lyrics) {
				l.Store(*this);
			}
		}
	}
}

void Artist::LoadLyrics() {
	Database& db = Database::Single();
	RealizeTypecasts();
	FindFile ff(AppendFileName(GetLyricsDir(), "*.json"));
	do {
		String path = ff.GetPath();
		String title = GetFileTitle(path);
		Lyrics lyr;
		lyr.LoadTitle(*this, title);
		
		const auto& typecasts = GetTypecasts();
		const auto& archetypes = GetContrasts();
		if (lyr.typecast >= 0 && lyr.typecast < typecasts.GetCount() &&
			lyr.archetype >= 0 && lyr.archetype < archetypes.GetCount()) {
			this->typecasts[lyr.typecast].archetypes[lyr.archetype].lyrics.Add()
				.LoadTitle(*this, title); // TODO avoid duplicate loading
		}
	}
	while (ff.Next());

}

void Artist::RealizeTypecasts() {
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

String Artist::GetLyricsDir() const {
	ASSERT(!file_title.IsEmpty());
	Database& db = Database::Single();
	return db.dir + DIR_SEPS "share" DIR_SEPS "lyrics" DIR_SEPS + file_title + DIR_SEPS;
}

bool Artist::FindSong(int& tc_i, int& arch_i, int& lyr_i, const String& lyrics_file_title) const {
	tc_i = -1;
	arch_i = -1;
	lyr_i = -1;
	for(int i = 0; i < typecasts.GetCount(); i++) {
		const Typecast& tc = typecasts[i];
		for(int j = 0; j < tc.archetypes.GetCount(); j++) {
			const Archetype& arch = tc.archetypes[j];
			for(int k = 0; k < arch.lyrics.GetCount(); k++) {
				const Lyrics& lyr = arch.lyrics[k];
				if (lyr.file_title == lyrics_file_title) {
					tc_i = i;
					arch_i = j;
					lyr_i = k;
					return true;
				}
			}
		}
	}
	return false;
}
