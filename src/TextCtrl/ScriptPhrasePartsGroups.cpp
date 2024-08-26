#include "TextCtrl.h"
#include <TextDataCtrl/TextDataCtrl.h>


BEGIN_TEXTLIB_NAMESPACE


ScriptPhrasePartsGroups::ScriptPhrasePartsGroups(ToolAppCtrl& o) : o(o) {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << vsplit << parts;
	hsplit.SetPos(3000);

	vsplit.Vert() << attrs << colors << actions << action_args;

	attrs.AddColumn(t_("Group"));
	attrs.AddColumn(t_("Value"));
	attrs.AddColumn(t_("Count"));
	attrs.AddIndex("IDX");
	attrs.ColumnWidths("1 1");
	attrs.WhenCursor << [this]() {
		if (!attrs.IsCursor()) return;
		DatabaseBrowser& b = DatabaseBrowser::Single(this->o.GetAppMode());
		b.SetAttr(attrs.Get("IDX"));
		PostCallback(THISBACK(Data));
		WhenBrowserCursor();
	};
	attrs.WhenBar << [this](Bar& b) {
		b.Add("Jump to next group value", THISBACK1(JumpToGroupValue, +1));
		b.Add("Jump to previous group value", THISBACK1(JumpToGroupValue, -1));
		b.Separator();
		b.Add("Sort by group", [this]{INHIBIT_CURSOR(attrs); attrs.SetSortColumn(0,false); sort[0] = 0;});
		b.Add("Sort by value", [this]{INHIBIT_CURSOR(attrs); attrs.SetSortColumn(1,false); sort[0] = 1;});
		b.Add("Sort by count", [this]{INHIBIT_CURSOR(attrs); attrs.SetSortColumn(2,true); sort[0] = 2;});
	};
	
	colors.AddColumn(t_("Colors"));
	colors.AddColumn(t_("Count"));
	colors.AddIndex("IDX");
	colors.WhenCursor << [this]() {
		if (!colors.IsCursor()) return;
		DatabaseBrowser& b = DatabaseBrowser::Single(this->o.GetAppMode());
		b.SetColor(colors.Get("IDX"));
		PostCallback(THISBACK(Data));
		WhenBrowserCursor();
	};
	colors.WhenBar << [this](Bar& b) {
		b.Add("Sort by color", [this]{INHIBIT_CURSOR(colors); colors.SetSortColumn(0,false); sort[1] = 0;});
		b.Add("Sort by count", [this]{INHIBIT_CURSOR(colors); colors.SetSortColumn(1,true); sort[1] = 1;});
	};
	
	actions.AddColumn(t_("Action"));
	actions.AddColumn(t_("Count"));
	actions.AddIndex("IDX");
	actions.ColumnWidths("3 1");
	actions.WhenCursor << [this]() {
		if (!actions.IsCursor()) return;
		DatabaseBrowser& b = DatabaseBrowser::Single(this->o.GetAppMode());
		b.SetGroup(actions.Get("IDX"));
		PostCallback(THISBACK(Data));
		WhenBrowserCursor();
	};
	actions.WhenBar << [this](Bar& b) {
		b.Add("Sort by name", [this]{INHIBIT_CURSOR(actions); actions.SetSortColumn(0,false); sort[2] = 0;});
		b.Add("Sort by count", [this]{INHIBIT_CURSOR(actions); actions.SetSortColumn(1,true); sort[2] = 1;});
	};
	
	action_args.AddColumn(t_("Action args"));
	action_args.AddColumn(t_("Count"));
	action_args.AddIndex("IDX");
	action_args.ColumnWidths("3 1");
	action_args.WhenCursor << [this]() {
		if (!action_args.IsCursor()) return;
		DatabaseBrowser& b = DatabaseBrowser::Single(this->o.GetAppMode());
		b.SetValue(action_args.Get("IDX"));
		PostCallback(THISBACK(Data));
		WhenBrowserCursor();
	};
	action_args.WhenBar << [this](Bar& b) {
		b.Add("Sort by name", [this]{INHIBIT_CURSOR(action_args); action_args.SetSortColumn(0,false); sort[3] = 0;});
		b.Add("Sort by count", [this]{INHIBIT_CURSOR(action_args); action_args.SetSortColumn(1,true); sort[3] = 1;});
	};
	
	parts.AddColumn(t_("Phrase"));
	parts.AddColumn(t_("Actions"));
	parts.AddColumn(t_("Group"));
	parts.AddColumn(t_("Value"));
	parts.AddColumn(t_("Scores")).SetDisplay(Single<ScoreDisplay>());
	parts.AddColumn(t_("Score-sum"));
	parts.AddIndex("IDX");
	parts.ColumnWidths("16 8 6 6 3 3");
	parts.WhenBar << [this](Bar& bar){
		bar.Add("Copy", [this]() {
			int i = parts.GetCursor();
			AttrText text = parts.Get(i, 3);
			String s = text.text.ToString();
			WriteClipboardText(s);
		});
	};

}

void ScriptPhrasePartsGroups::JumpToGroupValue(int diff) {
	if (!attrs.IsCursor()) return;
	int cur = attrs.GetCursor();
	String key = attrs.Get(0);
	int c = attrs.GetCount();
	for(int i = 1; i < c; i++) {
		int row = (c + cur + i*diff) % c; // first c keeps negative diff as positive sum
		if (attrs.Get(row, 0) == key) {
			attrs.SetCursor(row);
			break;
		}
	}
}


void ScriptPhrasePartsGroups::Data() {
	DatabaseBrowser& b = DatabaseBrowser::Single(this->o.GetAppMode());
	b.SetCtrl(o);
	b.SetMode(0);
	
	bool set_cursor = true;
	
	// Set attributes
	INHIBIT_CURSOR_(attrs, at);
	int row = 0;
	for(int i = 0; i < b.attrs.GetCount(); i++) {
		const auto& ah = b.attrs[i];
		if (!ah.count) break;
		attrs.Set(row, "IDX", i);
		attrs.Set(row, 0, ah.group);
		attrs.Set(row, 1, ah.value);
		attrs.Set(row, 2, ah.count);
		row++;
	}
	attrs.SetCount(row);
	if (sort[0] == 0) attrs.SetSortColumn(0, false);
	if (sort[0] == 1) attrs.SetSortColumn(1, false);
	if (sort[0] == 2) attrs.SetSortColumn(2, true);
	if (set_cursor) {
		int cur = b.GetCursorValue(0);
		if (cur >= 0 && cur < attrs.GetCount())
			attrs.SetCursor(cur);
	}
	
	
	// Colors
	INHIBIT_CURSOR_(colors, c);
	for(int i = 0; i < b.colors.GetCount(); i++) {
		const auto& clr = b.colors[i];
		colors.Set(i, "IDX", i);
		colors.Set(i, 0,
			AttrText("#" + IntStr(clr.clr_i))
				.NormalPaper(clr.clr).NormalInk(Black())
				.Paper(Blend(GrayColor(), GetGroupColor(i))).Ink(White()));
		colors.Set(i, 1, clr.count);
	}
	colors.SetCount(b.colors.GetCount());
	if (sort[1] == 0) colors.SetSortColumn(0, false);
	if (sort[1] == 1) colors.SetSortColumn(1, true);
	if (set_cursor) {
		int cur = b.GetCursorValue(1);
		if (cur >= 0 && cur < colors.GetCount())
			colors.SetCursor(cur);
	}
	
	
	// Actions
	INHIBIT_CURSOR_(actions, a);
	for(int i = 0; i < b.actions.GetCount(); i++) {
		const auto& a = b.actions[i];
		actions.Set(i, "IDX", i);
		actions.Set(i, 0, a.action);
		actions.Set(i, 1, a.count);
	}
	actions.SetCount(b.actions.GetCount());
	if (sort[1] == 0) actions.SetSortColumn(0, false);
	if (sort[1] == 1) actions.SetSortColumn(1, true);
	if (set_cursor) {
		int cur = b.GetCursorValue(2);
		if (cur >= 0 && cur < actions.GetCount())
			actions.SetCursor(cur);
	}
	
	
	// Action args
	INHIBIT_CURSOR_(action_args, ag);
	for(int i = 0; i < b.args.GetCount(); i++) {
		const auto& a = b.args[i];
		action_args.Set(i, "IDX", i);
		action_args.Set(i, 0, a.arg);
		action_args.Set(i, 1, a.count);
	}
	action_args.SetCount(b.args.GetCount());
	if (sort[1] == 0) action_args.SetSortColumn(0, false);
	if (sort[1] == 1) action_args.SetSortColumn(1, true);
	if (set_cursor) {
		int cur = b.GetCursorValue(3);
		if (cur >= 0 && cur < action_args.GetCount())
			action_args.SetCursor(cur);
	}
	
	
	DataList();
}

void ScriptPhrasePartsGroups::DataList() {
	DatabaseBrowser& b = DatabaseBrowser::Single(this->o.GetAppMode());
	
	TextDatabase& db = o.GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;

	int row = 0, max_rows = 10000;
	for(int i = 0; i < b.data.GetCount(); i++) {
		int pp_i = b.data[i];
		PhrasePart& pp = da.phrase_parts[pp_i];

		parts.Set(row, "IDX", pp_i);

		String phrase = da.GetWordString(pp.words);
		parts.Set(row, 0,
			AttrText(phrase)
				.NormalPaper(Blend(pp.clr, White(), 128+64)).NormalInk(Black())
				.Paper(Blend(pp.clr, GrayColor())).Ink(White())
			);

		parts.Set(row, 1, da.GetActionString(pp.actions));


		const AttrHeader& ah = da.attrs.GetKey(pp.attr);
		parts.Set(row, 2, ah.group);
		parts.Set(row, 3, ah.value);
		
		
		ValueArray va;
		int sum = 0;
		for(int i = 0; i < SCORE_COUNT; i++) {
			va.Add(pp.scores[i]);
			sum += pp.scores[i];
		}
		parts.Set(row, 4, va);
		parts.Set(row, 5, sum);

		/*parts.Set(row, 2,
			AttrText(ah.action)
				.NormalPaper(Blend(pp.clr, White(), 128+64)).NormalInk(Black())
				.Paper(Blend(pp.clr, GrayColor())).Ink(White())
			);
		parts.Set(row, 3,
			AttrText(ah.arg)
				.NormalPaper(Blend(pp.clr, White(), 128+64)).NormalInk(Black())
				.Paper(Blend(pp.clr, GrayColor())).Ink(White())
			);*/
		row++;

		if (row >= max_rows)
			break;
	}
	parts.SetCount(row);
	parts.SetSortColumn(5, true);
}

/*void ScriptPhrasePartsGroups::DoPhrases(int fn) {
	TextLib::TaskManager& tm = GetTaskManager();
	tm.DoPhrases(fn);
}*/

void ScriptPhrasePartsGroups::UpdateCounts() {
	
	/*int count = da.phrase_parts.GetCount();
	int row = 0;
	for(int i = 0; i < count; i++) {
		PhrasePart& pp = da.phrase_parts[i];
		pp.profiles.Clear();
		pp.primary.Clear();
		pp.secondary.Clear();
	}*/
	/*for (ExportAction& ea : da.primaries.GetValues())
		ea.count = 0;
	for (ExportAttr& ea : da.typecasts.GetValues())
		ea.count = 0;
	
	for(int i = 0; i < da.phrase_parts.GetCount(); i++) {
		PhrasePart& pp = da.phrase_parts[i];
		
		for (int ah_i : pp.primaries) {
			ExportAction& ea = da.primaries[ah_i];
			ea.count++;
		}
		
		if (pp.attr >= 0) {
			ExportAttr& ea = da.typecasts[pp.attr];
			ea.count++;
		}
	}*/
}


END_TEXTLIB_NAMESPACE

