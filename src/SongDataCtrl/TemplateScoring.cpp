#include "SongDataCtrl.h"

#if 0


SongDataTemplateScoring::SongDataTemplateScoring() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << vsplit << scores;
	hsplit.SetPos(2000);
	
	vsplit.Vert() << datasets << attrs << colors;
	vsplit.SetPos(1000,0);
	
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
	
	scores.AddColumn(t_("Group"));
	scores.AddColumn(t_("Value"));
	scores.AddColumn(t_("Phrase"));
	scores.AddColumn(t_("Total score"));
	String sc;
	for(int i = 0; i < SCORE_MODE_COUNT; i++) {
		for(int j = 0; j < SCORE_ATTR_COUNT; j++) {
			scores.AddColumn(ScoreTitles[i][j]);
			sc << " 1";
		}
	}
	scores.AddIndex("IDX");
	scores.ColumnWidths("2 2 4" + sc);
	
}

void SongDataTemplateScoring::EnableAll() {
	
}

void SongDataTemplateScoring::DisableAll() {
	
}

void SongDataTemplateScoring::Data() {
	
}

void SongDataTemplateScoring::DataMain() {
	SongDatabase& db = SongDatabase::Single();
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
	
void SongDataTemplateScoring::DataDataset() {
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

void SongDataTemplateScoring::DataAttribute() {
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

void SongDataTemplateScoring::DataColor() {
	if (!datasets.IsCursor() || !colors.IsCursor() || !attrs.IsCursor())
		return;
	
	SongDatabase& db = SongDatabase::Single();
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
	
	lock.Enter();
	
	int row = 0;
	for(int i = 0; i < da.tmpl_phrases.GetCount(); i++) {
		TemplatePhrase& tp = da.tmpl_phrases[i];
		
		// Filter by color group
		if (clr_filter && GetColorGroup(tp.clr) != clr_i)
			continue;
		
		// Filter by attribute
		if (attr_filter && (tp.group != group_str || tp.value != value_str))
			continue;
		
		String s;
		for (const String& p : tp.parts) {
			if (p == ",")
				s << ",";
			else {
				if (s.GetCount())
					s << " ";
				s << p;
			}
		}
		scores.Set(row, "IDX", i);
		scores.Set(row, 0, tp.group);
		scores.Set(row, 1, tp.value);
		scores.Set(row, 2,
			AttrText(s)
				.NormalPaper(Blend(tp.clr, White(), 128+64)).NormalInk(Black())
				.Paper(Blend(tp.clr, GrayColor())).Ink(White())
			);
		
		int total_score = 0;
		int l = 0;
		for(int j = 0; j < SCORE_MODE_COUNT; j++) {
			for(int k = 0; k < SCORE_ATTR_COUNT; k++) {
				total_score += tp.scores[j][k];
				scores.Set(row, 4+l, (int)(tp.scores[j][k]));
				l++;
			}
		}
		scores.Set(row, 3, total_score);
		row++;
	}
	scores.SetCount(row);
	scores.SetSortColumn(3, true);
	
	lock.Leave();
}

void SongDataTemplateScoring::ToolMenu(Bar& bar) {
	bar.Add(t_("Update data"), AppImg::BlueRing(), THISBACK(DataMain)).Key(K_CTRL_Q);
	for(int i = 0; i < SCORE_MODE_COUNT; i++) {
		bar.Separator();
		if (running)
			bar.Add(t_("Stop getting template scores") + Format(" %d", i), AppImg::RedRing(), THISBACK1(ToggleGettingTemplates,i));
		else
			bar.Add(t_("Start getting template scores") + Format(" %d", i), AppImg::RedRing(), THISBACK1(ToggleGettingTemplates,i));
	}
}

void SongDataTemplateScoring::ToggleGettingTemplates(int score_mode) {
	running = !running;
	if (running) {
		Thread::Start(THISBACK2(GetTemplateScores, 0, score_mode));
	}
}

void SongDataTemplateScoring::GetTemplateScores(int batch_i, int score_mode) {
	if (Thread::IsShutdownThreads())
		return;
	
	SongDatabase& db = SongDatabase::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	int begin = batch_i * per_batch;
	int end = (batch_i+1) * per_batch;
	
	if (batch_i < 0) {
		begin = 0;
		end = 1;
	}
	
	SongDataAnalysisArgs args;
	
	line_to_src.Clear();
	ds_is.Clear();
	phrases.Clear();
	int iter = 0;
	for(int ds_i = 0; ds_i < sd.GetCount(); ds_i++) {
		String ds_key = sd.GetKey(ds_i);
		DatasetAnalysis& ds = sda.datasets.GetAdd(ds_key);
		
		for(int i = 0; i < ds.tmpl_phrases.GetCount(); i++) {
			if (iter >= begin) {
				TemplatePhrase& tp =  ds.tmpl_phrases[i];
				String s;
				for (const String& p : tp.parts) {
					if (p == ",")
						s << ",";
					else {
						if (s.GetCount())
							s << " ";
						s << p;
					}
				}
				
				String phrase;
				phrase << tp.group << ": " << tp.value << ": \"" << s << "\"";
				
				for(const Vector<String>& v : tp.words) {
					phrase << ", [";
					int c = min(v.GetCount(), 5);
					for(int j = 0; j < c; j++) {
						if (j) phrase << ",";
						phrase << v[j];
					}
					phrase << "]";
				}
				
				line_to_src.Add(iter);
				ds_is.Add(ds_i);
				phrases.Add(&tp);
				args.phrases << phrase;
			}
			iter++;
			if (iter >= end) break;
		}
		if (iter >=  end) break;
	}
	
	if (args.phrases.IsEmpty()) {
		if (batch)
			batch = false;
		return;
	}
	
	Song& song = GetSong();
	
	args.fn = 8;
	args.score_mode = score_mode;
	
	TaskMgr& m = TaskMgr::Single();
	m.GetSongDataAnalysis(args, THISBACK2(OnTemplateScores, batch_i, score_mode));
}

void SongDataTemplateScoring::OnTemplateScores(String res, int batch_i, int score_mode) {
	if (Thread::IsShutdownThreads())
		return;
	
	
	/*
	Regular answer
	18. "With the {noun}, {noun} cock n blow it baby": S0: 8, S1: 8, S2: 8, S3: 6, S4: 7
	
	Freak answer:
	13. "we {verb} d, I keep all of {dem}": S0: 7, S1: 8, S2: 8, S3: 8, S4: 7
	     - [lit,cool,hyped,poppin,turnt]: S0: 6, S1: 7, S2: 8, S3: 8, S4: 6
	     - [dem,them,those,these,that]: S0: 6, S1: 7, S2: 8, S3: 8, S4: 6
	14. "Baby boy my {noun}": S0: 8, S1: 6, S2: 6, S3: 7, S4: 8
	*/
	
	lock.Enter();
	
	SongDatabase& db = SongDatabase::Single();
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
			
			int a = l.Find(":");
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
			
			TemplatePhrase& tp = *phrases[line_idx];
			for(int i = 0; i < SCORE_ATTR_COUNT; i++)
				tp.scores[score_mode][i] = score[i];
		}
	}
	
	lock.Leave();
	
	if (running)
		PostCallback(THISBACK2(GetTemplateScores, batch_i+1, score_mode));
}

#endif
