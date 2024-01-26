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


LinePicker::LinePicker() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << vsplit << parts;
	hsplit.SetPos(2000);
	
	vsplit.Vert() << ctrls << attrs << colors << actions << action_args;
	vsplit.SetPos(1000,0);
	
	CtrlLayout(ctrls);
	ctrls.sort_list.Add(t_("Total score"));
	for(int i = 0; i < SCORE_COUNT; i++)
		ctrls.sort_list.Add(ScoreTitles[i]);
	ctrls.sort_list.SetIndex(0);
	ctrls.sort_list.WhenAction << [this]() {
		DatabaseBrowser::Single().SortBy(ctrls.sort_list.GetIndex());
		PostCallback(THISBACK(DataActionHeader));
	};
	
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
	
}

void LinePicker::ToolMenu(Bar& bar) {
	bar.Add(t_("Get phrase colors"), AppImg::RedRing(), THISBACK1(DoPhrases, 0)).Key(K_F5);
	bar.Add(t_("Get phrase attributes"), AppImg::RedRing(), THISBACK1(DoPhrases, 1)).Key(K_F6);
	bar.Add(t_("Get phrase actions"), AppImg::RedRing(), THISBACK1(DoPhrases, 2)).Key(K_F7);
	bar.Add(t_("Get phrase scores"), AppImg::RedRing(), THISBACK1(DoPhrases, 3)).Key(K_F8);
}

void LinePicker::Data() {
	DataMain();
}

void LinePicker::DataMain() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	
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

void LinePicker::DataAttribute() {
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

void LinePicker::DataColor() {
	if (!colors.IsCursor() || !attrs.IsCursor())
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

void LinePicker::DataAction() {
	if (!actions.IsCursor())
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

void LinePicker::DataActionHeader() {
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

void LinePicker::DoPhrases(int fn) {
	SongLib::TaskManager& tm = SongLib::TaskManager::Single();
	tm.DoPhrases(ds_i, fn);
}

