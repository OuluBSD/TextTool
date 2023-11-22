#include "SimplifiedCtrl.h"


SongDataLineActions::SongDataLineActions() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << vsplit << vsplit1;
	hsplit.SetPos(2000);
	
	vsplit.Vert() << datasets << actions << action_args;
	vsplit.SetPos(1000,0);
	
	vsplit1.Vert() << phrases;
	
	datasets.AddColumn(t_("Dataset"));
	datasets.WhenCursor << THISBACK(DataDataset);
	
	actions.AddColumn(t_("Action"));
	actions.AddColumn(t_("Count"));
	actions.ColumnWidths("3 1");
	actions.WhenCursor << THISBACK(DataAction);
	
	action_args.AddColumn(t_("Action args"));
	action_args.AddColumn(t_("Count"));
	action_args.ColumnWidths("3 1");
	action_args.WhenCursor << THISBACK(DataActionArg);
	
	phrases.AddColumn(t_("Txt"));
	String cw = "2";
	for(int i = 0; i < action_cols; i++) {
		cw << " 1";
		phrases.AddColumn("#" + IntStr(i));
	}
	phrases.AddIndex("IDX");
	phrases.ColumnWidths(cw);
	
	/*clr_wordnets.AddColumn(t_("Main class"));
	clr_wordnets.AddColumn(t_("Anchor word"));
	clr_wordnets.AddColumn(t_("#1 alternative"));
	clr_wordnets.AddColumn(t_("#2 alternative"));
	clr_wordnets.AddColumn(t_("#3 alternative"));
	clr_wordnets.AddColumn(t_("#4 alternative"));
	clr_wordnets.AddColumn(t_("#5 alternative"));
	clr_wordnets.AddColumn(t_("#6 alternative"));
	clr_wordnets.AddColumn(t_("#7 alternative"));
	clr_wordnets.AddColumn(t_("Total score"));
	clr_wordnets.AddColumn(t_("Idea score (likes)"));
	clr_wordnets.AddColumn(t_("Emotion score (comments)"));
	clr_wordnets.AddColumn(t_("Hook score (listens)"));
	clr_wordnets.AddColumn(t_("Share score (relatability)"));
	clr_wordnets.AddColumn(t_("Value score (bookmarks)"));
	clr_wordnets.AddIndex("IDX");
	clr_wordnets.ColumnWidths("2 2 1 1 1 1 1 1 1 2 2 2 2 2 2");
	*/
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
	
	DataActionArg();
}

void SongDataLineActions::DataActionArg() {
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
	
	int row = 0;
	for (const ActionPhrase& ap : da.action_phrases) {
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
		
		this->phrases.Set(row, 0, ap.txt);
		int c = min(action_cols, ap.actions.GetCount());
		for(int j = 0; j < c; j++) {
			const auto& aa = ap.actions[j];
			String s = aa.action + "("+ aa.arg + ")";
			this->phrases.Set(row, 1+j, s);
		}
		for(int j = c; j < action_cols; j++)
			this->phrases.Set(row, 1+j, Value());
		
		row++;
	}
	this->phrases.SetCount(row);
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
	bar.Separator();
	if (running1)
		bar.Add(t_("Stop getting colored wordnet scores"), AppImg::RedRing(), THISBACK(ToggleGettingColorWordnetScores)).Key(K_F7);
	else
		bar.Add(t_("Start getting colored wordnet scores"), AppImg::RedRing(), THISBACK(ToggleGettingColorWordnetScores)).Key(K_F7);
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
				for(int k = 0; k < lines.GetCount(); k++) {
					String l = TrimBoth(lines[k]);
					hash_t h = l.GetHashValue();
					if (VectorFind<hash_t>(line_hashes, h) >= 0)
						continue;
					line_hashes << h;
					added_lines << l;
					if (added_lines.GetCount() >= per_batch) {
						Batch& b = batches.Add();
						b.artist = &artist;
						b.lyrics = &lyrics;
						b.ds_i = ds_i;
						b.txt = Join(added_lines, "\n");
						ASSERT(b.txt.GetCount());
						added_lines.SetCount(0);
					}
				}
				if (added_lines.GetCount()) {
					Batch& b = batches.Add();
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
		if (1) {
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
	Vector<ActionArg> actions;
	
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
				
				ActionArg& aa = actions.Add();
				aa.action = action;
				aa.arg = arg;
				
			}
			Sort(actions, ActionArg());
			for (ActionArg& aa : actions) {
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
				}
				i++;
			}
			if (!found) {
				ap_is << da.action_phrases.GetCount();
				ActionPhrase& ap = da.action_phrases.Add();
				ap.hash = h;
				ap.txt = txt;
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

void SongDataLineActions::ToggleGettingColorWordnetScores() {
	running1 = !running1;
	if (running1) {
		Thread::Start(THISBACK1(GetColorWordnetScores, 0));
	}
}

void SongDataLineActions::GetColorWordnetScores(int batch_i) {
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
	
	/*tmp_clr_wordnets.Clear();
	
	int iter = 0;
	for(int ds_i = 0; ds_i < sd.GetCount(); ds_i++) {
		String ds_key = sd.GetKey(ds_i);
		DatasetAnalysis& ds = sda.datasets.GetAdd(ds_key);
		
		for(int i = 0; i < ds.clr_wordnets.GetCount(); i++) {
			ColorWordnet& wn = ds.clr_wordnets[i];
			
			if (iter >= begin) {
				String key = wn.main_class + ": ";
				int c = min(7, wn.words.GetCount());
				for(int j = 0; j < c; j++) {
					if (j) key << ", ";
					key << wn.words[j];
				}
				
				args.words << key;
				tmp_clr_wordnets << &wn;
			}
			iter++;
			if (iter >=  end) break;
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
	Pipe& pipe = *song.pipe;
	TaskMgr& m = pipe;
	
	args.fn = 9;
	
	m.GetSongDataAnalysis(args, THISBACK1(OnColorWordnetScores, batch_i));*/
}

void SongDataLineActions::OnColorWordnetScores(String res, int batch_i) {
	if (Thread::IsShutdownThreads())
		return;
	
	/*Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	res.Replace("\r", "");
	Vector<String> lines = Split(res, "\n");
	for(int i = 0; i < lines.GetCount(); i++) {
		String& l = lines[i];
		l = TrimBoth(l);
		if (l.IsEmpty() || l[0] == '-')
			lines.Remove(i--);
	}
	
	if (lines.GetCount() == per_batch) {
		lines.Remove(0);
		VectorMap<int,Vector<String>> iter_phrase_parts;
		VectorMap<int,Vector<Vector<String>>> iter_phrase_words;
		int line_idx = -1;
		for (String& l : lines) {
			line_idx++;
			l = TrimBoth(l);
			if (l.IsEmpty())
				continue;
			
			int a = l.Find(".");
			if (a < 0) continue;
			a++;
			
			Vector<String> parts = Split(l.Mid(a), ",");
			if (parts.GetCount() != SCORE_COUNT)
				continue;
			int score[SCORE_COUNT] = {0,0,0,0,0};
			for(int i = 0; i < parts.GetCount(); i++) {
				String& s = parts[i];
				int a = s.Find(":");
				if (a < 0) continue;
				score[i] = StrInt(TrimBoth(s.Mid(a+1)));
			}
			
			ColorWordnet& wn = *tmp_clr_wordnets[line_idx];
			for(int i = 0; i < SCORE_COUNT; i++)
				wn.scores[i] = score[i];
		}
	}*/
	
	if (running1)
		PostCallback(THISBACK1(GetColorWordnetScores, batch_i+1));
}
