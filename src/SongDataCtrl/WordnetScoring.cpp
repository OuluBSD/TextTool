#include "SongDataCtrl.h"


SongDataWordnetScoring::SongDataWordnetScoring() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << vsplit << vsplit1;
	hsplit.SetPos(2000);
	
	vsplit.Vert() << datasets << attrs << colors;
	vsplit.SetPos(1000,0);
	
	vsplit1.Vert() << wordnets << clr_wordnets;
	
	datasets.AddColumn(t_("Dataset"));
	datasets.WhenCursor << THISBACK(DataDataset);
	
	attrs.AddColumn(t_("Group"));
	attrs.AddColumn(t_("Value"));
	attrs.AddIndex("GROUP");
	attrs.AddIndex("VALUE");
	attrs.ColumnWidths("1 1");
	attrs.WhenCursor << THISBACK(DataAttribute);
	
	colors.AddColumn(t_("Colors"));
	colors.WhenCursor << THISBACK(DataColor);
	
	wordnets.AddColumn(t_("Group"));
	wordnets.AddColumn(t_("Value"));
	wordnets.AddColumn(t_("Main class"));
	wordnets.AddColumn(t_("Words"));
	wordnets.AddColumn(t_("Total score"));
	String sc;
	for(int i = 0; i < SCORE_MODE_COUNT; i++) {
		for(int j = 0; j < SCORE_ATTR_COUNT; j++) {
			wordnets.AddColumn(ScoreTitles[i][j]);
			sc << " 1";
		}
	}
	wordnets.AddIndex("IDX");
	wordnets.ColumnWidths("1 2 1 3 1" + sc);
	
	clr_wordnets.AddColumn(t_("Main class"));
	clr_wordnets.AddColumn(t_("Anchor word"));
	clr_wordnets.AddColumn(t_("#1 alternative"));
	clr_wordnets.AddColumn(t_("#2 alternative"));
	clr_wordnets.AddColumn(t_("#3 alternative"));
	clr_wordnets.AddColumn(t_("#4 alternative"));
	clr_wordnets.AddColumn(t_("#5 alternative"));
	clr_wordnets.AddColumn(t_("#6 alternative"));
	clr_wordnets.AddColumn(t_("#7 alternative"));
	clr_wordnets.AddColumn(t_("Total score"));
	sc.Clear();
	for(int i = 0; i < SCORE_MODE_COUNT; i++) {
		for(int j = 0; j < SCORE_ATTR_COUNT; j++) {
			clr_wordnets.AddColumn(ScoreTitles[i][j]);
			sc << " 2";
		}
	}
	clr_wordnets.AddIndex("IDX");
	clr_wordnets.ColumnWidths("2 2 1 1 1 1 1 1 1" + sc);
	
}

void SongDataWordnetScoring::EnableAll() {
	
}

void SongDataWordnetScoring::DisableAll() {
	
}

void SongDataWordnetScoring::Data() {
	
}

void SongDataWordnetScoring::DataMain() {
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
	
void SongDataWordnetScoring::DataDataset() {
	if (!datasets.IsCursor())
		return;
	
	
	int gi = 0;
	int i = 0;
	
	attrs.Set(i, 0, "All");
	attrs.Set(i, 1, "All");
	attrs.Set(i, "GROUP", -1);
	attrs.Set(i, "VALUE", -1);
	i++;
	
	#define ATTR_ITEM(e, g, i0, i1) \
		attrs.Set(i, 0, g); \
		attrs.Set(i, 1, i0); \
		attrs.Set(i, "GROUP", gi); \
		attrs.Set(i, "VALUE", 0); \
		i++; \
		attrs.Set(i, 0, g); \
		attrs.Set(i, 1, i1); \
		attrs.Set(i, "GROUP", gi); \
		attrs.Set(i, "VALUE", 1); \
		i++, gi++;
	ATTR_LIST
	#undef ATTR_ITEM

	if (!attrs.IsCursor() && attrs.GetCount())
		attrs.SetCursor(0);
	
	
	DataAttribute();
}

void SongDataWordnetScoring::DataAttribute() {
	if (!attrs.IsCursor())
		return;
	
	
	
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

void SongDataWordnetScoring::DataColor() {
	if (!datasets.IsCursor() || !colors.IsCursor() || !attrs.IsCursor())
		return;
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	int ds_i = datasets.GetCursor();
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	int clr_i = colors.GetCursor();
	int attr_group_i = attrs.Get("GROUP");
	int attr_value_i = attrs.Get("VALUE");
	String group_str = attr_group_i >= 0 ? ToLower(Attr::AttrKeys[attr_group_i][1]) : String();
	String value_str = attr_group_i >= 0 ? ToLower(Attr::AttrKeys[attr_group_i][2 + attr_value_i]) : String();
	
	bool clr_filter = clr_i > 0;
	bool attr_filter = attr_group_i >= 0;
	clr_i--;
	
	int row = 0;
	for(int i = 0; i < da.wordnets.GetCount(); i++) {
		Wordnet& wn = da.wordnets[i];
		
		// Filter by color group
		if (clr_filter && wn.clr_group != clr_i)
			continue;
		
		// Filter by attribute
		if (attr_filter && (wn.group != group_str || wn.value != value_str))
			continue;
		
		wordnets.Set(row, "IDX", i);
		
		wordnets.Set(row, 0, wn.group);
		wordnets.Set(row, 1, wn.value);
		
		// Colored main class
		bool has_main_class_clr = false;
		Color main_class_clr;
		{
			int j = GetWordgroupColors().Find(wn.main_class);
			if (j >= 0) {
				main_class_clr = GetWordgroupColors()[j];
				has_main_class_clr = true;
			}
		}
		if (has_main_class_clr) {
			wordnets.Set(row, 2,
				AttrText(wn.main_class)
					.NormalPaper(Blend(main_class_clr, White(), 128+64)).NormalInk(Black())
					.Paper(Blend(main_class_clr, GrayColor())).Ink(White())
			);
		}
		else {
			wordnets.Set(row, 2, wn.main_class);
		}
		
		
		// All words
		if (wn.words.GetCount()) {
			wordnets.Set(row, 3,
				AttrText(Join(wn.words, ", "))
					.NormalPaper(Blend(wn.clr, White(), 128+64)).NormalInk(Black())
					.Paper(Blend(GrayColor(), wn.clr)).Ink(White()));
		}
		
		int total_score = 0, l = 0;
		for(int j = 0; j < SCORE_MODE_COUNT; j++) {
			for(int k = 0; k < SCORE_ATTR_COUNT; k++) {
				total_score += wn.scores[j][k];
				wordnets.Set(row, 5+l, wn.scores[j][k]);
				l++;
			}
		}
		wordnets.Set(row, 4, total_score);
		
		row++;
	}
	wordnets.SetCount(row);
	wordnets.SetSortColumn(4, true);
	
	
	row = 0;
	for(int i = 0; i < da.clr_wordnets.GetCount(); i++) {
		ColorWordnet& wn = da.clr_wordnets[i];
		
		// Filter by color group
		if (clr_filter && wn.clr_group != clr_i)
			continue;
		
		clr_wordnets.Set(row, "IDX", i);
		
		// Colored main class
		bool has_main_class_clr = false;
		Color main_class_clr;
		{
			int j = GetWordgroupColors().Find(wn.main_class);
			if (j >= 0) {
				main_class_clr = GetWordgroupColors()[j];
				has_main_class_clr = true;
			}
		}
		if (has_main_class_clr) {
			clr_wordnets.Set(row, 0,
				AttrText(wn.main_class)
					.NormalPaper(Blend(main_class_clr, White(), 128+64)).NormalInk(Black())
					.Paper(Blend(main_class_clr, GrayColor())).Ink(White())
			);
		}
		else {
			clr_wordnets.Set(row, 0, wn.main_class);
		}
		
		
		// Source word
		{
			clr_wordnets.Set(row, 1,
				AttrText(wn.src_word)
					.NormalPaper(Blend(wn.clr, White(), 128+64)).NormalInk(Black())
					.Paper(Blend(GrayColor(), wn.clr)).Ink(White()));
		}
		
		
		// Alternative words
		int c = min(7, wn.words.GetCount());
		for(int j = 0; j < c; j++) {
			const String& txt = wn.words[j];
			Color clr = wn.colors[j];
			clr_wordnets.Set(row, 2+j,
				AttrText(txt)
					.NormalPaper(Blend(clr, White(), 128+64)).NormalInk(Black())
					.Paper(Blend(GrayColor(), clr)).Ink(White()));
		}
		for(int j = c; j < 7; j++)
			clr_wordnets.Set(row, 2+j, Value());
		
		int total_score = 0, l = 0;
		for(int j = 0; j < SCORE_MODE_COUNT; j++) {
			for(int k = 0; k < SCORE_ATTR_COUNT; k++) {
				total_score += wn.scores[j][k];
				clr_wordnets.Set(row, 10+l, wn.scores[j][k]);
				l++;
			}
		}
		clr_wordnets.Set(row, 9, total_score);
		
		row++;
	}
	clr_wordnets.SetCount(row);
	clr_wordnets.SetSortColumn(9, true);
	
}

void SongDataWordnetScoring::ToolMenu(Bar& bar) {
	bar.Add(t_("Update data"), AppImg::BlueRing(), THISBACK(DataMain)).Key(K_CTRL_Q);
	for(int i = 0; i < SCORE_MODE_COUNT; i++) {
		bar.Separator();
		if (running)
			bar.Add(t_("Stop getting wordnet scores") + Format(" %d", i), AppImg::RedRing(), THISBACK1(ToggleGettingWordnetScores,i)).Key(K_F6 + i);
		else
			bar.Add(t_("Start getting wordnet scores") + Format(" %d", i), AppImg::RedRing(), THISBACK1(ToggleGettingWordnetScores,i)).Key(K_F6 + i);
		
		if (running)
			bar.Add(t_("Stop getting colored wordnet scores") + Format(" %d", i), AppImg::RedRing(), THISBACK1(ToggleGettingColorWordnetScores,i)).Key(K_F6 + i);
		else
			bar.Add(t_("Start getting colored wordnet scores") + Format(" %d", i), AppImg::RedRing(), THISBACK1(ToggleGettingColorWordnetScores,i)).Key(K_F6 + i);
	}
}

void SongDataWordnetScoring::ToggleGettingWordnetScores(int score_mode) {
	running = !running;
	if (running) {
		Thread::Start(THISBACK2(GetWordnetScores, 0, score_mode));
	}
}

void SongDataWordnetScoring::GetWordnetScores(int batch_i, int score_mode) {
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
	
	tmp_wordnets.Clear();
	
	int iter = 0;
	for(int ds_i = 0; ds_i < sd.GetCount(); ds_i++) {
		String ds_key = sd.GetKey(ds_i);
		DatasetAnalysis& ds = sda.datasets.GetAdd(ds_key);
		
		for(int i = 0; i < ds.wordnets.GetCount(); i++) {
			Wordnet& wn = ds.wordnets[i];
			
			if (iter >= begin) {
				String key = wn.group + ": " + wn.value + ": " + wn.main_class + ": ";
				int c = min(5, wn.words.GetCount());
				for(int j = 0; j < c; j++) {
					if (j) key << ", ";
					key << wn.words[j];
				}
				
				args.words << key;
				tmp_wordnets << &wn;
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
	
	args.fn = 9;
	args.score_mode = score_mode;
	
	TaskMgr& m = SongLib::TaskManager::Single().MakePipe();
	m.GetSongDataAnalysis(args, THISBACK2(OnWordnetScores, batch_i, score_mode));
}

void SongDataWordnetScoring::OnWordnetScores(String res, int batch_i, int score_mode) {
	if (Thread::IsShutdownThreads())
		return;
	
	Database& db = Database::Single();
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
			if (parts.GetCount() != SCORE_ATTR_COUNT)
				continue;
			int score[SCORE_ATTR_COUNT] = {0,0,0,0,0};
			for(int i = 0; i < parts.GetCount(); i++) {
				String& s = parts[i];
				int a = s.Find(":");
				if (a < 0) continue;
				score[i] = StrInt(TrimBoth(s.Mid(a+1)));
			}
			
			Wordnet& wn = *tmp_wordnets[line_idx];
			for(int i = 0; i < SCORE_ATTR_COUNT; i++)
				wn.scores[score_mode][i] = score[i];
		}
	}
	
	if (running)
		PostCallback(THISBACK2(GetWordnetScores, batch_i+1, score_mode));
}

void SongDataWordnetScoring::ToggleGettingColorWordnetScores(int score_mode) {
	running = !running;
	if (running) {
		Thread::Start(THISBACK2(GetColorWordnetScores, 0, score_mode));
	}
}

void SongDataWordnetScoring::GetColorWordnetScores(int batch_i, int score_mode) {
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
	
	tmp_clr_wordnets.Clear();
	
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
	
	args.fn = 9;
	args.score_mode = score_mode;
	
	TaskMgr& m = SongLib::TaskManager::Single().MakePipe();
	m.GetSongDataAnalysis(args, THISBACK2(OnColorWordnetScores, batch_i, score_mode));
}

void SongDataWordnetScoring::OnColorWordnetScores(String res, int batch_i, int score_mode) {
	if (Thread::IsShutdownThreads())
		return;
	
	Database& db = Database::Single();
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
			if (parts.GetCount() != SCORE_ATTR_COUNT)
				continue;
			int score[SCORE_ATTR_COUNT] = {0,0,0,0,0};
			for(int i = 0; i < parts.GetCount(); i++) {
				String& s = parts[i];
				int a = s.Find(":");
				if (a < 0) continue;
				score[i] = StrInt(TrimBoth(s.Mid(a+1)));
			}
			
			ColorWordnet& wn = *tmp_clr_wordnets[line_idx];
			for(int i = 0; i < SCORE_ATTR_COUNT; i++)
				wn.scores[score_mode][i] = score[i];
		}
	}
	
	if (running)
		PostCallback(THISBACK2(GetColorWordnetScores, batch_i+1, score_mode));
}
