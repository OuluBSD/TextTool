#include "SongDatabase.h"


BEGIN_SONGLIB_NAMESPACE


int SongDatabase::trans_i = -1;


SongDatabase::SongDatabase() {
	
}

String SongDatabase::GetArtistsDir() const {
	return dir + DIR_SEPS "share" DIR_SEPS "artists" DIR_SEPS;
}

String SongDatabase::GetReleasesDir() const {
	return dir + DIR_SEPS "share" DIR_SEPS "releases" DIR_SEPS;
}

String SongDatabase::GetSongsDir() const {
	return dir + DIR_SEPS "share" DIR_SEPS "songs" DIR_SEPS;
}

void SongDatabase::Store() {
	StoreAsJsonFileStandard(*this, dir + DIR_SEPS "share" DIR_SEPS "db.json", true);
}

void SongDatabase::Load() {
	Clear();
	
	lock.EnterWrite();
	LoadFromJsonFileStandard(*this, dir + DIR_SEPS "share" DIR_SEPS "db.json");
	lock.LeaveWrite();
	
}

void SongDatabase::FindOrphaned() {
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

String SongDatabase::Translate(const String& s) {
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






END_SONGLIB_NAMESPACE



int EditorPtrs::GetActiveArtistIndex() const {return VectorFindPtr(artist, SongLib::SongDatabase::Single().artists);}
int EditorPtrs::GetActiveReleaseIndex() const {if (!artist) return -1; return VectorFindPtr(release, artist->releases);}
int EditorPtrs::GetActiveSongIndex() const {if (!release) return -1; return VectorFindPtr(song, release->songs);}
int EditorPtrs::GetActiveTypecastIndex() const {return VectorFindPtr(typecast, artist->typecasts);}
int EditorPtrs::GetActiveArchetypeIndex() const {return VectorFindPtr(archetype, typecast->archetypes);}
int EditorPtrs::GetActiveLyricsIndex() const {return VectorFindPtr(lyrics, archetype->lyrics);}


template <>
void CheckSerialisationData<SongLib::Song>(const String& json) {
	SongLib::Song song;
	LoadFromJson(song, json);
	//ASSERT(song.native_title.GetCount() || song.english_title.GetCount());
}
