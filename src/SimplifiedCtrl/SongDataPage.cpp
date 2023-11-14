#include "SimplifiedCtrl.h"


SongDataPage::SongDataPage() {
	Add(hsplit.HSizePos().VSizePos(0,30));
	Add(prog.HSizePos().BottomPos(0,30));
	
	prog.Set(0,1);
	
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

void SongDataPage::EnableAll() {
	disabled = false;
	datasets.Enable();
	artists.Enable();
	songs.Enable();
	active_songs.Enable();
	analysis.Enable();
}

void SongDataPage::DisableAll() {
	disabled = true;
	datasets.Disable();
	artists.Disable();
	songs.Disable();
	active_songs.Disable();
	analysis.Disable();
}

void SongDataPage::ToolMenu(Bar& bar) {
	bar.Add(t_("Add 10 random songs to list"), AppImg::BlueRing(), THISBACK1(AddRandomSongsToList, 10)).Key(K_CTRL_Q);
	bar.Add(t_("Remove song from list"), AppImg::BlueRing(), THISBACK(RemoveSongFromActiveList)).Key(K_CTRL_W);
	bar.Separator();
	bar.Add(t_("Hotfix text"), AppImg::RedRing(), THISBACK(StartHotfixText)).Key(K_F5);
}

void SongDataPage::AddRandomSongsToList(int count) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	if (!datasets.IsCursor() || !artists.IsCursor() || !songs.IsCursor()) return;
	int cur = datasets.GetCursor();
	int acur = artists.GetCursor();
	const auto& data = db.song_data[cur];
	const auto& artist = data[acur];
	if (artist.lyrics.IsEmpty()) return;
	
	String ds_key = sd.GetKey(cur);
	DatasetAnalysis& ds = sda.datasets.GetAdd(ds_key);
	
	auto& songs = ds.artists.GetAdd(artist.name).songs;
	
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
	SongDataAnalysis& sda = db.song_data.a;
	
	if (!datasets.IsCursor() || !artists.IsCursor() || !songs.IsCursor()) return;
	int cur = datasets.GetCursor();
	int acur = artists.GetCursor();
	int scur = songs.GetCursor();
	const auto& data = db.song_data[cur];
	const auto& artist = data[acur];
	const auto& song = artist.lyrics[scur];
	
	String ds_key = sd.GetKey(cur);
	DatasetAnalysis& ds = sda.datasets.GetAdd(ds_key);
	auto& v = ds.artists.GetAdd(artist.name).songs;
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
	SongDataAnalysis& sda = db.song_data.a;
	
	if (!datasets.IsCursor() || !artists.IsCursor() || !active_songs.IsCursor()) return;
	int cur = datasets.GetCursor();
	int acur = artists.GetCursor();
	int scur = active_songs.GetCursor();
	const auto& data = db.song_data[cur];
	const auto& artist = data[acur];
	
	String ds_key = sd.GetKey(cur);
	DatasetAnalysis& ds = sda.datasets.GetAdd(ds_key);
	
	ds.artists.GetAdd(artist.name).songs.Remove(scur);
	
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
	const auto& data = db.song_data[cur];
	
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
	const auto& data = db.song_data[cur];
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
	SongDataAnalysis& sda = db.song_data.a;
	
	if (!datasets.IsCursor() || !artists.IsCursor()) return;
	int cur = datasets.GetCursor();
	int acur = artists.GetCursor();
	const auto& data = db.song_data[cur];
	const auto& artist = data[acur];
	
	String ds_key = sd.GetKey(cur);
	DatasetAnalysis& ds = sda.datasets.GetAdd(ds_key);
	
	int i = ds.artists.Find(artist.name);
	if (i < 0) {
		active_songs.Clear();
	}
	else {
		const auto& songs = ds.artists[i].songs;
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
	const auto& data = db.song_data[cur];
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
	SongDataAnalysis& sda = db.song_data.a;
	
	if (!datasets.IsCursor() || !artists.IsCursor() || !active_songs.IsCursor()) return;
	int cur = datasets.GetCursor();
	int acur = artists.GetCursor();
	int scur = active_songs.GetCursor();
	const auto& data = db.song_data[cur];
	const auto& artist = data[acur];
	
	String ds_key = sd.GetKey(cur);
	DatasetAnalysis& ds = sda.datasets.GetAdd(ds_key);
	
	lyrics.Clear();
	analysis.Clear();
	
	int i = ds.artists.Find(artist.name);
	if (i < 0)
		return;
	
	const LyricsAnalysis& la = ds.artists[i].songs[scur];
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

void SongDataPage::HotfixText() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	int total = 0;
	for(int i = 0; i < sd.GetCount(); i++) {
		Vector<ArtistDataset>& artists = sd[i];
		for(int j = 0; j < artists.GetCount(); j++) {
			ArtistDataset& artist = artists[j];
			total += artist.lyrics.GetCount();
		}
	}
	
	int actual = 0;
	for(int i = 0; i < sd.GetCount(); i++) {
		Vector<ArtistDataset>& artists = sd[i];
		for(int j = 0; j < artists.GetCount(); j++) {
			ArtistDataset& artist = artists[j];
			for(int k = 0; k < artist.lyrics.GetCount(); k++) {
				LyricsDataset& song = artist.lyrics[k];
				
				ReplaceWord(song.text, "im", "I'm");
				ReplaceWord(song.text, "ive", "I've");
				ReplaceWord(song.text, "ill", "I'll");
				ReplaceWord(song.text, "id", "I'd");
				ReplaceWord(song.text, "youre", "you're");
				ReplaceWord(song.text, "youd", "you'd");
				ReplaceWord(song.text, "youve", "you've");
				ReplaceWord(song.text, "youll", "you'll");
				ReplaceWord(song.text, "hes", "he's");
				ReplaceWord(song.text, "heve", "he've");
				ReplaceWord(song.text, "hed", "he'd");
				ReplaceWord(song.text, "shes", "she's");
				ReplaceWord(song.text, "sheve", "she've");
				ReplaceWord(song.text, "shed", "she'd");
				ReplaceWord(song.text, "theyll", "they'll");
				ReplaceWord(song.text, "theyve", "they've");
				ReplaceWord(song.text, "theyre", "they're");
				
				ReplaceWord(song.text, "didnt", "didn't");
				ReplaceWord(song.text, "dont", "don't");
				
				ReplaceWord(song.text, "its", "it's");
				ReplaceWord(song.text, "itll", "it'll");
				ReplaceWord(song.text, "itve", "it've");
				ReplaceWord(song.text, "isnt", "isn't");
				
				ReplaceWord(song.text, "whats", "what's");
				ReplaceWord(song.text, "couldnt", "couldn't");
				ReplaceWord(song.text, "shouldnt", "shouldn't");
				ReplaceWord(song.text, "theres", "there's");
				ReplaceWord(song.text, "wasnt", "wasn't");
				ReplaceWord(song.text, "alright", "allright");
				ReplaceWord(song.text, "thats", "that's");
				
				// These change too much
				if (0) {
					ReplaceWord(song.text, "tryna", "tring to");
					ReplaceWord(song.text, "aint", "aren't");
					ReplaceWord(song.text, "gotta", "have to");
					ReplaceWord(song.text, "wanna", "want to");
					ReplaceWord(song.text, "em", "them");
					ReplaceWord(song.text, "ol", "old");
					ReplaceWord(song.text, "bout", "about");
					ReplaceWord(song.text, "nunya", "none of your");
					ReplaceWord(song.text, "thang", "thing");
					ReplaceWord(song.text, "I'ma", "I'll");
				}
				
				ReplaceWord(song.text, "tryin", "trying");
				ReplaceWord(song.text, "fuckin", "fucking");
				ReplaceWord(song.text, "livin", "living");
				ReplaceWord(song.text, "lookin", "looking");
				ReplaceWord(song.text, "prayin", "praying");
				ReplaceWord(song.text, "rollin", "rolling");
				ReplaceWord(song.text, "workin", "working");
				ReplaceWord(song.text, "chargin", "charging");
				ReplaceWord(song.text, "runnin", "running");
				ReplaceWord(song.text, "doin", "doing");
				ReplaceWord(song.text, "judgin", "judging");
				ReplaceWord(song.text, "blendin", "blending");
				ReplaceWord(song.text, "gettin", "getting");
				ReplaceWord(song.text, "talkin", "talking");
				ReplaceWord(song.text, "changin", "changing");
				ReplaceWord(song.text, "makin", "making");
				ReplaceWord(song.text, "retracin", "retracing");
				ReplaceWord(song.text, "motherfuckin", "motherfucking");
				ReplaceWord(song.text, "rockin", "rocking");
				ReplaceWord(song.text, "goin", "going");
				ReplaceWord(song.text, "frontin", "fronting");
				ReplaceWord(song.text, "somethin", "something");
				ReplaceWord(song.text, "playin", "playing");
				ReplaceWord(song.text, "hittin", "hitting");
				ReplaceWord(song.text, "movin", "moving");
				
				
				if (actual++ % 100 == 0)
					PostProgress(actual, total);
			}
		}
	}
	
	PostCallback(THISBACK(EnableAll));
}
