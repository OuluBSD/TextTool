#include "SongDataCtrl.h"

#if 0

SongDataAnalysisPage::SongDataAnalysisPage() {
	Add(hsplit.HSizePos().VSizePos(0,30));
	Add(prog.HSizePos().BottomPos(0,30));
	hsplit.Horz() << list << analysis;
	hsplit.SetPos(3333);
	
	list.AddColumn(t_("#"));
	list.AddColumn(t_("Task"));
	list.AddColumn(t_("Song"));
	list.AddColumn(t_("Txt-length"));
	list.AddColumn(t_("Ready"));
	list.ColumnWidths("1 2 3 1 1");
	list.WhenCursor << THISBACK(DataTask);
	
	prog.Set(0, 1);
}

void SongDataAnalysisPage::DisableAll() {
	disabled = true;
	list.Disable();
	analysis.Disable();
}

void SongDataAnalysisPage::EnableAll() {
	disabled = false;
	list.Enable();
	analysis.Enable();
}

void SongDataAnalysisPage::ToolMenu(Bar& bar) {
	//bar.Add(t_("Get rhymes"), AppImg::BlueRing(), THISBACK2(GetRhymes, -1, false)).Key(K_CTRL_Q);
	bar.Add(t_("Get words"), AppImg::BlueRing(), THISBACK2(GetWords, -1, false)).Key(K_CTRL_W);
	bar.Add(t_("Get attribute distributions"), AppImg::BlueRing(), THISBACK2(GetAttributeDistributions, -1, false)).Key(K_CTRL_E);
	//bar.Add(t_("Get rhyme locations"), AppImg::BlueRing(), THISBACK2(GetRhymeLocations, -1, false)).Key(K_CTRL_R);
	bar.Separator();
	//bar.Add(t_("Get all rhymes"), AppImg::RedRing(), THISBACK2(GetRhymes, 0, true)).Key(K_F5);
	bar.Add(t_("Get all words"), AppImg::RedRing(), THISBACK2(GetWords, 0, true)).Key(K_F6);
	bar.Add(t_("Get all attribute distributions"), AppImg::RedRing(), THISBACK2(GetAttributeDistributions, 0, true)).Key(K_F7);
	//bar.Add(t_("Get all rhyme locations"), AppImg::RedRing(), THISBACK2(GetRhymeLocations, 0, true)).Key(K_F8);
	bar.Separator();
	bar.Add(t_("Get everything!"), AppImg::RedRing(), THISBACK(StartBatch)).Key(K_F9);
	
}

void SongDataAnalysisPage::StartBatch() {
	if (disabled)
		return;
	batch = true;
	GetWords(0, true);
}

#if 0

void SongDataAnalysisPage::GetRhymes(int list_i, bool start_next) {
	if (disabled)
		return;
	if (list_i < 0) {
		if (!list.IsCursor())
			return;
		list_i = list.GetCursor();
	}
	
	if (list_i < 0 || list_i >= list.GetCount())
		return;
	
	Task& t = tasks[list_i];
	
	if (t.analysis && t.analysis->rhymes.GetCount()) {
		if (start_next) {
			disabled = false;
			if (list_i+1 < list.GetCount())
				PostCallback(THISBACK2(GetRhymes, list_i+1, true));
			else if (batch)
				GetWords(0, true);
		}
		return;
	}
	
	PostCallback(THISBACK(DisableAll));
	
	Song& song = GetSong();
	
	SongDataAnalysisArgs args;
	args.fn = 0;
	args.artist = t.artist;
	args.song = t.song;
	args.text = t.text;
	
	TaskMgr& m = TaskMgr::Single();
	m.GetSongDataAnalysis(args, THISBACK2(OnRhymes, list_i, start_next));
}

void SongDataAnalysisPage::OnRhymes(String res, int list_i, bool start_next) {
	PostCallback(THISBACK(EnableAll));
	
	Task& t = tasks[list_i];
	LyricsDataset& ld = *t.ld;
	LyricsAnalysis& anal = *t.analysis;
	
	anal.rhymes.Clear();
	res.Replace("\r", "");
	
	Vector<String> lines = Split(res, "\n");
	for (String& l : lines) {
		RemoveLineChar(l);
		int a = l.Find(":");
		if (a >= 0)
			l = l.Mid(a+1);
		a = l.Find("(");
		int score = 0;
		if (a >= 0) {
			int a1 = a+1;
			int b = l.Find(")", a1);
			if (b >= 0) {
				score = ScanInt(l.Mid(a1,b-a1));
			}
			l = TrimBoth(l.Left(a));
		}
		
		auto& r = anal.rhymes.Add();
		r.words = Split(l, "/");
		r.score = score;
	}
	
	PostCallback(THISBACK(DataTask));
	
	if (start_next) {
		disabled = false;
		if (list_i+1 < list.GetCount())
			GetRhymes(list_i+1, true);
		else if (batch)
			GetWords(0, true);
	}
}

#endif

void SongDataAnalysisPage::GetWords(int list_i, bool start_next) {
	if (disabled || Thread::IsShutdownThreads())
		return;
	bool forced = false;
	if (list_i < 0) {
		forced = true;
		if (!list.IsCursor())
			return;
		list_i = list.GetCursor();
	}
	
	if (list_i < 0 || list_i >= list.GetCount())
		return;
	
	Task& t = tasks[list_i];
	
	if (!forced && t.analysis && t.analysis->phrases.GetCount()) {
		if (start_next) {
			disabled = false;
			if (list_i+1 < list.GetCount())
				PostCallback(THISBACK2(GetWords, list_i+1, true));
			else if (batch)
				GetAttributeDistributions(0, true);
		}
		return;
	}
	
	PostCallback(THISBACK(DisableAll));
	
	Song& song = GetSong();
	
	SongDataAnalysisArgs args;
	args.fn = 1;
	args.artist = t.artist;
	args.song = t.song;
	args.text = t.text;
	
	TaskMgr& m = TaskMgr::Single();
	m.GetSongDataAnalysis(args, THISBACK2(OnWords, list_i, start_next));
}

void SongDataAnalysisPage::OnWords(String res, int list_i, bool start_next) {
	PostCallback(THISBACK(EnableAll));
	
	if (Thread::IsShutdownThreads())
		return;
	
	Task& t = tasks[list_i];
	LyricsDataset& ld = *t.ld;
	LyricsAnalysis& anal = *t.analysis;
	
	anal.phrases.Clear();
	res.Replace("\r", "");
	
	res = "-\"" + res;
	
	Vector<String> lines = Split(res, "\n");
	for (String& l : lines) {
		String group, value, phrase;
		
		RemoveLineChar(l);
		if (l.Left(1) != "\"") continue;
		int a = 1;
		int b = l.Find("\"", a);
		if (b < 0) continue;
		phrase = l.Mid(a, b-a);
		
		l = TrimBoth(l.Mid(b+1));
		
		a = l.Find(":");
		if (a < 0) continue;
		l = TrimBoth(l.Mid(a+1));
		
		a = l.Find(":");
		if (a < 0) continue;
		group = ToLower(TrimBoth(l.Left(a)));
		l = TrimBoth(l.Mid(a+1));
		
		a = l.Find(":");
		if (a < 0) {
			a = l.Find("(RGB");
			if (a < 0)
				continue;
		}
		value = ToLower(TrimBoth(l.Left(a)));
		l = TrimBoth(l.Mid(a+1));
		
		a = l.Find("RGB(");
		if (a < 0) continue;
		a += 4;
		b = l.Find(")", a);
		if (b < 0) continue;
		Vector<String> clr_parts = Split(l.Mid(a,b-a), ",");
		if (clr_parts.GetCount() != 3) continue;
		
		int R = ScanInt(TrimBoth(clr_parts[0]));
		int G = ScanInt(TrimBoth(clr_parts[1]));
		int B = ScanInt(TrimBoth(clr_parts[2]));
		Color clr(R,G,B);
		
		auto& p = anal.phrases.Add();
		p.phrase = phrase;
		p.group = group;
		p.value = value;
		p.clr = clr;
	}
	
	PostCallback(THISBACK(DataTask));
	
	if (start_next) {
		disabled = false;
		if (list_i+1 < list.GetCount())
			GetWords(list_i+1, true);
		else if (batch)
			GetAttributeDistributions(0, true);
	}
}

void SongDataAnalysisPage::GetAttributeDistributions(int list_i, bool start_next) {
	if (disabled || Thread::IsShutdownThreads())
		return;
	bool forced = false;
	if (list_i < 0) {
		forced = true;
		if (!list.IsCursor())
			return;
		list_i = list.GetCursor();
	}
	
	if (list_i < 0 || list_i >= list.GetCount())
		return;
	
	Task& t = tasks[list_i];
	
	if (!forced && t.analysis && t.analysis->positive_roles.GetCount()) {
		if (start_next) {
			disabled = false;
			if (list_i+1 < list.GetCount())
				PostCallback(THISBACK2(GetAttributeDistributions, list_i+1, true));
			//else if (batch)
			//	GetRhymeLocations(0, true);
			else batch = false;
		}
		return;
	}
	
	PostCallback(THISBACK(DisableAll));
	
	Song& song = GetSong();
	
	SongDataAnalysisArgs args;
	args.fn = 2;
	args.artist = t.artist;
	args.song = t.song;
	args.text = t.text;
	
	TaskMgr& m = TaskMgr::Single();
	m.GetSongDataAnalysis(args, THISBACK2(OnRoleDistributions, list_i, start_next));
}

void SongDataAnalysisPage::OnRoleDistributions(String res, int list_i, bool start_next) {
	PostCallback(THISBACK(EnableAll));
	
	if (Thread::IsShutdownThreads())
		return;
	
	Task& t = tasks[list_i];
	LyricsDataset& ld = *t.ld;
	LyricsAnalysis& anal = *t.analysis;
	
	anal.positive_roles.Clear();
	anal.negative_roles.Clear();
	res.Replace("\r", "");
	
	int a = res.Find("4 negative stereotypes");
	if (a >= 0) {
		String pos = TrimBoth(res.Left(a));
		String neg = TrimBoth(res.Mid(a));
		
		{
			Vector<String> lines = Split(pos, "\n");
			for (String& l : lines) {
				l = TrimBoth(l);
				if (l.IsEmpty()) continue;
				RemoveLineChar(l);
				Vector<String> g = Split(l, ",");
				auto& out = anal.positive_roles.Add();
				for (String& s : g) {
					s = TrimBoth(s);
					Vector<String> kv = Split(s, ":");
					if (kv.GetCount() == 2) {
						Vector<String> gv = Split(kv[0], "/");
						if (gv.GetCount() == 2) {
							String value0 = TrimBoth(kv[1]);
							auto& out0 = out.Add();
							out0.group = ToLower(TrimBoth(gv[0]));
							out0.value = ToLower(TrimBoth(gv[1]));
							out0.percent = ScanInt(value0);
						}
					}
				}
			}
		}
		{
			Vector<String> lines = Split(neg, "\n");
			if (lines.GetCount()) lines.Remove(0);
			for (String& l : lines) {
				l = TrimBoth(l);
				if (l.IsEmpty()) continue;
				RemoveLineChar(l);
				Vector<String> g = Split(l, ",");
				auto& out = anal.negative_roles.Add();
				for (String& s : g) {
					s = TrimBoth(s);
					Vector<String> kv = Split(s, ":");
					if (kv.GetCount() == 2) {
						Vector<String> gv = Split(kv[0], "/");
						if (gv.GetCount() == 2) {
							String value0 = TrimBoth(kv[1]);
							auto& out0 = out.Add();
							out0.group = ToLower(TrimBoth(gv[0]));
							out0.value = ToLower(TrimBoth(gv[1]));
							out0.percent = ScanInt(value0);
						}
					}
				}
			}
		}
	}
	
	PostCallback(THISBACK(DataTask));
	
	if (start_next) {
		disabled = false;
		if (list_i+1 < list.GetCount())
			GetAttributeDistributions(list_i+1, true);
		//else if (batch)
		//	GetRhymeLocations(0, true);
		else batch = false;
	}
}

#if 0

void SongDataAnalysisPage::GetRhymeLocations(int list_i, bool start_next) {
	if (disabled)
		return;
	if (list_i < 0) {
		if (!list.IsCursor())
			return;
		list_i = list.GetCursor();
	}
	
	if (list_i < 0 || list_i >= list.GetCount())
		return;
	
	Task& t = tasks[list_i];
	
	if (0) {
		if (t.analysis && t.analysis->rhyme_locations.GetCount()) {
			if (start_next) {
				disabled = false;
				if (list_i+1 < list.GetCount())
					PostCallback(THISBACK2(GetRhymeLocations, list_i+1, true));
				//else if (batch)
				//	; // pass
			}
			return;
		}
	}
	
	PostCallback(THISBACK(DisableAll));
	
	Song& song = GetSong();
	
	SongDataAnalysisArgs args;
	args.fn = 3;
	args.artist = t.artist;
	args.song = t.song;
	args.text = t.text;
	
	TaskMgr& m = TaskMgr::Single();
	m.GetSongDataAnalysis(args, THISBACK2(OnRhymeLocations, list_i, start_next));
}

void SongDataAnalysisPage::OnRhymeLocations(String res, int list_i, bool start_next) {
	PostCallback(THISBACK(EnableAll));
	
	Task& t = tasks[list_i];
	LyricsDataset& ld = *t.ld;
	LyricsAnalysis& anal = *t.analysis;
	
	anal.rhyme_locations.Clear();
	
	res.Replace("\r", "");
	
	//LOG(res);
	
	Vector<String> lines = Split(res, "\n");
	Vector<String> words;
	Vector<bool> rhyming;
	for (String& line : lines) {
		RemoveLineNumber(line);
		SplitParenthesisWords(line, words, rhyming);
		
		//DUMPC(words);
		//DUMPC(rhyming);
		
		if (words.GetCount()) {
			LyricsAnalysis::RhymeLocationLine& l = anal.rhyme_locations.Add();
			l.words <<= words;
			l.rhyming <<= rhyming;
		}
	}
	
	if (start_next) {
		disabled = false;
		if (list_i+1 < list.GetCount())
			GetRhymeLocations(list_i+1, true);
		else
			batch = false;
	}
}

#endif

void SongDataAnalysisPage::Data() {
	if (tasks.GetCount() == 0)
		UpdateTasks();
	
	list.SetCount(tasks.GetCount());
	for(int i = 0; i < tasks.GetCount(); i++) {
		Task& t = tasks[i];
		list.Set(i, 0, i);
		list.Set(i, 1, t.artist);
		list.Set(i, 2, t.song);
		list.Set(i, 3, t.text.GetCount());
	}
	
	DataTask();
}

void SongDataAnalysisPage::DataTask() {
	analysis.Clear();
	
	if (!list.IsCursor()) {
		return;
	}
	
	Task& t = tasks[list.GetCursor()];
	if (t.analysis) {
		String s = t.analysis->AsString();
		if (GetDefaultCharset() != CHARSET_UTF8)
			s = ToCharset(CHARSET_DEFAULT, s, CHARSET_UTF8);
		analysis.SetData(s);
	}
}

void SongDataAnalysisPage::UpdateTasks() {
	SongDatabase& db = SongDatabase::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	tasks.Clear();
	
	for (int ds_i = 0; ds_i < sd.GetCount(); ds_i++) {
		String ds_key = sd.GetKey(ds_i);
		DatasetAnalysis& ds = sda.datasets.GetAdd(ds_key);
		
		for(int i = 0; i < ds.artists.GetCount(); i++) {
			String artist_name = ds.artists.GetKey(i);
			auto& v = ds.artists[i].songs;
			for(int j = 0; j < v.GetCount(); j++) {
				LyricsAnalysis& song = v[j];
				bool found = false;
				
				if (!found) for(ArtistDataset& ad : sd.artists_en) {
					if (ad.name != artist_name) continue;
					for (LyricsDataset& ld : ad.lyrics) {
						if (ld.name != song.name) continue;
						if (ld.text.GetCount() > txt_limit) continue;
						Task& t = tasks.Add();
						t.ld = &ld;
						t.analysis = &song;
						t.artist = ad.name;
						t.song = ld.name;
						t.text = ld.text;
						found = true;
					}
				}
				
				if (!found) for(ArtistDataset& ad : sd.artists_fi) {
					if (ad.name != artist_name) continue;
					for (LyricsDataset& ld : ad.lyrics) {
						if (ld.name != song.name) continue;
						if (ld.text.GetCount() > txt_limit) continue;
						Task& t = tasks.Add();
						t.ld = &ld;
						t.analysis = &song;
						t.artist = ad.name;
						t.song = ld.name;
						t.text = ld.text;
						found = true;
					}
				}
			}
		}
	}
}

#endif
