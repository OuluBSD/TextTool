#include "SongDataCtrl.h"


SongDataLineActions::SongDataLineActions() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << vsplit << vsplit1;
	hsplit.SetPos(2000);
	
	vsplit.Vert() << datasets << actions << action_args;
	vsplit.SetPos(1000,0);
	
	vsplit1.Vert() << phrases << next_lines;
	vsplit1.SetPos(8000);
	
	datasets.AddColumn(t_("Dataset"));
	datasets.WhenCursor << THISBACK(DataDataset);
	
	actions.AddColumn(t_("Action"));
	actions.AddColumn(t_("Count"));
	actions.ColumnWidths("3 1");
	actions.WhenCursor << THISBACK(DataAction);
	
	action_args.AddColumn(t_("Action args"));
	action_args.AddColumn(t_("Count"));
	action_args.ColumnWidths("3 1");
	action_args.WhenCursor << THISBACK(DataActionHeader);
	
	phrases.AddColumn(t_("Txt"));
	String cw = "2";
	for(int i = 0; i < action_cols; i++) {
		cw << " 1";
		phrases.AddColumn("#" + IntStr(i));
	}
	cw << " 1";
	phrases.AddColumn(t_("First line count"));
	phrases.AddIndex("IDX");
	phrases.ColumnWidths(cw);
	phrases.WhenCursor << THISBACK(DataNextLine);
	
	next_lines.AddColumn(t_("Txt"));
	next_lines.AddColumn(t_("Total score"));
	String sc;
	for(int i = 0; i < SCORE_MODE_COUNT; i++) {
		for(int j = 0; j < SCORE_ATTR_COUNT; j++) {
			next_lines.AddColumn(ScoreTitles[i][j]);
			sc << " 1";
		}
	}
	next_lines.ColumnWidths("3 1" + sc);
	
}

void SongDataLineActions::EnableAll() {
	
}

void SongDataLineActions::DisableAll() {
	
}

void SongDataLineActions::Data() {
	
}

void SongDataLineActions::DataMain() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	
	for(int i = 0; i < sda.datasets.GetCount(); i++) {
		datasets.Set(i, 0, sda.datasets.GetKey(i));
	}
	datasets.SetCount(sda.datasets.GetCount());
	if (!datasets.IsCursor() || datasets.GetCount())
		datasets.SetCursor(0);
	
	DataDataset();
}
	
void SongDataLineActions::DataDataset() {
	if (!datasets.IsCursor())
		return;
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	int ds_i = datasets.GetCursor();
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	uniq_acts.Clear();
	for (const ActionPhrase& ap : da.action_phrases) {
		for (const auto& a : ap.actions)
			uniq_acts.GetAdd(a.action).GetAdd(a.arg, 0)++;
	}
	if (0) {
		SortByKey(uniq_acts, StdLess<String>());
		for (auto& v : uniq_acts.GetValues())
			SortByKey(v, StdLess<String>());
	}
	else {
		struct Sorter {
			bool operator()(const VectorMap<String, int>& a, const VectorMap<String, int>& b) const {
				return a.GetCount() > b.GetCount();
			}
		};
		SortByValue(uniq_acts, Sorter());
		for (auto& v : uniq_acts.GetValues())
			SortByValue(v, StdGreater<int>());
	}
	
	actions.Set(0, 0, "All");
	actions.Set(0, 1, da.action_phrases.GetCount());
	for(int i = 0; i < uniq_acts.GetCount(); i++) {
		actions.Set(1+i, 0, uniq_acts.GetKey(i));
		actions.Set(1+i, 1, uniq_acts[i].GetCount());
	}
	actions.SetCount(1+uniq_acts.GetCount());
	if (!actions.IsCursor() && actions.GetCount())
		actions.SetCursor(0);
	
	DataAction();
}

void SongDataLineActions::DataAction() {
	if (!datasets.IsCursor() || !actions.IsCursor())
		return;
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	int ds_i = datasets.GetCursor();
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	
	String action = actions.Get(0);
	int i = uniq_acts.Find(action);
	if (i < 0) {
		action_args.SetCount(1);
		action_args.Set(0, 0, "All");
		action_args.Set(0, 1, da.action_phrases.GetCount());
	}
	else {
		auto& args = uniq_acts[i];
		action_args.Set(0, 0, "All");
		action_args.Set(0, 1, args.GetCount());
		for(int i = 0; i < args.GetCount(); i++) {
			action_args.Set(1+i, 0, args.GetKey(i));
			action_args.Set(1+i, 1, args[i]);
		}
		action_args.SetCount(1+args.GetCount());
	}
	if (!action_args.IsCursor() && action_args.GetCount())
		action_args.SetCursor(0);
	
	DataActionHeader();
}

void SongDataLineActions::DataActionHeader() {
	if (!datasets.IsCursor() || !actions.IsCursor() || !action_args.IsCursor())
		return;
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	int ds_i = datasets.GetCursor();
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	String action = actions.Get(0);
	String action_arg = action_args.Get(0);
	bool filter_action = action != "All";
	bool filter_action_arg = action_arg != "All";
	
	int idx = -1;
	int row = 0;
	for (const ActionPhrase& ap : da.action_phrases) {
		idx++;
		if (filter_action) {
			bool found = false;
			for (const auto& a : ap.actions) {
				if (a.action == action) {
					found = true;
					break;
				}
			}
			if (!found) continue;
		}
		if (filter_action_arg) {
			bool found = false;
			for (const auto& a : ap.actions) {
				if (a.arg == action_arg) {
					found = true;
					break;
				}
			}
			if (!found) continue;
		}
		
		phrases.Set(row, 0, ap.txt);
		int c = min(action_cols, ap.actions.GetCount());
		for(int j = 0; j < c; j++) {
			const auto& aa = ap.actions[j];
			String s = aa.action + "("+ aa.arg + ")";
			phrases.Set(row, 1+j, s);
		}
		for(int j = c; j < action_cols; j++)
			phrases.Set(row, 1+j, Value());
		
		if (ap.first_lines > 0)
			phrases.Set(row, 1+action_cols, ap.first_lines);
		else
			phrases.Set(row, 1+action_cols, Value());
		
		phrases.Set(row, "IDX", idx);
		row++;
	}
	phrases.SetCount(row);
	phrases.SetSortColumn(1+action_cols, true);
	if (!phrases.IsCursor() && phrases.GetCount())
		phrases.SetCursor(0);
	
	DataNextLine();
}

void SongDataLineActions::DataNextLine() {
	if (!datasets.IsCursor() || !actions.IsCursor() || !action_args.IsCursor() || !phrases.IsCursor()) {
		next_lines.Clear();
		return;
	}
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	int ds_i = datasets.GetCursor();
	DatasetAnalysis& da = sda.datasets[ds_i];
	int phrase_i = phrases.Get("IDX");
	const ActionPhrase& ap = da.action_phrases[phrase_i];
	
	for(int i = 0; i < ap.next_phrases.GetCount(); i++) {
		int next_i = ap.next_phrases[i];
		const ActionPhrase& next = da.action_phrases[next_i];
		next_lines.Set(i, 0, next.txt);
		if (i < next.next_scores.GetCount()) {
			auto& sc = next.next_scores[i];
			int total_score = 0, l = 0;
			for(int j = 0; j < SCORE_MODE_COUNT; j++) {
				for(int k = 0; k < SCORE_ATTR_COUNT; k++) {
					int v = sc.scores[j][k];
					total_score += v;
					next_lines.Set(i, 2+l, v);
					l++;
				}
			}
			next_lines.Set(i, 1, total_score);
		}
		else {
			next_lines.Set(i, 1, Value());
			for(int j = 0; j < (SCORE_MODE_COUNT * SCORE_ATTR_COUNT); j++) {
				next_lines.Set(i, 2+j, Value());
			}
		}
	}
	next_lines.SetCount(ap.next_phrases.GetCount());
	
}

void SongDataLineActions::ToolMenu(Bar& bar) {
	bar.Add(t_("Update data"), AppImg::BlueRing(), THISBACK(DataMain)).Key(K_CTRL_Q);
	bar.Separator();
	bar.Add(t_("Update batches"), AppImg::BlueRing(), THISBACK(UpdateBatches)).Key(K_F5);
	bar.Separator();
	if (running0)
		bar.Add(t_("Stop getting line actions"), AppImg::RedRing(), THISBACK(ToggleGettingLineActions)).Key(K_F6);
	else
		bar.Add(t_("Start getting line actions"), AppImg::RedRing(), THISBACK(ToggleGettingLineActions)).Key(K_F6);
	for(int i = 0; i < SCORE_MODE_COUNT; i++) {
		bar.Separator();
		
		if (running1)
			bar.Add(t_("Stop getting colored wordnet scores") + Format(" %d",i), AppImg::RedRing(), THISBACK1(ToggleGettingLineChangeScores,i)).Key(K_F7 + i);
		else
			bar.Add(t_("Start getting colored wordnet scores") + Format(" %d",i), AppImg::RedRing(), THISBACK1(ToggleGettingLineChangeScores,i)).Key(K_F7 + i);
	}
}

void SongDataLineActions::UpdateBatches() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	
	batches.SetCount(0);
	batches.Reserve(1000);
	
	Vector<hash_t> line_hashes;
	Vector<String> added_lines;
	
	for (int ds_i = 0; ds_i < sd.GetCount(); ds_i++) {
		Vector<ArtistDataset>& artists = sd[ds_i];
		for(int i = 0; i < artists.GetCount(); i++) {
			ArtistDataset& artist = artists[i];
			for(int j = 0; j < artist.lyrics.GetCount(); j++) {
				LyricsDataset& lyrics = artist.lyrics[j];
				Vector<String> lines = Split(lyrics.text, "\n");
				for(int k = 0; k < lines.GetCount(); k++) {
					String& l = lines[k];
					l = TrimBoth(l);
					if (l.IsEmpty()) lines.Remove(k--);
				}
				if (lines.IsEmpty()) continue;
				added_lines.SetCount(0);
				line_hashes.SetCount(0);
				bool song_begins = true;
				for(int k = 0; k < lines.GetCount(); k++) {
					String l = TrimBoth(lines[k]);
					hash_t h = l.GetHashValue();
					if (VectorFind<hash_t>(line_hashes, h) >= 0)
						continue;
					line_hashes << h;
					added_lines << l;
					if (added_lines.GetCount() >= per_batch) {
						Batch& b = batches.Add();
						b.song_begins = song_begins;
						b.artist = &artist;
						b.lyrics = &lyrics;
						b.ds_i = ds_i;
						b.txt = Join(added_lines, "\n");
						ASSERT(b.txt.GetCount());
						added_lines.SetCount(0);
						song_begins = false;
					}
				}
				if (added_lines.GetCount()) {
					Batch& b = batches.Add();
					b.song_begins = song_begins;
					b.artist = &artist;
					b.lyrics = &lyrics;
					b.ds_i = ds_i;
					b.txt = Join(added_lines, "\n");
					ASSERT(b.txt.GetCount());
				}
			}
		}
	}
}

void SongDataLineActions::ToggleGettingLineActions() {
	running0 = !running0;
	if (running0) {
		if (0) {
			Database& db = Database::Single();
			SongData& sd = db.song_data;
			SongDataAnalysis& sda = db.song_data.a;
			for (DatasetAnalysis& da : sda.datasets) {
				da.action_phrases.Clear();
			}
		}
		UpdateBatches();
		Thread::Start(THISBACK1(GetLineActions, 0));
	}
}

void SongDataLineActions::GetLineActions(int batch_i) {
	if (Thread::IsShutdownThreads())
		return;
	if (batches.IsEmpty()) UpdateBatches();
	if (batch_i < 0 || batch_i >= batches.GetCount()) {
		this->batch = false;
		return;
	}
	Batch& batch = batches[batch_i];
	tmp_batch_i = batch_i;
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	SongDataAnalysisArgs args;
	args.fn = 10;
	args.phrases <<= Split(batch.txt, "\n");
	
	Song& song = GetSong();
	song.RealizePipe();
	TaskMgr& m = *song.pipe;
	m.GetSongDataAnalysis(args, THISBACK1(OnLineActions, batch_i));
}

void SongDataLineActions::OnLineActions(String res, int batch_i) {
	if (Thread::IsShutdownThreads())
		return;
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	Batch& batch = batches[tmp_batch_i];
	DatasetAnalysis& da = sda.datasets[batch.ds_i];
	
	res.Replace("\r", "");
	Vector<String> lines = Split(res, "\n");
	for(int i = 0; i < lines.GetCount(); i++) {
		String& l = lines[i];
		RemoveLineNumber(l);
		l = TrimBoth(l);
		if (l.IsEmpty() || l[0] == '-')
			lines.Remove(i--);
	}
	
	Vector<String> txt_lines = Split(batch.txt, "\n");
	
	// e.g. tone(desperate) + msg(distracting oneself) + bias(impulsiveness)
	Vector<ActionHeader> actions;
	
	if (lines.GetCount() == txt_lines.GetCount()) {
		int line_idx = -1;
		Vector<int> ap_is;
		for (String& l : lines) {
			line_idx++;
			l = TrimBoth(l);
			if (l.IsEmpty())
				continue;
			
			const String& txt = txt_lines[line_idx];
			actions.SetCount(0);
			
			Vector<String> parts = Split(l, "+");
			CombineHash ch;
			for(int i = 0; i < parts.GetCount(); i++) {
				String& s = parts[i];
				s = TrimBoth(s);
				int a = s.Find("(");
				int b = s.Find(")");
				if (a < 0 || b < 0 || a > b) {
					parts.Remove(i--);
					continue;
				}
				String action = TrimBoth(s.Left(a));
				a++;
				String arg = TrimBoth(s.Mid(a,b-a));
				
				ActionHeader& aa = actions.Add();
				aa.action = action;
				aa.arg = arg;
				
			}
			Sort(actions, ActionHeader());
			for (ActionHeader& aa : actions) {
				ch.Do(aa.action);
				ch.Do(aa.arg);
			}
			hash_t h = ch;
			
			bool found = false;
			int i = 0;
			for (ActionPhrase& ap : da.action_phrases) {
				if (ap.hash == h) {
					found = true;
					ap_is << i;
					if (line_idx == 0 && batch.song_begins)
						ap.first_lines++;
				}
				i++;
			}
			if (!found) {
				ap_is << da.action_phrases.GetCount();
				ActionPhrase& ap = da.action_phrases.Add();
				ap.hash = h;
				ap.txt = txt;
				ap.first_lines = line_idx == 0 && batch.song_begins ? 1 : 0;
				Swap(ap.actions, actions);
			}
		}
		for(int i = 1; i < ap_is.GetCount(); i++) {
			int ap_i0 = ap_is[i-1];
			int ap_i1 = ap_is[i];
			ActionPhrase& ap0 = da.action_phrases[ap_i0];
			VectorFindAdd(ap0.next_phrases, ap_i1);
		}
	}
	
	if (running0)
		PostCallback(THISBACK1(GetLineActions, batch_i+1));
}

void SongDataLineActions::ToggleGettingLineChangeScores(int score_mode) {
	running1 = !running1;
	if (running1) {
		Thread::Start(THISBACK2(GetLineChangeScores, 0, score_mode));
	}
}

void SongDataLineActions::GetLineChangeScores(int batch_i, int score_mode) {
	if (Thread::IsShutdownThreads())
		return;
	if (batches.IsEmpty()) UpdateBatches();
	if (batch_i < 0 || batch_i >= batches.GetCount()) {
		this->batch = false;
		return;
	}
	Batch& batch = batches[batch_i];
	tmp_batch_i = batch_i;
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[batch.ds_i];
	
	SongDataAnalysisArgs args;
	args.fn = 11;
	args.score_mode = score_mode;
	args.phrases <<= Split(batch.txt, "\n");
	
	ap_is.Clear();
	
	bool fail = false;
	for (String& l : args.phrases) {
		int ap_i = 0;
		bool found = false;
		for (ActionPhrase& ap : da.action_phrases) {
			if (ap.txt == l) {
				l.Clear();
				for (const auto& arg : ap.actions) {
					if (!l.IsEmpty()) l << " + ";
					l << arg.action << "(" << arg.arg << ")";
				}
				found = true;
				break;
			}
			ap_i++;
		}
		ap_is << (found ? ap_i : -1);
		if (!found)
			fail = true;
	}
	
	/*if (fail) {
		PromptOK("Failed to finish line change scores");
		return;
	}*/
	
	Song& song = GetSong();
	song.RealizePipe();
	TaskMgr& m = *song.pipe;
	m.GetSongDataAnalysis(args, THISBACK2(OnLineChangeScores, batch_i, score_mode));
}

void SongDataLineActions::OnLineChangeScores(String res, int batch_i, int score_mode) {
	if (Thread::IsShutdownThreads())
		return;
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	Batch& batch = batches[tmp_batch_i];
	DatasetAnalysis& da = sda.datasets[batch.ds_i];
	
	res.Replace("\r", "");
	res = "1. Stop line 1 & start line 2: S0:" + res;
	res.Replace("not applicable", "0");
	
	Vector<String> lines = Split(res, "\n");
	for(int i = 0; i < lines.GetCount(); i++) {
		String& l = lines[i];
		RemoveLineNumber(l);
		l = TrimBoth(l);
		if (l.IsEmpty() || l[0] == '-')
			lines.Remove(i--);
	}
	
	for (String& l : lines) {
		int a = l.Find("line");
		if (a < 0) continue;
		int b = l.Find("&", a);
		if (b < 0) continue;
		a += 4;
		int line0 = StrInt(TrimBoth(l.Mid(a,b-a)));
		a = l.Find("line",b);
		if (a < 0) continue;
		b = l.Find(":", a);
		if (b < 0) continue;
		a += 4;
		int line1 = StrInt(TrimBoth(l.Mid(a,b-a)));
		
		Vector<String> parts = Split(l.Mid(b+1), ",");
		if (parts.GetCount() != SCORE_ATTR_COUNT)
			continue;
		int score[SCORE_ATTR_COUNT] = {0,0,0,0,0};
		for(int i = 0; i < parts.GetCount(); i++) {
			String& s = parts[i];
			int a = s.Find(":");
			if (a < 0) continue;
			score[i] = StrInt(TrimBoth(s.Mid(a+1)));
		}
		
		line0--;
		line1--;
		if (line0 >= ap_is.GetCount() || line1 >= ap_is.GetCount()) continue;
		
		int ap_i0 = ap_is[line0];
		int ap_i1 = ap_is[line1];
		if (ap_i0 < 0 || ap_i1 < 0)
			continue;
		
		ActionPhrase& ap0 = da.action_phrases[ap_i0];
		//ActionPhrase& ap1 = da.action_phrases[ap_i1];
		int j = VectorFind(ap0.next_phrases, ap_i1);
		if (j >= 0) {
			if (ap0.next_scores.GetCount() <= j)
				ap0.next_scores.SetCount(j+1);
			auto& tgt = ap0.next_scores[j];
			for(int j = 0; j < SCORE_ATTR_COUNT; j++)
				tgt.scores[score_mode][j] = score[j];
		}
	}
	
	if (running1)
		PostCallback(THISBACK2(GetLineChangeScores, batch_i+1, score_mode));
}
