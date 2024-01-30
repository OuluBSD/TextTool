#include "EditorCtrl.h"


void Paint_ScoreCtrl(Size& sz, ImageDraw& d, int q[SCORE_COUNT]) {
	d.DrawRect(sz, White());
	
	int w = sz.cx;
	int h = sz.cy;
	double cx = (double)w / (double)SCORE_COUNT;
	Color clr = LtBlue();
	for(int i = 0; i < SCORE_COUNT; i++) {
		int sc = q[i];
		int h0 = h * sc / 10;
		int x0 = (int)(i * cx);
		int x1 = (int)((i+1) * cx);
		int y0 = h - h0;
		
		Color clr = GetGroupColor(1+i); // unrelated, but gives nice colors
		d.DrawRect(Rect(x0,y0,x1,h), clr);
	}
	for(int i = 1; i < SCORE_COUNT; i++) {
		int x0 = (int)(i * cx);
		d.DrawLine(x0,0,x0,h,1,GrayColor());
	}
}


void LinePicker::SetBrowserMode(int i) {
	vsplit.Remove(ctrls);
	vsplit.Remove(attrs);
	vsplit.Remove(colors);
	vsplit.Remove(actions);
	vsplit.Remove(action_args);
	
	if (i == 0)
		vsplit.Vert() << ctrls << attrs << colors << actions << action_args;
	if (i == 1)
		vsplit.Vert() << ctrls << actions << action_args << colors << attrs;
	if (i == 2)
		vsplit.Vert() << ctrls << colors << actions << action_args << attrs;
	if (i == 3)
		vsplit.Vert() << ctrls << colors << actions << attrs << action_args;
	vsplit.SetPos(1500,0);
}





LinePicker::LinePicker() {
	Add(main_split.SizePos());
	
	PostCallback(THISBACK1(SetView, 0));
	
	hsplit.Horz() << vsplit << parts;
	hsplit.SetPos(2000);
	
	
	CtrlLayout(ctrls);
	
	ctrls.browser_list.Add(t_("Attribute - Color - Action"));
	ctrls.browser_list.Add(t_("Action - Color - Attribute"));
	ctrls.browser_list.Add(t_("Color - Action - Attribute"));
	ctrls.browser_list.Add(t_("Color - Action group - Attribute - Action value"));
	ctrls.browser_list.SetIndex(0);
	ctrls.browser_list.WhenAction << [this]() {
		DatabaseBrowser& b = DatabaseBrowser::Single();
		int mode = ctrls.browser_list.GetIndex();
		SetBrowserMode(mode);
		b.SetMode(mode);
		PostCallback(THISBACK1(DataMain, b.GetCur(0)));
	};
	ctrls.sort_list.Add(t_("Total score"));
	for(int i = 0; i < SCORE_COUNT; i++)
		ctrls.sort_list.Add(ScoreTitles[i]);
	ctrls.sort_list.SetIndex(0);
	ctrls.sort_list.WhenAction << [this]() {
		DatabaseBrowser::Single().SortBy(ctrls.sort_list.GetIndex());
		PostCallback(THISBACK(DataPhrases));
	};
	ctrls.end_rhyme.WhenEnter << [this]() {
		SetEndRhyme(ctrls.end_rhyme.GetData());
	};
	ctrls.mid_rhyme.WhenEnter << [this]() {
		SetMidRhyme(ctrls.mid_rhyme.GetData());
	};
	ctrls.mid_rhyming_limit.SetData(DatabaseBrowser::Single().GetMidRhymingLimit());
	ctrls.mid_rhyming_limit.WhenEnter << [this]() {
		DatabaseBrowser& b = DatabaseBrowser::Single();
		b.SetMidRhymingLimit(ctrls.mid_rhyming_limit.GetData());
		PostCallback(THISBACK1(DataMain, -1));
	};
	ctrls.end_rhyming_limit.SetData(DatabaseBrowser::Single().GetEndRhymingLimit());
	ctrls.end_rhyming_limit.WhenEnter << [this]() {
		DatabaseBrowser& b = DatabaseBrowser::Single();
		b.SetEndRhymingLimit(ctrls.end_rhyming_limit.GetData());
		PostCallback(THISBACK1(DataMain, -1));
	};
	
	PostCallback([this]() {
		SetBrowserMode(0);
		DatabaseBrowser::Single().ds_i = ds_i;
		DatabaseBrowser::Single().SetMode(0);
		PostCallback(THISBACK1(DataMain, -1));
	});
	
	attrs.AddColumn(t_("Group"));
	attrs.AddColumn(t_("Value"));
	attrs.AddColumn(t_("Count"));
	attrs.ColumnWidths("3 3 1");
	attrs.WhenCursor << [this]() {
		SetFilterValues();
		DatabaseBrowser& b = DatabaseBrowser::Single();
		b.SetAttr(attrs.GetCursor());
		PostCallback(THISBACK1(DataMain, b.GetCur(0)));
	};
	
	colors.AddColumn(t_("Colors"));
	colors.AddColumn(t_("Count"));
	colors.ColumnWidths("3 1");
	colors.WhenCursor << [this]() {
		SetFilterValues();
		DatabaseBrowser& b = DatabaseBrowser::Single();
		b.SetColor(colors.GetCursor());
		PostCallback(THISBACK1(DataMain, b.GetCur(1)));
	};
	
	actions.AddColumn(t_("Action"));
	actions.AddColumn(t_("Count"));
	actions.ColumnWidths("3 1");
	actions.WhenCursor << [this]() {
		SetFilterValues();
		DatabaseBrowser& b = DatabaseBrowser::Single();
		b.SetGroup(actions.GetCursor());
		PostCallback(THISBACK1(DataMain, b.GetCur(2)));
	};
	
	action_args.AddColumn(t_("Action args"));
	action_args.AddColumn(t_("Count"));
	action_args.ColumnWidths("3 1");
	action_args.WhenCursor << [this]() {
		SetFilterValues();
		DatabaseBrowser& b = DatabaseBrowser::Single();
		b.SetValue(action_args.GetCursor());
		PostCallback(THISBACK1(DataMain, b.GetCur(3)));
	};
	
	parts.AddColumn(t_("Actions"));
	parts.AddColumn(t_("Group"));
	parts.AddColumn(t_("Value"));
	parts.AddColumn(t_("Phrase"));
	parts.AddColumn(t_("Scores (Ide,Emo,Hoo,Sha,Val,Com,Sex,Pol,Lov,Soc)"));
	parts.AddColumn(t_("Score-sum"));
	parts.AddIndex("IDX");
	parts.ColumnWidths("16 6 6 8 3 1");
	parts.WhenBar << [this](Bar& bar){
		bar.Add("Copy", [this]() {
			int i = parts.GetCursor();
			AttrText text = parts.Get(i, 3);
			String s = text.text.ToString();
			WriteClipboardText(s);
		});
	};
	
	picked.AddColumn(t_("Actions"));
	picked.AddColumn(t_("Group"));
	picked.AddColumn(t_("Value"));
	picked.AddColumn(t_("Phrase"));
	picked.AddColumn(t_("Scores (Ide,Emo,Hoo,Sha,Val,Com,Sex,Pol,Lov,Soc)"));
	picked.AddColumn(t_("Score-sum"));
	picked.AddIndex("IDX");
	picked.AddIndex("PART");
	picked.AddIndex("LINE");
	picked.ColumnWidths("16 6 6 8 3 1");
	picked.WhenBar << [this](Bar& bar){
		if (mode == MODE_PICKED)
			bar.Add("Remove", THISBACK(RemoveLine));
	};
	picked.WhenCursor << [this]() {
		if (mode == MODE_SUBPICKED)
			DataSubPicked();
	};
	
	subpicked.AddColumn(t_("Actions"));
	subpicked.AddColumn(t_("Group"));
	subpicked.AddColumn(t_("Value"));
	subpicked.AddColumn(t_("Phrase"));
	subpicked.AddColumn(t_("Scores (Ide,Emo,Hoo,Sha,Val,Com,Sex,Pol,Lov,Soc)"));
	subpicked.AddColumn(t_("Score-sum"));
	subpicked.AddIndex("IDX");
	subpicked.ColumnWidths("16 6 6 8 3 1");
	subpicked.WhenBar << [this](Bar& bar){
		if (mode == MODE_SUBPICKED)
			bar.Add("Remove", THISBACK(RemoveLine));
	};
	
}

void LinePicker::SetView(int i) {
	Database& db = Database::Single();
	this->mode = i;
	
	main_split.Clear();
	
	switch(i) {
		
	case MODE_PICKED:
		main_split.Vert() << hsplit << picked;
		main_split.SetPos(8000);
		if (db.ctx.ed.song)
			DataPicked();
		break;
		
	case MODE_SUBPICKED:
		main_split.Vert() << hsplit << picked << subpicked;
		main_split.SetPos(7000, 0);
		main_split.SetPos(8500, 1);
		if (db.ctx.ed.song) {
			DataPicked();
			DataSubPicked();
		}
		break;
		
	}
}

void LinePicker::ToolMenu(Bar& bar) {
	bar.Add(t_("Add line"), AppImg::BlueRing(), THISBACK(AddLine)).Key(K_CTRL_Q);
	bar.Add(t_("Remove line"), AppImg::BlueRing(), THISBACK(RemoveLine)).Key(K_CTRL_W);
	bar.Separator();
	bar.Add(t_("Toggle view"), AppImg::BlueRing(), THISBACK(ToggleView)).Key(K_F5);
	bar.Add(t_("Clear all"), AppImg::BlueRing(), THISBACK(ClearAll)).Key(K_F9);
}

void LinePicker::Data() {
	DataMain(0);
	DataPicked();
	if (mode == MODE_SUBPICKED)
		DataSubPicked();
}

void LinePicker::SetFilterValues() {
	DatabaseBrowser& b = DatabaseBrowser::Single();
	b.SetMidRhymeFilter(ctrls.mid_rhyme.GetData(), false);
	b.SetEndRhymeFilter(ctrls.end_rhyme.GetData(), false);
	b.SetMidRhymingLimit(ctrls.mid_rhyming_limit.GetData(), false);
	b.SetEndRhymingLimit(ctrls.end_rhyming_limit.GetData(), false);
}

void LinePicker::DataMain(int cur) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	DatabaseBrowser& b = DatabaseBrowser::Single();
	b.SetDataset(ds_i);
	
	// Set attributes
	if (b.IsSub(cur, 0)) {
		for(int i = 0; i < b.attrs.GetCount(); i++) {
			DatabaseBrowser::Attr& a = b.attrs[i];
			attrs.Set(i, 0, a.group);
			attrs.Set(i, 1, a.value);
			attrs.Set(i, 2, a.count);
		}
		INHIBIT_CURSOR(attrs);
		attrs.SetCount(b.attrs.GetCount());
		//attrs.SetSortColumn(2, true);
		if (!attrs.IsCursor() && attrs.GetCount())
			attrs.SetCursor(0);
	}
	
	// Set color
	if (b.IsSub(cur, 1)) {
		for(int i = 0; i < b.colors.GetCount(); i++) {
			DatabaseBrowser::ColorGroup& a = b.colors[i];
			SetColoredListValue(colors, i, 0, a.group, a.clr, false);
			colors.Set(i, 1, a.count);
		}
		INHIBIT_CURSOR(colors);
		colors.SetCount(b.colors.GetCount());
		//colors.SetSortColumn(1, true);
		if (!colors.IsCursor() && colors.GetCount())
			colors.SetCursor(0);
	}
	
	// Set action group
	if (b.IsSub(cur, 2)) {
		for(int i = 0; i < b.groups.GetCount(); i++) {
			DatabaseBrowser::ActionGroup& a = b.groups[i];
			actions.Set(i, 0, a.group);
			actions.Set(i, 1, a.count);
		}
		INHIBIT_CURSOR(actions);
		actions.SetCount(b.groups.GetCount());
		//actions.SetSortColumn(1, true);
		if (!actions.IsCursor() && actions.GetCount())
			actions.SetCursor(0);
	}
	
	// Set action value
	if (b.IsSub(cur, 3)) {
		for(int i = 0; i < b.values.GetCount(); i++) {
			DatabaseBrowser::ActionValue& a = b.values[i];
			action_args.Set(i, 0, a.value);
			action_args.Set(i, 1, a.count);
		}
		INHIBIT_CURSOR(action_args);
		action_args.SetCount(b.values.GetCount());
		//action_args.SetSortColumn(1, true);
		if (!action_args.IsCursor() && action_args.GetCount())
			action_args.SetCursor(0);
	}
	
	DataPhrases();
}

void LinePicker::DataPhrases() {
	if (!colors.IsCursor() || !attrs.IsCursor() ||
		!actions.IsCursor() || !action_args.IsCursor())
		return;
	
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];

	DatabaseBrowser& b = DatabaseBrowser::Single();
	
	int count = min(b.data.GetCount(), 10000);
	for(int i = 0; i < count; i++) {
		int pp_i = b.data[i];
		int row = i;
		PhrasePart& pp = da.phrase_parts[pp_i];
		
		parts.Set(row, 0, da.GetActionString(pp.actions));
		
		String group, value;
		if (pp.attr >= 0) {
			const AttrHeader& ah = da.attrs.GetKey(pp.attr);
			parts.Set(row, 1, ah.group);
			parts.Set(row, 2, ah.value);
		}
		else {
			parts.Set(row, 1, Value());
			parts.Set(row, 2, Value());
		}
		
		parts.Set(row, "IDX", pp_i);
		
		String phrase = da.GetWordString(pp.words);
		parts.Set(row, 3,
			AttrText(phrase)
				.NormalPaper(Blend(pp.clr, White(), 128+64)).NormalInk(Black())
				.Paper(Blend(pp.clr, GrayColor())).Ink(White())
			);
		
		Size sz(80, 10);
		ImageDraw id(sz);
		Paint_ScoreCtrl(sz, id, pp.scores);
		parts.Set(row, 4, AttrText().SetImage(id));
		
		
		int sum = 0;
		for(int i = 0; i < SCORE_COUNT; i++)
			sum += pp.scores[i];
		parts.Set(row, 5, sum);
	}
	parts.SetCount(count);
	//parts.SetSortColumn(5, true);
	
}

void LinePicker::DataPicked() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	Song& song = GetSong();
	
	int cursor = picked.IsCursor() ? picked.GetCursor() : -1;
	
	VectorMap<int,int> line_pps;
	if (mode == MODE_SUBPICKED) {
		for(int i = 0; i < song.parts.GetCount(); i++) {
			StaticPart& sp = song.parts[i];
			const auto& lines = sp.nana.Get();
			for(int j = 0; j < lines.GetCount(); j++) {
				int code = i * 100 + j;
				int pp_i = lines[j].pp_i;
				if (pp_i < 0)
					continue;
				line_pps.Add(code) = pp_i;
			}
		}
	}
	
	int count =
		mode == MODE_PICKED ?
			song.picked_phrase_parts.GetCount() :
			line_pps.GetCount();
	
	int row = 0;
	for(int i = 0; i < count; i++) {
		int pp_i =
			mode == MODE_PICKED ?
				song.picked_phrase_parts[i] :
				line_pps[i];
		
		PhrasePart& pp = da.phrase_parts[pp_i];
		
		picked.Set(row, 0, da.GetActionString(pp.actions));
		
		String group, value;
		if (pp.attr >= 0) {
			const AttrHeader& ah = da.attrs.GetKey(pp.attr);
			picked.Set(row, 1, ah.group);
			picked.Set(row, 2, ah.value);
		}
		else {
			picked.Set(row, 1, Value());
			picked.Set(row, 2, Value());
		}
		
		if (mode == MODE_PICKED) {
			picked.Set(row, "IDX", i);
		}
		else if (mode == MODE_SUBPICKED) {
			picked.Set(row, "IDX", pp_i);
			int code = line_pps.GetKey(i);
			int part = code / 100;
			int line = code % 100;
			picked.Set(row, "PART", part);
			picked.Set(row, "LINE", line);
		}
		
		String phrase = da.GetWordString(pp.words);
		picked.Set(row, 3,
			AttrText(phrase)
				.NormalPaper(Blend(pp.clr, White(), 128+64)).NormalInk(Black())
				.Paper(Blend(pp.clr, GrayColor())).Ink(White())
			);
		
		Size sz(80, 10);
		ImageDraw id(sz);
		Paint_ScoreCtrl(sz, id, pp.scores);
		picked.Set(row, 4, AttrText().SetImage(id));
		
		
		int sum = 0;
		for(int i = 0; i < SCORE_COUNT; i++)
			sum += pp.scores[i];
		picked.Set(row, 5, sum);
		
		row++;
	}
	picked.SetCount(count);
	if (mode == MODE_PICKED)
		picked.SetSortColumn(5, true);
	
	if (!picked.IsCursor() && cursor >= 0) {
		if (cursor < picked.GetCount())
			picked.SetCursor(cursor);
		else if (cursor >= picked.GetCount() && picked.GetCount() > 0)
			picked.SetCursor(picked.GetCount()-1);
	}
}

void LinePicker::DataSubPicked() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	Song& song = GetSong();
	
	if (!picked.IsCursor()) {
		subpicked.Clear();
		return;
	}
	ASSERT(mode == MODE_SUBPICKED);
	int part_i = picked.Get("PART");
	int line_i = picked.Get("LINE");
	
	
	StaticPart& sp = song.parts[part_i];
	const auto& lines = sp.nana.Get();
	const Index<int>& line_pps = lines[line_i].sub_pp_i;
	
	
	int cursor = subpicked.IsCursor() ? subpicked.GetCursor() : -1;
	
	for(int i = 0; i < line_pps.GetCount(); i++) {
		int pp_i = line_pps[i];
		int row = i;
		PhrasePart& pp = da.phrase_parts[pp_i];
		
		subpicked.Set(row, 0, da.GetActionString(pp.actions));
		
		String group, value;
		if (pp.attr >= 0) {
			const AttrHeader& ah = da.attrs.GetKey(pp.attr);
			subpicked.Set(row, 1, ah.group);
			subpicked.Set(row, 2, ah.value);
		}
		else {
			subpicked.Set(row, 1, Value());
			subpicked.Set(row, 2, Value());
		}
		
		subpicked.Set(row, "IDX", pp_i);
		
		String phrase = da.GetWordString(pp.words);
		subpicked.Set(row, 3,
			AttrText(phrase)
				.NormalPaper(Blend(pp.clr, White(), 128+64)).NormalInk(Black())
				.Paper(Blend(pp.clr, GrayColor())).Ink(White())
			);
		
		Size sz(80, 10);
		ImageDraw id(sz);
		Paint_ScoreCtrl(sz, id, pp.scores);
		subpicked.Set(row, 4, AttrText().SetImage(id));
		
		
		int sum = 0;
		for(int i = 0; i < SCORE_COUNT; i++)
			sum += pp.scores[i];
		subpicked.Set(row, 5, sum);
	}
	subpicked.SetCount(line_pps.GetCount());
	subpicked.SetSortColumn(5, true);
	
	if (!subpicked.IsCursor() && cursor >= 0) {
		if (cursor < subpicked.GetCount())
			subpicked.SetCursor(cursor);
		else if (cursor >= subpicked.GetCount() && subpicked.GetCount() > 0)
			subpicked.SetCursor(subpicked.GetCount()-1);
	}
}

void LinePicker::DoPhrases(int fn) {
	SongLib::TaskManager& tm = SongLib::TaskManager::Single();
	tm.DoPhrases(ds_i, fn);
}

void LinePicker::AddLine() {
	if (!parts.IsCursor())
		return;
	
	Song& song = GetSong();
	
	if (mode == MODE_PICKED) {
		int pp_i = parts.Get("IDX");
		song.picked_phrase_parts.FindAdd(pp_i);
		PostCallback(THISBACK(DataPicked));
	}
	else if (mode == MODE_SUBPICKED) {
		if (!picked.IsCursor())
			return;
		
		int pp_i = parts.Get("IDX");
		int part_i = picked.Get("PART");
		int line_i = picked.Get("LINE");
		StaticPart& sp = song.parts[part_i];
		sp.nana.SetSubPicked(line_i, pp_i);
		PostCallback(THISBACK(DataSubPicked));
	}
}

void LinePicker::RemoveLine() {
	Song& song = GetSong();
	
	if (mode == MODE_PICKED) {
		if (!picked.IsCursor())
			return;
		
		int idx = picked.Get("IDX");
		song.picked_phrase_parts.Remove(idx);
		PostCallback(THISBACK(DataPicked));
	}
	else if (mode == MODE_SUBPICKED) {
		if (!picked.IsCursor() || !subpicked.IsCursor())
			return;
		
		int pp_i = subpicked.Get("IDX");
		int part_i = picked.Get("PART");
		int line_i = picked.Get("LINE");
		StaticPart& sp = song.parts[part_i];
		sp.nana.RemoveSubPicked(line_i, pp_i);
		PostCallback(THISBACK(DataSubPicked));
	}
}


void LinePicker::ClearAll() {
	Song& song = GetSong();
	
	if (mode == MODE_PICKED) {
		song.picked_phrase_parts.Clear();
		PostCallback(THISBACK(DataPicked));
	}
	else if (mode == MODE_SUBPICKED) {
		if (!picked.IsCursor())
			return;
		int part_i = picked.Get("PART");
		int line_i = picked.Get("LINE");
		StaticPart& sp = song.parts[part_i];
		sp.nana.ClearSubPicked(line_i);
		PostCallback(THISBACK(DataSubPicked));
	}
}

void LinePicker::SetMidRhyme(String word_) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	Song& song = GetSong();
	
	DatabaseBrowser& b = DatabaseBrowser::Single();
	
	if (!word_.IsEmpty()) {
		int wrd_i = da.words.Find(word_);
		if (wrd_i < 0) {
			PromptOK("Word was not found in the database");
			return;
		}
		b.SetMidRhymingLimit(ctrls.mid_rhyming_limit.GetData(), false);
		b.SetMidRhymeFilter(da.words[wrd_i].phonetic);
	}
	else b.SetMidRhymeFilter(WString());
	
	PostCallback(THISBACK1(DataMain, -1));
}

void LinePicker::SetEndRhyme(String word_) {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	DatasetAnalysis& da = sda.datasets[ds_i];
	Song& song = GetSong();
	
	DatabaseBrowser& b = DatabaseBrowser::Single();
	
	if (!word_.IsEmpty()) {
		int wrd_i = da.words.Find(word_);
		if (wrd_i < 0) {
			PromptOK("Word was not found in the database");
			return;
		}
		b.SetEndRhymingLimit(ctrls.end_rhyming_limit.GetData(), false);
		b.SetEndRhymeFilter(da.words[wrd_i].phonetic);
	}
	else b.SetEndRhymeFilter(WString());
	
	PostCallback(THISBACK1(DataMain, -1));
}
