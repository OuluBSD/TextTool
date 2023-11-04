#include "SimplifiedCtrl.h"


SongDataPage::SongDataPage() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << vsplit << lyrics << analysis;
	hsplit.SetPos(2500);
	
	vsplit.Vert() << datasets << artists << songs << active_songs;
	
	datasets.AddColumn(t_("Dataset"));
	datasets.WhenCursor << THISBACK(DataDataset);
	
	artists.AddColumn(t_("Artist"));
	artists.WhenCursor << THISBACK(DataArtist);
	
	songs.AddColumn(t_("Song"));
	songs.WhenCursor << THISBACK(DataSong);
	songs.WhenBar << [this](Bar& bar) {
		bar.Add(t_("Add song to active list"), THISBACK(AddSongToActiveList));
	};
	
	active_songs.AddColumn(t_("Song"));
	active_songs.WhenCursor << THISBACK(DataActiveSong);
	active_songs.WhenBar << [this](Bar& bar) {
		bar.Add(t_("Remove song from active list"), THISBACK(RemoveSongFromActiveList));
	};
	
	
}

void SongDataPage::ToolMenu(Bar& bar) {
	bar.Add(t_("Add 10 random songs to list"), AppImg::BlueRing(), THISBACK1(AddRandomSongsToList, 10)).Key(K_CTRL_Q);
	bar.Add(t_("Remove song from list"), AppImg::BlueRing(), THISBACK(RemoveSongFromActiveList)).Key(K_CTRL_W);
}

void SongDataPage::AddRandomSongsToList(int count) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	
	if (!datasets.IsCursor() || !artists.IsCursor() || !songs.IsCursor()) return;
	int cur = datasets.GetCursor();
	int acur = artists.GetCursor();
	const auto& data = cur == 0 ? db.song_data.artists_en : db.song_data.artists_fi;
	const auto& artist = data[acur];
	if (artist.lyrics.IsEmpty()) return;
	
	auto& songs = sd.active_songs.GetAdd(artist.name);
	
	for(int i = 0; i < count; i++) {
		for (int tries = 0; tries < 1000; tries++) {
			int scur = Random(artist.lyrics.GetCount());
			const auto& song = artist.lyrics[scur];
			int k = -1;
			for(int j = 0; j < songs.GetCount(); j++) {
				if (songs[j].name == song.name) {
					k = j;
					break;
				}
			}
			if (k >= 0)
				continue;
			songs.Add().name = song.name;
			break;
		}
	}
	
	PostCallback(THISBACK(DataArtistActiveSongs));
	PostCallback(THISBACK(DataActiveSong));
}

void SongDataPage::AddSongToActiveList() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	
	if (!datasets.IsCursor() || !artists.IsCursor() || !songs.IsCursor()) return;
	int cur = datasets.GetCursor();
	int acur = artists.GetCursor();
	int scur = songs.GetCursor();
	const auto& data = cur == 0 ? db.song_data.artists_en : db.song_data.artists_fi;
	const auto& artist = data[acur];
	const auto& song = artist.lyrics[scur];
	
	auto& v = sd.active_songs.GetAdd(artist.name);
	int j = -1;
	for(int i = 0; i < v.GetCount(); i++) if (v[i].name == song.name) {j = i; break;}
	if (j < 0)
		v.Add().name = song.name;
	
	PostCallback(THISBACK(DataArtistActiveSongs));
	PostCallback(THISBACK(DataActiveSong));
}

void SongDataPage::RemoveSongFromActiveList() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	
	if (!datasets.IsCursor() || !artists.IsCursor() || !active_songs.IsCursor()) return;
	int cur = datasets.GetCursor();
	int acur = artists.GetCursor();
	int scur = active_songs.GetCursor();
	const auto& data = cur == 0 ? db.song_data.artists_en : db.song_data.artists_fi;
	const auto& artist = data[acur];
	
	sd.active_songs.GetAdd(artist.name).Remove(scur);
	
	PostCallback(THISBACK(DataArtistActiveSongs));
	PostCallback(THISBACK(DataActiveSong));
}

void SongDataPage::Data() {
	Database& db = Database::Single();
	
	datasets.Set(0, 0, "English");
	datasets.Set(1, 0, "Finnish");
	
	if (!datasets.IsCursor())
		datasets.SetCursor(0);
	
	DataDataset();
}

void SongDataPage::DataDataset() {
	Database& db = Database::Single();
	
	if (!datasets.IsCursor()) return;
	int cur = datasets.GetCursor();
	const auto& data = cur == 0 ? db.song_data.artists_en : db.song_data.artists_fi;
	
	artists.SetCount(data.GetCount());
	for(int i = 0; i < data.GetCount(); i++) {
		String s = data[i].name;
		if (GetDefaultCharset() != CHARSET_UTF8)
			s = ToCharset(CHARSET_DEFAULT, s, CHARSET_UTF8);
		
		artists.Set(i, 0, s);
	}
	
	if (!artists.IsCursor() && artists.GetCount())
		artists.SetCursor(0);
	
	DataArtist();
}

void SongDataPage::DataArtist() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	
	if (!datasets.IsCursor() || !artists.IsCursor()) return;
	int cur = datasets.GetCursor();
	int acur = artists.GetCursor();
	const auto& data = cur == 0 ? db.song_data.artists_en : db.song_data.artists_fi;
	const auto& artist = data[acur];
	
	songs.SetCount(artist.lyrics.GetCount());
	for(int i = 0; i < artist.lyrics.GetCount(); i++) {
		String s = artist.lyrics[i].name;
		if (GetDefaultCharset() != CHARSET_UTF8)
			s = ToCharset(CHARSET_DEFAULT, s, CHARSET_UTF8);
		
		songs.Set(i, 0, s);
	}
	
	if (!songs.IsCursor() && songs.GetCount())
		songs.SetCursor(0);
	
	DataArtistActiveSongs();
	DataSong();
}

void SongDataPage::DataArtistActiveSongs() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	
	if (!datasets.IsCursor() || !artists.IsCursor()) return;
	int cur = datasets.GetCursor();
	int acur = artists.GetCursor();
	const auto& data = cur == 0 ? db.song_data.artists_en : db.song_data.artists_fi;
	const auto& artist = data[acur];
	
	int i = sd.active_songs.Find(artist.name);
	if (i < 0) {
		active_songs.Clear();
	}
	else {
		const auto& songs = sd.active_songs[i];
		active_songs.SetCount(songs.GetCount());
		for(int i = 0; i < songs.GetCount(); i++) {
			active_songs.Set(i, 0, songs[i].name);
		}
	}
}

void SongDataPage::DataSong() {
	Database& db = Database::Single();
	
	if (!datasets.IsCursor() || !artists.IsCursor() || !songs.IsCursor()) return;
	int cur = datasets.GetCursor();
	int acur = artists.GetCursor();
	int scur = songs.GetCursor();
	const auto& data = cur == 0 ? db.song_data.artists_en : db.song_data.artists_fi;
	const auto& artist = data[acur];
	const auto& song = artist.lyrics[scur];
	
	String s = song.text;
	if (GetDefaultCharset() != CHARSET_UTF8)
		s = ToCharset(CHARSET_DEFAULT, s, CHARSET_UTF8);
	lyrics.SetData(s);
	analysis.Clear();
}

void SongDataPage::DataActiveSong() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	
	if (!datasets.IsCursor() || !artists.IsCursor() || !active_songs.IsCursor()) return;
	int cur = datasets.GetCursor();
	int acur = artists.GetCursor();
	int scur = active_songs.GetCursor();
	const auto& data = cur == 0 ? db.song_data.artists_en : db.song_data.artists_fi;
	const auto& artist = data[acur];
	
	lyrics.Clear();
	analysis.Clear();
	
	int i = sd.active_songs.Find(artist.name);
	if (i < 0)
		return;
	
	const LyricsAnalysis& la = sd.active_songs[i][scur];
	String song_name = la.name;
	for(int i = 0; i < artist.lyrics.GetCount(); i++) {
		const LyricsDataset& song = artist.lyrics[i];
		if (song.name == song_name) {
			String s = song.text;
			if (GetDefaultCharset() != CHARSET_UTF8)
				s = ToCharset(CHARSET_DEFAULT, s, CHARSET_UTF8);
			lyrics.SetData(s);
			
			s = la.AsString();
			if (GetDefaultCharset() != CHARSET_UTF8)
				s = ToCharset(CHARSET_DEFAULT, s, CHARSET_UTF8);
			analysis.SetData(s);
		}
	}
}

