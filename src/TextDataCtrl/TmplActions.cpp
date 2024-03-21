#include "TextDataCtrl.h"

#if 0


SongDataTmplActions::SongDataTmplActions() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << vsplit << vsplit1;
	hsplit.SetPos(2000);
	
	vsplit.Vert() << datasets << attrs << colors;
	vsplit.SetPos(1000,0);
	
	vsplit1.Vert() << phrases << matches;
	vsplit1.SetPos(8000,0);
	
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
	
	phrases.AddColumn(t_("Group"));
	phrases.AddColumn(t_("Value"));
	phrases.AddColumn(t_("Txt"));
	String cw = "1 1 2";
	for(int i = 0; i < action_cols; i++) {
		cw << " 1";
		phrases.AddColumn("#" + IntStr(i));
	}
	phrases.AddIndex("IDX");
	phrases.ColumnWidths(cw);
	phrases.WhenCursor << THISBACK(DataMatches);
	
	matches.AddColumn(t_("Match score"));
	matches.AddColumn(t_("Phrase"));
	cw = "1 3";
	for(int i = 0; i < action_cols; i++) {
		cw << " 1";
		matches.AddColumn("#" + IntStr(i));
	}
	matches.AddIndex("IDX");
	matches.ColumnWidths(cw);
	
}

void SongDataTmplActions::EnableAll() {
	
}

void SongDataTmplActions::DisableAll() {
	
}

void SongDataTmplActions::Data() {
	
}

void SongDataTmplActions::DataMain() {
	TextDatabase& db = TextDatabase::Single();
	TextData& sd = db.song_data;
	TextDataAnalysis& sda = db.song_data.a;
	
	
	for(int i = 0; i < sda.datasets.GetCount(); i++) {
		datasets.Set(i, 0, sda.datasets.GetKey(i));
	}
	datasets.SetCount(sda.datasets.GetCount());
	if (!datasets.IsCursor() || datasets.GetCount())
		datasets.SetCursor(0);
	
	DataDataset();
}

void SongDataTmplActions::DataDataset() {
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

void SongDataTmplActions::DataAttribute() {
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

void SongDataTmplActions::DataColor() {
	if (!datasets.IsCursor() || !colors.IsCursor() || !attrs.IsCursor())
		return;
	
	TextDatabase& db = TextDatabase::Single();
	TextData& sd = db.song_data;
	TextDataAnalysis& sda = db.song_data.a;
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
	
	int idx = -1, row = 0;
	for (const ActionTemplate& ap : da.action_tmpls) {
		idx++;
		const TemplatePhrase& tp = da.tmpl_phrases[ap.tp_i];
		
		// Filter by color group
		if (clr_filter && GetColorGroup(tp.clr) != clr_i)
			continue;
		
		// Filter by attribute
		if (attr_filter && (tp.group != group_str || tp.value != value_str))
			continue;
		
		String s = tp.GetText();
		
		this->phrases.Set(row, 0, tp.group);
		this->phrases.Set(row, 1, tp.value);
		this->phrases.Set(row, 2, AttrText(s)
			.NormalPaper(Blend(tp.clr, White(), 128+64)).NormalInk(Black())
			.Paper(Blend(tp.clr, GrayColor())).Ink(White()));
		
		int c = min(action_cols, ap.actions.GetCount());
		for(int j = 0; j < c; j++) {
			const auto& aa = ap.actions[j];
			String s = aa.action + "("+ aa.arg + ")";
			this->phrases.Set(row, 3+j, s);
		}
		for(int j = c; j < action_cols; j++)
			this->phrases.Set(row, 3+j, Value());
		
		phrases.Set(row, "IDX", idx);
		row++;
	}
	this->phrases.SetCount(row);
	if (!phrases.IsCursor() && phrases.GetCount())
		phrases.SetCursor(0);
	
	DataMatches();
}

void SongDataTmplActions::DataMatches() {
	if (!datasets.IsCursor() || !attrs.IsCursor() || !colors.IsCursor() || !phrases.IsCursor()) {
		matches.Clear();
		return;
	}
	
	TextDatabase& db = TextDatabase::Single();
	TextData& sd = db.song_data;
	TextDataAnalysis& sda = db.song_data.a;
	int ds_i = datasets.GetCursor();
	DatasetAnalysis& da = sda.datasets[ds_i];
	int phrase_i = phrases.Get("IDX");
	const ActionTemplate& at = da.action_tmpls[phrase_i];
	//const TemplatePhrase& tp = da.tmpl_phrases[ap.tp_i];
	
	int idx = -1, row = 0;;
	for (const ActionPhrase& ap : da.action_phrases) {
		idx++;
		
		int match_count = 0, full_match = 0;
		for (const auto& aa0 : ap.actions) {
			for (const auto& aa1 : at.actions) {
				if (aa0.action == aa1.action) {
					match_count++;
					if (aa0.arg == aa1.arg) {
						full_match++;
					}
				}
			}
		}
		double perc0 = (double)match_count / (ap.actions.GetCount()-1);
		double perc1 = (double)full_match / (ap.actions.GetCount()-1);
		double perc = (perc0 + perc1) * 0.5;
		
		if (perc1 > 0.5) {
			String s = ap.txt;
			matches.Set(row, 0, (int)(perc * 100));
			matches.Set(row, 1, s);
			matches.Set(row, "IDX", idx);
			int c = min(action_cols, ap.actions.GetCount());
			for(int j = 0; j < c; j++) {
				const auto& aa = ap.actions[j];
				String s = aa.action + "("+ aa.arg + ")";
				matches.Set(row, 2+j, s);
			}
			for(int j = c; j < action_cols; j++)
				matches.Set(row, 2+j, Value());
			row++;
		}
	}
	matches.SetCount(row);
	matches.SetSortColumn(0, true);
}

void SongDataTmplActions::ToolMenu(Bar& bar) {
	bar.Add(t_("Update data"), AppImg::BlueRing(), THISBACK(DataMain)).Key(K_CTRL_Q);
	bar.Separator();
	bar.Add(t_("Update batches"), AppImg::BlueRing(), THISBACK(UpdateBatches)).Key(K_F5);
	bar.Separator();
	if (running0)
		bar.Add(t_("Stop getting line actions"), AppImg::RedRing(), THISBACK(ToggleGettingLineActions)).Key(K_F6);
	else
		bar.Add(t_("Start getting line actions"), AppImg::RedRing(), THISBACK(ToggleGettingLineActions)).Key(K_F6);
	bar.Separator();
}

void SongDataTmplActions::UpdateBatches() {
	TextDatabase& db = TextDatabase::Single();
	TextData& sd = db.song_data;
	TextDataAnalysis& sda = db.song_data.a;
	
	batches.SetCount(0);
	batches.Reserve(1000);
	
	
	for (int ds_i = 0; ds_i < sd.GetCount(); ds_i++) {
		DatasetAnalysis& da = sda.datasets[ds_i];
		Vector<String> added_lines;
		Vector<int> added_tpis;
		for(int i = 0; i < da.tmpl_phrases.GetCount(); i++) {
			TemplatePhrase& tp = da.tmpl_phrases[i];
			String s = tp.GetText();
			added_lines << s;
			added_tpis << i;
			if (added_lines.GetCount() >= per_batch) {
				Batch& b = batches.Add();
				b.ds_i = ds_i;
				b.txt = Join(added_lines, "\n");
				for(int j = 0; j < added_tpis.GetCount(); j++)
					b.tp_i[j] = added_tpis[j];
				ASSERT(b.txt.GetCount());
				added_lines.SetCount(0);
				added_tpis.SetCount(0);
			}
		}
		if (added_lines.GetCount()) {
			Batch& b = batches.Add();
			b.ds_i = ds_i;
			b.txt = Join(added_lines, "\n");
			for(int j = 0; j < added_tpis.GetCount(); j++)
				b.tp_i[j] = added_tpis[j];
			ASSERT(b.txt.GetCount());
		}
	}
}

void SongDataTmplActions::ToggleGettingLineActions() {
	running0 = !running0;
	if (running0) {
		if (0) {
			TextDatabase& db = TextDatabase::Single();
			TextData& sd = db.song_data;
			TextDataAnalysis& sda = db.song_data.a;
			for (DatasetAnalysis& da : sda.datasets) {
				da.action_phrases.Clear();
			}
		}
		UpdateBatches();
		Thread::Start(THISBACK1(GetLineActions, 0));
	}
}

void SongDataTmplActions::GetLineActions(int batch_i) {
	if (Thread::IsShutdownThreads())
		return;
	if (batches.IsEmpty()) UpdateBatches();
	if (batch_i < 0 || batch_i >= batches.GetCount()) {
		this->batch = false;
		return;
	}
	Batch& batch = batches[batch_i];
	tmp_batch_i = batch_i;
	
	TextDatabase& db = TextDatabase::Single();
	TextData& sd = db.song_data;
	TextDataAnalysis& sda = db.song_data.a;
	
	TextDataAnalysisArgs args;
	args.fn = 12;
	args.phrases <<= Split(batch.txt, "\n");
	
	
	TaskMgr& m = TaskMgr::Single();
	m.GetTextDataAnalysis(args, THISBACK1(OnLineActions, batch_i));
}

void SongDataTmplActions::OnLineActions(String res, int batch_i) {
	if (Thread::IsShutdownThreads())
		return;
	
	TextDatabase& db = TextDatabase::Single();
	TextData& sd = db.song_data;
	TextDataAnalysis& sda = db.song_data.a;
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
		for (String& l : lines) {
			line_idx++;
			l = TrimBoth(l);
			if (l.IsEmpty())
				continue;
			
			const String& txt = txt_lines[line_idx];
			actions.SetCount(0);
			
			int tp_i = batch.tp_i[line_idx];
			//const TemplatePhrase& tp = da.tmpl_phrases[tp_i];
			
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
			for (ActionTemplate& ap : da.action_tmpls) {
				if (ap.hash == h) {
					found = true;
					ap_is << i;
				}
				i++;
			}
			if (!found) {
				ActionTemplate& ap = da.action_tmpls.Add();
				ap.hash = h;
				ap.tp_i = tp_i;
				Swap(ap.actions, actions);
			}
		}
	}
	
	if (running0)
		PostCallback(THISBACK1(GetLineActions, batch_i+1));
}

#endif
