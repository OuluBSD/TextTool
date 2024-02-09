#include "SongDataCtrl.h"


void ScoreDisplay::Paint(Draw& d, const Rect& r, const Value& q,
                         Color ink, Color paper, dword style) const {
	d.DrawRect(r, Blend(paper, White()));
	ValueArray va = q;
	if (va.GetCount() == SCORE_COUNT) {
		int w = r.Width();
		int h = r.Height();
		double cx = (double)w / (double)SCORE_COUNT;
		Color clr = LtBlue();
		for(int i = 0; i < va.GetCount(); i++) {
			int sc = va[i];
			int h0 = h * sc / 10;
			int x0 = (int)(i * cx);
			int x1 = (int)((i+1) * cx);
			int y0 = h - h0;
			d.DrawRect(Rect(x0,y0,x1,h), clr);
		}
		for(int i = 1; i < SCORE_COUNT; i++) {
			int x0 = (int)(i * cx);
			d.DrawLine(x0,0,x0,h,1,GrayColor());
		}
	}
}



PhrasePartAnalysis::PhrasePartAnalysis() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << vsplit << parts;
	hsplit.SetPos(2000);
	
	vsplit.Vert() << datasets << attrs << colors << actions << action_args;
	vsplit.SetPos(1000,0);
	
	datasets.AddColumn(t_("Dataset"));
	datasets.WhenCursor << THISBACK(DataDataset);
	
	attrs.AddColumn(t_("Group"));
	attrs.AddColumn(t_("Value"));
	attrs.AddColumn(t_("Count"));
	attrs.ColumnWidths("3 3 1");
	attrs.WhenCursor << [this]() {
		DatabaseBrowser::Single().SetAttr(attrs.GetCursor());
		DataAttribute();
	};
	
	colors.AddColumn(t_("Colors"));
	colors.AddColumn(t_("Count"));
	colors.ColumnWidths("3 1");
	colors.WhenCursor << [this]() {
		DatabaseBrowser::Single().SetColor(colors.GetCursor());
		DataColor();
	};
	
	actions.AddColumn(t_("Action"));
	actions.AddColumn(t_("Count"));
	actions.ColumnWidths("3 1");
	actions.WhenCursor << [this]() {
		DatabaseBrowser::Single().SetGroup(actions.GetCursor());
		DataAction();
	};
	
	action_args.AddColumn(t_("Action args"));
	action_args.AddColumn(t_("Count"));
	action_args.ColumnWidths("3 1");
	action_args.WhenCursor << [this]() {
		DatabaseBrowser::Single().SetValue(action_args.GetCursor());
		DataActionHeader();
	};
	
	parts.AddColumn(t_("Actions"));
	parts.AddColumn(t_("Group"));
	parts.AddColumn(t_("Value"));
	parts.AddColumn(t_("Phrase"));
	parts.AddColumn(t_("Scores")).SetDisplay(Single<ScoreDisplay>());
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
	
}

void PhrasePartAnalysis::ToolMenu(Bar& bar) {
	bar.Add(t_("Get phrase colors"), AppImg::RedRing(), THISBACK1(DoPhrases, 0)).Key(K_F5);
	bar.Add(t_("Get phrase attributes"), AppImg::RedRing(), THISBACK1(DoPhrases, 1)).Key(K_F6);
	bar.Add(t_("Get phrase actions"), AppImg::RedRing(), THISBACK1(DoPhrases, 2)).Key(K_F7);
	bar.Add(t_("Get phrase scores"), AppImg::RedRing(), THISBACK1(DoPhrases, 3)).Key(K_F8);
	bar.Separator();
	bar.Add(t_("Get typecasts"), AppImg::RedRing(), THISBACK1(DoPhrases, 4)).Key(K_CTRL_1);
	bar.Add(t_("Get singer profile"), AppImg::RedRing(), THISBACK1(DoPhrases, 5)).Key(K_CTRL_2);
	bar.Add(t_("Get primary human"), AppImg::RedRing(), THISBACK1(DoPhrases, 6)).Key(K_CTRL_3);
	bar.Add(t_("Get secondary human"), AppImg::RedRing(), THISBACK1(DoPhrases, 7)).Key(K_CTRL_4);
	bar.Separator();
	bar.Add(t_("Update action counts"), AppImg::RedRing(), THISBACK(UpdateCounts)).Key(K_F9);
}

void PhrasePartAnalysis::Data() {
	DataMain();
}

void PhrasePartAnalysis::DataMain() {
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

void PhrasePartAnalysis::DataDataset() {
	if (!datasets.IsCursor())
		return;
	
	int ds_i = datasets.GetCursor();
	DatabaseBrowser& b = DatabaseBrowser::Single();
	b.SetDataset(ds_i);
		
	// Set attributes
	for(int i = 0; i < b.attrs.GetCount(); i++) {
		DatabaseBrowser::Attr& a = b.attrs[i];
		attrs.Set(i, 0, a.group);
		attrs.Set(i, 1, a.value);
		attrs.Set(i, 2, a.count);
	}
	INHIBIT_CURSOR(attrs);
	attrs.SetCount(b.attrs.GetCount());
	attrs.SetSortColumn(2, true);
	if (!attrs.IsCursor() && attrs.GetCount())
		attrs.SetCursor(0);
	
	
	DataAttribute();
}

void PhrasePartAnalysis::DataAttribute() {
	if (!attrs.IsCursor())
		return;
	
	DatabaseBrowser& b = DatabaseBrowser::Single();
		
	// Set color
	for(int i = 0; i < b.colors.GetCount(); i++) {
		DatabaseBrowser::ColorGroup& a = b.colors[i];
		SetColoredListValue(colors, i, 0, a.group, a.clr, false);
		colors.Set(i, 1, a.count);
	}
	INHIBIT_CURSOR(attrs);
	colors.SetCount(b.colors.GetCount());
	colors.SetSortColumn(1, true);
	if (!colors.IsCursor() && colors.GetCount())
		colors.SetCursor(0);
	
	
	DataColor();
}

void PhrasePartAnalysis::DataColor() {
	if (!datasets.IsCursor() || !colors.IsCursor() || !attrs.IsCursor())
		return;
	
	DatabaseBrowser& b = DatabaseBrowser::Single();
		
	// Set color
	for(int i = 0; i < b.groups.GetCount(); i++) {
		DatabaseBrowser::ActionGroup& a = b.groups[i];
		actions.Set(i, 0, a.group);
		actions.Set(i, 1, a.count);
	}
	INHIBIT_CURSOR(actions);
	actions.SetCount(b.groups.GetCount());
	actions.SetSortColumn(1, true);
	if (!actions.IsCursor() && actions.GetCount())
		actions.SetCursor(0);
	
	DataAction();
}

void PhrasePartAnalysis::DataAction() {
	if (!datasets.IsCursor() || !actions.IsCursor())
		return;
	
	DatabaseBrowser& b = DatabaseBrowser::Single();
		
	// Set color
	for(int i = 0; i < b.values.GetCount(); i++) {
		DatabaseBrowser::ActionValue& a = b.values[i];
		action_args.Set(i, 0, a.value);
		action_args.Set(i, 1, a.count);
	}
	INHIBIT_CURSOR(action_args);
	action_args.SetCount(b.values.GetCount());
	action_args.SetSortColumn(1, true);
	if (!action_args.IsCursor() && action_args.GetCount())
		action_args.SetCursor(0);
	
	
	DataActionHeader();
}

void PhrasePartAnalysis::DataActionHeader() {
	if (!datasets.IsCursor() || !colors.IsCursor() || !attrs.IsCursor() ||
		!actions.IsCursor() || !action_args.IsCursor())
		return;
	
	int ds_i = datasets.GetCursor();
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
			
		ValueArray va;
		int sum = 0;
		for(int i = 0; i < SCORE_COUNT; i++) {
			va.Add(pp.scores[i]);
			sum += pp.scores[i];
		}
		parts.Set(row, 4, va);
		parts.Set(row, 5, sum);
	}
	parts.SetCount(count);
	parts.SetSortColumn(5, true);
	
}

void PhrasePartAnalysis::DoPhrases(int fn) {
	int ds_i = datasets.GetCursor();
	SongLib::TaskManager& tm = SongLib::TaskManager::Single();
	tm.DoPhrases(ds_i, fn);
}

void PhrasePartAnalysis::UpdateCounts() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	int ds_i = datasets.GetCursor();
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	for (ExportAction& ea : da.actions.GetValues())
		ea.count = 0;
	for (ExportAttr& ea : da.attrs.GetValues())
		ea.count = 0;
	
	for(int i = 0; i < da.phrase_parts.GetCount(); i++) {
		PhrasePart& pp = da.phrase_parts[i];
		
		for (int ah_i : pp.actions) {
			ExportAction& ea = da.actions[ah_i];
			ea.count++;
		}
		
		if (pp.attr >= 0) {
			ExportAttr& ea = da.attrs[pp.attr];
			ea.count++;
		}
	}
}

