#include "SimplifiedCtrl.h"


SongDataPage::SongDataPage() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << vsplit << lyrics;
	hsplit.SetPos(3333);
	
	vsplit.Vert() << datasets << artists << songs;
	
	datasets.AddColumn(t_("Dataset"));
	datasets.WhenCursor << THISBACK(DataDataset);
	
	artists.AddColumn(t_("Artist"));
	artists.WhenCursor << THISBACK(DataArtist);
	
	songs.AddColumn(t_("Song"));
	songs.WhenCursor << THISBACK(DataSong);
	
	
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
	
	DataSong();
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
}

