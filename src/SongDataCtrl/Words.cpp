#include "SongDataCtrl.h"

SongDataWords::SongDataWords() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << vsplit << words;
	hsplit.SetPos(2500);
	
	vsplit.Vert() << datasets << artists << colors;
	
	datasets.AddColumn(t_("Dataset"));
	datasets.WhenCursor << THISBACK(DataDataset);
	
	colors.AddColumn(t_("Word group"));
	colors.ColumnWidths("1");
	colors.WhenCursor << THISBACK(DataColor);
	
	artists.AddColumn(t_("Artist"));
	artists.WhenCursor << THISBACK(DataArtist);
	
	words.AddColumn(t_("Word"));
	words.AddColumn(t_("Translation"));
	words.AddColumn(t_("Main class"));
	words.AddColumn(t_("Spelling"));
	words.AddColumn(t_("Phonetic spelling"));
	words.AddColumn(t_("Word count"));
	//words.AddColumn(t_("Flags"));
	words.ColumnWidths("1 1 1 1 1 1");
	words.AddIndex("IDX");
	
}

void SongDataWords::EnableAll() {
	disabled = false;
	words.Enable();
	datasets.Enable();
	colors.Enable();
	artists.Enable();
}

void SongDataWords::DisableAll() {
	disabled = true;
	words.Disable();
	datasets.Disable();
	colors.Disable();
	artists.Disable();
}

void SongDataWords::Data() {
	// Pass
}

void SongDataWords::DataMain() {
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
	
	
	colors.SetCount(1+GetColorGroupCount());
	colors.Set(0, 0, t_("All words"));
	for(int i = 0; i < GetColorGroupCount(); i++) {
		colors.Set(1+i, 0,
			AttrText("#" + IntStr(i))
				.NormalPaper(GetGroupColor(i)).NormalInk(Black())
				.Paper(Blend(GrayColor(), GetGroupColor(i))).Ink(White()));
	}
	if (colors.GetCount() && !colors.IsCursor())
		colors.SetCursor(0);
	
	DataColor();
}

void SongDataWords::DataColor() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	if (!datasets.IsCursor() || !artists.IsCursor() || !colors.IsCursor())
		return;
	
	int ds_i = datasets.GetCursor();
	int a_i = artists.GetCursor();
	int wg_i = colors.GetCursor();
	String ds_key = sd.GetKey(ds_i);
	DatasetAnalysis& ds = sda.datasets.GetAdd(ds_key);
	const Vector<ArtistDataset>& avec = sd[ds_i];
	
	{
		bool clr_filter = wg_i > 0;
		bool loop_artist = a_i > 0;
		a_i--;
		wg_i--;
		const ArtistDataset* artist = 0;
		const ArtistAnalysis* aa = 0;
		if (loop_artist) {
			artist = &avec[a_i];
			aa = &ds.artists.GetAdd(artist->name);
		}
		
		int count = loop_artist ? aa->word_counts.GetCount () : ds.words.GetCount();
		int limit = count > 10000 ? 10 : 0;
		int row = 0;
		for (int i = 0; i < count; i++) {
			String key;
			WordAnalysis* wap;
			if (loop_artist) {
				key = aa->word_counts.GetKey(i);
				int j = ds.FindWord(key);
				if (j < 0) continue;
				wap = &ds.words[j];
			}
			else {
				key = ds.words[i].txt;
				wap = &ds.words[i];
			}
			WordAnalysis& wa = *wap;
			
			if (wa.count < limit)
				continue;
			
			// Filter by color group
			if (clr_filter && GetColorGroup(wa.clr) != wg_i)
				continue;
			
			// If details have parsed
			if (wa.main_class.GetCount()) {
				words.Set(row, 0,
					AttrText(key)
						.NormalPaper(Blend(wa.clr, White(), 128+64)).NormalInk(Black())
						.Paper(Blend(wa.clr, GrayColor())).Ink(White())
				);
				words.Set(row, 1,
					AttrText(wa.translation)
						.NormalPaper(Blend(wa.clr, White(), 128+64)).NormalInk(Black())
						.Paper(Blend(wa.clr, GrayColor())).Ink(White())
				);
			}
			else {
				words.Set(row, 0, key);
				words.Set(row, 1, wa.translation);
			}
			
			bool has_main_class_clr = wa.main_class_clr != Black();
			if (!has_main_class_clr) {
				int j = GetWordgroupColors().Find(wa.main_class);
				if (j >= 0) {
					wa.main_class_clr = GetWordgroupColors()[j];
					has_main_class_clr = true;
				}
			}
			if (has_main_class_clr) {
				words.Set(row, 2,
					AttrText(wa.main_class)
						.NormalPaper(Blend(wa.main_class_clr, White(), 128+64)).NormalInk(Black())
						.Paper(Blend(wa.main_class_clr, GrayColor())).Ink(White())
				);
			}
			else {
				words.Set(row, 2, wa.main_class);
			}
			words.Set(row, 3, wa.spelling);
			words.Set(row, 4, wa.phonetic);
			words.Set(row, 5, wa.count);
			
			/*String s;
			for(int j = 0; j < wa.group_is.GetCount(); j++) {
				if (j) s << ", ";
				s << ds.groups.GetKey(wa.group_is[j]);
			}
			words.Set(row, 6, s);*/
			words.Set(row, "IDX", i);
			row++;
		}
		words.SetCount(row);
		words.SetSortColumn(5, true);
	}
}

void SongDataWords::ToolMenu(Bar& bar) {
	bar.Add(t_("Update Data"), AppImg::BlueRing(), THISBACK(DataMain)).Key(K_CTRL_Q);
	bar.Separator();
	bar.Add(t_("Update all words"), AppImg::RedRing(), THISBACK(UpdateWords)).Key(K_F5);
	//bar.Add(t_("Update all word groups"), AppImg::RedRing(), THISBACK(UpdateWordFlagGroups)).Key(K_F6);
	//bar.Add(t_("Update all word flags"), AppImg::RedRing(), THISBACK(UpdateWordFlags)).Key(K_F7);
	bar.Separator();
	bar.Add(t_("Get details"), AppImg::BlueRing(), THISBACK2(GetDetails, -1, false)).Key(K_CTRL_W);
	bar.Add(t_("Get all details"), AppImg::RedRing(), THISBACK2(GetDetails, 0, true)).Key(K_F6);
	bar.Separator();
	bar.Add(t_("Get word syllables"), AppImg::BlueRing(), THISBACK2(GetSyllables, -1, false)).Key(K_CTRL_Q);
	bar.Add(t_("Get all syllables"), AppImg::RedRing(), THISBACK2(GetSyllables, 0, true)).Key(K_F7);
	bar.Separator();
	bar.Add(t_("Get everything"), AppImg::RedRing(), THISBACK(GetEverything)).Key(K_F8);
	bar.Separator();
	bar.Add(t_("Debug dump word groups"), AppImg::BlueRing(), THISBACK(DumpWordGroups)).Key(K_F9);
	bar.Add(t_("Debug dump phonetic characters"), AppImg::BlueRing(), THISBACK(DumpPhoneticChars)).Key(K_F10);
	
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
						WordAnalysis& wa = ds.GetAddWord(w);
						wa.count++;
					}
				}
			}
		}
	}
	
	
	PostCallback(THISBACK(EnableAll));
}

/*void SongDataWords::UpdateWordFlagGroups() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	Index<String> unique_wordgroups;
	
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
					unique_wordgroups.FindAdd(group);
				}
			}
		}
		SortByKey(ds.groups, StdLess<String>());
	}
	
	DUMPC(unique_wordgroups);
	
	PostCallback(THISBACK(DataArtist));
}*/

/*void SongDataWords::UpdateWordFlags() {
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
}*/

void SongDataWords::GetEverything() {
	if (disabled) return;
	batch = true;
	GetDetails(0, true);
}

void SongDataWords::GetSyllables(int batch_i, bool start_next) {
	if (Thread::IsShutdownThreads())
		return;
	
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
	tmp_map_ds_i.Clear();
	
	int iter = 0;
	for(int ds_i = 0; ds_i < sd.GetCount(); ds_i++) {
		String ds_key = sd.GetKey(ds_i);
		DatasetAnalysis& ds = sda.datasets.GetAdd(ds_key);
		
		for(int i = 0; i < ds.words.GetCount(); i++) {
			if (iter >= begin) {
				String wrd = ds.words[i].txt;
				
				// hotfix
				HotfixReplaceWord(wrd);
				
				args.words << wrd;
				tmp_map_ds_i.Add(wrd, ds_i);
			}
			iter++;
			if (iter >= end) break;
		}
		if (iter >=  end) break;
	}
	
	if (args.words.IsEmpty()) {
		if (batch)
			batch = false;
		return;
	}
	
	Song& song = GetSong();
	song.RealizePipe();
	TaskMgr& pipe = *song.pipe;
	TaskMgr& m = pipe;
	
	args.fn = 4;
	
	m.GetSongDataAnalysis(args, THISBACK2(OnSyllables, batch_i, start_next));
}

void SongDataWords::OnSyllables(String res, int batch_i, bool start_next) {
	if (Thread::IsShutdownThreads())
		return;
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	//-hey: hey [heɪ]
	//- hello: hel-lo [hɛˈloʊ]
	
	res.Replace("\r", "");
	Vector<String> lines = Split(res, "\n");
	for (String& line : lines) {
		RemoveLineChar(line);
		int a = line.Find(":");
		if (a < 0) continue;
		String wrd = TrimBoth(line.Left(a));
		line = line.Mid(a+1);
		
		a = line.Find("[");
		if (a < 0) continue;
		String spelling = TrimBoth(line.Left(a));
		a++;
		int b = line.Find("]", a);
		if (b < 0) continue;
		WString phonetic = TrimBoth(line.Mid(a,b-a)).ToWString();
		
		int ds_i = tmp_map_ds_i.Get(wrd, -1);
		if (ds_i < 0) continue;
		String ds_key = sd.GetKey(ds_i);
		
		// hotfix
		if (1) {
			wrd = ToLower(wrd.ToWString()).ToString();
			wrd.Replace("'", "");
		}
		
		DatasetAnalysis& ds = sda.datasets.GetAdd(ds_key);
		WordAnalysis& wa = ds.GetAddWord(wrd);
		wa.spelling = spelling;
		wa.phonetic = phonetic;
	}
	
	if (start_next) {
		disabled = false;
		GetSyllables(batch_i+1, true);
	}
}

void SongDataWords::GetDetails(int batch_i, bool start_next) {
	if (Thread::IsShutdownThreads())
		return;
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	int begin = batch_i * per_batch;
	int end = (batch_i+1) * per_batch;
	
	if (batch_i < 0) {
		begin = 0;
		end = 1;
	}
	
	tmp_ds_i.Clear();
	tmp_words.Clear();
	
	SongDataAnalysisArgs args;
	
	int iter = 0;
	for(int ds_i = 0; ds_i < sd.GetCount(); ds_i++) {
		String ds_key = sd.GetKey(ds_i);
		DatasetAnalysis& ds = sda.datasets.GetAdd(ds_key);
		
		for(int i = 0; i < ds.words.GetCount(); i++) {
			auto& wa = ds.words[i];
			if (wa.translation.GetCount())
				continue;
			String wrd = ds.words[i].txt;
			
			// Skip cyrillic words
			bool is_cyrillic = false;
			WString ws = wrd.ToWString();
			for(int j = 0; j < ws.GetCount(); j++) {
				int chr = ws[j];
				if (chr >= 0x0400 && chr < 0x052F) {
					is_cyrillic = true;
					break;
				}
			}
			if (is_cyrillic)
				continue;
			
			if (iter >= begin) {
				args.words << wrd;
				tmp_ds_i << ds_i;
				tmp_words.Add(wrd);
			}
			iter++;
			if (iter >= end) break;
		}
		if (iter >= end) break;
	}
	
	if (args.words.IsEmpty()) {
		if (batch)
			GetSyllables(0, true);
		return;
	}
	
	Song& song = GetSong();
	song.RealizePipe();
	TaskMgr& pipe = *song.pipe;
	TaskMgr& m = pipe;
	
	args.fn = 5;
	
	m.GetSongDataAnalysis(args, THISBACK2(OnDetails, batch_i, start_next));
}

void SongDataWords::OnDetails(String res, int batch_i, bool start_next) {
	if (Thread::IsShutdownThreads())
		return;
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	
	res.Replace("\r", "");
	
	Vector<String> lines = Split(res, "\n");
	//DUMPC(lines);
	
	for(int i = 0; i < lines.GetCount(); i++) {
		String& l = lines[i];
		
		RemoveLineChar(l);
		
		int a = l.Find(":");
		if (a < 0) continue;
		String orig = l.Left(a);
		l = TrimBoth(l.Mid(a+1));
		
		a = l.Find(" or ");
		if (a > 0) {
			l = TrimBoth(l.Mid(a+4));
		}
		
		
		//orig = ToLower(orig.ToWString()).ToString();
		//orig.Replace("'", "");
		
		
		
		a = l.Find(",");
		if (a < 0) continue;
		String main_class = TrimBoth(l.Left(a));
		l = TrimBoth(l.Mid(a+1));
		
		a = l.Find("),");
		if (a < 0) continue;
		String rgb = TrimBoth(l.Left(a));
		l = TrimBoth(l.Mid(a+2));
		a = rgb.Find("(");
		if (a < 0) continue;
		rgb = TrimBoth(rgb.Mid(a+1));
		
		String translation = TrimBoth(l);
		
		int j = tmp_words.Find(orig);
		if (j < 0) continue;
		int ds_i = tmp_ds_i[j];
		DatasetAnalysis& ds = sda.datasets[ds_i];
		
		j = ds.FindWord(orig);
		if (j < 0) continue;
		
		WordAnalysis& wa = ds.words[j];
		wa.main_class = main_class;
		wa.translation = translation;
		
		Vector<String> clr_str = Split(rgb, ",");
		if (clr_str.GetCount() == 3) {
			wa.clr = Color(
				ScanInt(TrimBoth(clr_str[0])),
				ScanInt(TrimBoth(clr_str[1])),
				ScanInt(TrimBoth(clr_str[2])));
		}
	}
	
	
	
	if (start_next) {
		disabled = false;
		GetDetails(batch_i+1, true);
	}
}

void SongDataWords::DumpWordGroups() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	Index<String> main_classes;
	for(int ds_i = 0; ds_i < sd.GetCount(); ds_i++) {
		String ds_key = sd.GetKey(ds_i);
		DatasetAnalysis& ds = sda.datasets.GetAdd(ds_key);
		
		for(int i = 0; i < ds.words.GetCount(); i++) {
			const WordAnalysis& wa = ds.words[i];
			main_classes.FindAdd(wa.main_class);
		}
	}
	
	for(int i = 0; i < main_classes.GetCount(); i++) {
		LOG("- " << main_classes[i]);
	}
}

void SongDataWords::DumpPhoneticChars() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	Index<WString> chars;
	for(int ds_i = 0; ds_i < sd.GetCount(); ds_i++) {
		String ds_key = sd.GetKey(ds_i);
		DatasetAnalysis& ds = sda.datasets.GetAdd(ds_key);
		
		for(int i = 0; i < ds.words.GetCount(); i++) {
			const WordAnalysis& wa = ds.words[i];
			for(int j = 0; j < wa.phonetic.GetCount(); j++) {
				chars.FindAdd(wa.phonetic.Mid(j,1));
			}
		}
	}
	
	for(int i = 0; i < chars.GetCount(); i++) {
		const WString& ws = chars[i];
		if (ws.IsEmpty()) continue;
		int c0 = ws[0];
		int c1 = ws.GetCount() > 1 ? ws[1] : 0;
		int e = GetPhonemeEnum(c0, c1, 0);
		if (e >= 0)
			continue;
		LOG("PHONOME_ALT(\"" << ws << "\", \"\") \\");
	}
}
