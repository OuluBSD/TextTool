#include "SimplifiedCtrl.h"

SongDataWords::SongDataWords() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << vsplit << words;
	hsplit.SetPos(2500);
	
	vsplit.Vert() << datasets << artists << wordgroups;
	
	datasets.AddColumn(t_("Dataset"));
	datasets.WhenCursor << THISBACK(DataDataset);
	
	wordgroups.AddColumn(t_("Word group"));
	wordgroups.AddColumn(t_("Is a word flag group"));
	wordgroups.ColumnWidths("6 1");
	wordgroups.WhenCursor << THISBACK(DataWordgroup);
	
	artists.AddColumn(t_("Artist"));
	artists.WhenCursor << THISBACK(DataArtist);
	
	words.AddColumn(t_("Word"));
	words.AddColumn(t_("Syllables"));
	words.AddColumn(t_("Phonetic syllables"));
	words.AddColumn(t_("Word count"));
	words.AddColumn(t_("Flags"));
	words.ColumnWidths("1 1 1 1 2");
	words.AddIndex("IDX");
	
}

void SongDataWords::EnableAll() {
	disabled = false;
	words.Enable();
	datasets.Enable();
	wordgroups.Enable();
	artists.Enable();
}

void SongDataWords::DisableAll() {
	disabled = true;
	words.Disable();
	datasets.Disable();
	wordgroups.Disable();
	artists.Disable();
}

void SongDataWords::Data() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	
	
	datasets.SetCount(sd.GetCount());
	for(int i = 0; i < sd.GetCount(); i++) {
		String key = sd.GetKey(i);
		datasets.Set(i, 0, key);
	}
	
	if (!datasets.IsCursor() && datasets.GetCount())
		datasets.SetCursor(0);
	
	DataDataset();
}

void SongDataWords::DataDataset() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	if (!datasets.IsCursor())
		return;
	
	int ds_i = datasets.GetCursor();
	
	artists.Set(0, 0, t_("All artists"));
	
	String ds_key = sd.GetKey(ds_i);
	DatasetAnalysis& ds = sda.datasets.GetAdd(ds_key);
	
	Vector<ArtistDataset>& avec = sd[ds_i];
	artists.SetCount(avec.GetCount());
	for(int j = 0; j < avec.GetCount(); j++) {
		const ArtistDataset& artist = avec[j];
		const ArtistAnalysis& aa = ds.artists.GetAdd(artist.name);
		artists.Set(1+j, 0, artist.name);
	}
	
	if (!artists.IsCursor() && artists.GetCount())
		artists.SetCursor(0);
	
	DataArtist();
}

void SongDataWords::DataArtist() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	if (!datasets.IsCursor() || !artists.IsCursor())
		return;
	
	int ds_i = datasets.GetCursor();
	int a_i = artists.GetCursor();
	
	String ds_key = sd.GetKey(ds_i);
	DatasetAnalysis& ds = sda.datasets.GetAdd(ds_key);
	Vector<ArtistDataset>& avec = sd[ds_i];
	
	if (a_i == 0) {
		wordgroups.SetCount(1+ds.groups.GetCount());
		wordgroups.Set(0, 0, t_("All words"));
		for(int i = 0; i < ds.groups.GetCount(); i++) {
			wordgroups.Set(1+i, 0, ds.groups.GetKey(i));
		}
	}
	else {
		a_i--;
		const Vector<ArtistDataset>& avec = sd[ds_i];
		const ArtistDataset& artist = avec[a_i];
		const ArtistAnalysis& aa = ds.artists.GetAdd(artist.name);
		
		wordgroups.SetCount(1);
		wordgroups.Set(0, 0, t_("All words"));
		
		/*
		for(int j = 0; j < artist.lyrics.GetCount(); j++) {
			const ArtistAnalysis& aa = aa.lyrics[j];
			artists.Set(j, 0, artist.name);
		}
		
		if (!artists.IsCursor() && artists.GetCount())
			artists.SetCursor(0);*/
	}
	
	if (wordgroups.GetCount() && !wordgroups.IsCursor())
		wordgroups.SetCursor(0);
	
	DataWordgroup();
}

void SongDataWords::DataWordgroup() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	if (!datasets.IsCursor() || !artists.IsCursor() || !wordgroups.IsCursor())
		return;
	
	int ds_i = datasets.GetCursor();
	int a_i = artists.GetCursor();
	int wg_i = wordgroups.GetCursor();
	String ds_key = sd.GetKey(ds_i);
	DatasetAnalysis& ds = sda.datasets.GetAdd(ds_key);
	
	if (a_i == 0 && wg_i == 0) {
		int limit = ds.words.GetCount() > 10000 ? 10 : 0;
		int row = 0;
		for(int i = 0; i < ds.words.GetCount(); i++) {
			String key = ds.words.GetKey(i);
			const WordAnalysis& wa = ds.words[i];
			if (wa.count < limit)
				continue;
			
			words.Set(row, 0, key);
			words.Set(row, 3, wa.count);
			
			String s;
			for(int j = 0; j < wa.group_is.GetCount(); j++) {
				if (j) s << ", ";
				s << ds.groups.GetKey(wa.group_is[j]);
			}
			words.Set(row, 4, s);
			words.Set(row, "IDX", i);
			row++;
		}
		words.SetCount(row);
		words.SetSortColumn(3, true);
	}
	else if (a_i > 0 && wg_i == 0) {
		a_i--;
		const Vector<ArtistDataset>& avec = sd[ds_i];
		const ArtistDataset& artist = avec[a_i];
		const ArtistAnalysis& aa = ds.artists.GetAdd(artist.name);
		words.SetCount(aa.word_counts.GetCount());
		for(int i = 0; i < aa.word_counts.GetCount(); i++) {
			words.Set(i, 0, aa.word_counts.GetKey(i));
			words.Set(i, 1, Value());
			words.Set(i, 2, Value());
			words.Set(i, 3, aa.word_counts[i]);
			words.Set(i, 4, Value());
			words.Set(i, "IDX", i);
		}
		words.SetSortColumn(3, true);
	}
	else if (a_i == 0 && wg_i > 0) {
		wg_i--;
		const WordGroupAnalysis& wga = ds.groups[wg_i];
		words.SetCount(wga.values.GetCount());
		for(int i = 0; i < wga.values.GetCount(); i++) {
			words.Set(i, 0, wga.values.GetKey(i));
			words.Set(i, 1, Value());
			words.Set(i, 2, Value());
			words.Set(i, 3, wga.values[i]);
			words.Set(i, 4, Value());
			words.Set(i, "IDX", i);
		}
		words.SetSortColumn(3, true);
	}
	else {
		a_i--;
		wg_i--;
		words.SetCount(0);
	}
	
	
}

void SongDataWords::ToolMenu(Bar& bar) {
	bar.Add(t_("Update all words"), AppImg::RedRing(), THISBACK(UpdateWords)).Key(K_F5);
	bar.Add(t_("Update all word groups"), AppImg::RedRing(), THISBACK(UpdateWordFlagGroups)).Key(K_F6);
	bar.Add(t_("Update all word flags"), AppImg::RedRing(), THISBACK(UpdateWordFlags)).Key(K_F7);
	bar.Separator();
	bar.Add(t_("Update word syllables"), AppImg::BlueRing(), THISBACK2(GetSyllables, -1, false)).Key(K_CTRL_Q);
	bar.Add(t_("Update all syllables"), AppImg::RedRing(), THISBACK2(GetSyllables, 0, true)).Key(K_F8);
	
}

void SongDataWords::UpdateWords() {
	if (disabled) return;
	DisableAll();
	Thread::Start(THISBACK(UpdateWordsProcess));
}

void SongDataWords::UpdateWordsProcess() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	
	for(int i = 0; i < sd.GetCount(); i++) {
		String ds_key = sd.GetKey(i);
		DatasetAnalysis& ds = sda.datasets.GetAdd(ds_key);
		
		ds.words.Clear();
		
		const Vector<ArtistDataset>& dataset = sd[i];
		for(int j = 0; j < dataset.GetCount(); j++) {
			const ArtistDataset& artist = dataset[j];
			ArtistAnalysis& aa = ds.artists.GetAdd(artist.name);
			aa.word_counts.Clear();
			
			for(int k = 0; k < artist.lyrics.GetCount(); k++) {
				const LyricsDataset& song = artist.lyrics[k];
				
				String text = song.text;
				if (GetDefaultCharset() != CHARSET_UTF8)
					text = ToCharset(CHARSET_DEFAULT, text, CHARSET_UTF8);
				
				Vector<String> lines = Split(text, "\n");
				for (String& l : lines) {
					Vector<String> words;
					GetWords(l, words);
					if (words.IsEmpty()) continue;
					
					for (String& w : words) {
						w = ToLower(w.ToWString()).ToString();
					}
					
					aa.total_words += words.GetCount();
					for (String& w : words) {
						aa.word_counts.GetAdd(w, 0)++;
						WordAnalysis& wa = ds.words.GetAdd(w);
						wa.count++;
					}
				}
			}
		}
	}
	
	
	PostCallback(THISBACK(EnableAll));
}

void SongDataWords::UpdateWordFlagGroups() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	for(int ds_i = 0; ds_i < sd.GetCount(); ds_i++) {
		String ds_key = sd.GetKey(ds_i);
		DatasetAnalysis& ds = sda.datasets.GetAdd(ds_key);
			
		ds.groups.Clear();
		
		for(int i = 0; i < ds.artists.GetCount(); i++) {
			String key = ds.artists.GetKey(i);
			ArtistAnalysis& aa = ds.artists[i];
			
			for(int j = 0; j < aa.songs.GetCount(); j++) {
				LyricsAnalysis& la = aa.songs[j];
				
				for(int k = 0; k < la.word_groups.GetCount(); k++) {
					String group = la.word_groups.GetKey(k);
					group = ToLower(group);
					if (group.Left(1) == "-")
						group = group.Mid(1);
					group = TrimBoth(group);
					if (group.IsEmpty())
						continue;
					const Vector<String>& values = la.word_groups[k];
					WordGroupAnalysis& ga = ds.groups.GetAdd(group);
					for (const String& value : values)
						ga.values.GetAdd(value, 0)++;
				}
			}
		}
		SortByKey(ds.groups, StdLess<String>());
	}
	
	PostCallback(THISBACK(DataArtist));
}

void SongDataWords::UpdateWordFlags() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	for(int ds_i = 0; ds_i < sd.GetCount(); ds_i++) {
		String ds_key = sd.GetKey(ds_i);
		DatasetAnalysis& ds = sda.datasets.GetAdd(ds_key);
		
		for (auto& wa : ds.words)
			wa.group_is.Clear();
		
		for(int i = 0; i < ds.groups.GetCount(); i++) {
			String wg_key = ds.groups.GetKey(i);
			const WordGroupAnalysis& wga = ds.groups[i];
			
			for(int j = 0; j < wga.values.GetCount(); j++) {
				String wrd = wga.values.GetKey(j);
				int k = ds.words.Find(wrd);
				if (k >= 0) {
					WordAnalysis& wa = ds.words[k];
					wa.group_is.Add(i);
				}
			}
		}
	}
}

void SongDataWords::GetSyllables(int batch_i, bool start_next) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	int begin = batch_i * per_batch;
	int end = (batch_i+1) * per_batch;
	
	if (batch_i < 0) {
		begin = 0;
		end = 1;
	}
	
	SongDataAnalysisArgs args;
	
	int iter = 0;
	for(int ds_i = 0; ds_i < sd.GetCount(); ds_i++) {
		String ds_key = sd.GetKey(ds_i);
		DatasetAnalysis& ds = sda.datasets.GetAdd(ds_key);
		
		for(int i = 0; i < ds.words.GetCount(); i++) {
			if (iter >= begin) {
				const String& wrd = ds.words.GetKey(i);
				args.words << wrd;
			}
			iter++;
			if (iter >= end) break;
		}
		if (iter >=  end) break;
	}
	
	if (args.words.IsEmpty())
		return;
	
	Song& song = GetSong();
	song.RealizePipe();
	Pipe& pipe = *song.pipe;
	TaskMgr& m = pipe;
	
	args.fn = 4;
	
	m.GetSongDataAnalysis(args, THISBACK2(OnSyllables, batch_i, start_next));
}

void SongDataWords::OnSyllables(String res, int batch_i, bool start_next) {
	
	
	
	if (start_next) {
		disabled = false;
		GetSyllables(batch_i+1, true);
	}
}

