#include "TextDataCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


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

	vsplit.Vert() << attrs << colors << actions << action_args;

	attrs.AddColumn(t_("Group"));
	attrs.AddColumn(t_("Value"));
	attrs.ColumnWidths("1 1");
	attrs.WhenCursor << THISBACK(DataAttribute);

	colors.AddColumn(t_("Colors"));
	colors.WhenCursor << THISBACK(DataColor);

	actions.AddColumn(t_("Action"));
	actions.AddColumn(t_("Count"));
	actions.ColumnWidths("3 1");
	actions.WhenCursor << THISBACK(DataAction);

	action_args.AddColumn(t_("Action args"));
	action_args.AddColumn(t_("Count"));
	action_args.ColumnWidths("3 1");
	action_args.WhenCursor << THISBACK(DataActionHeader);

	parts.AddColumn(t_("Phrase"));
	parts.AddColumn(t_("Actions"));
	parts.AddColumn(t_("Group"));
	parts.AddColumn(t_("Value"));
	parts.AddColumn(t_("Scores")).SetDisplay(Single<ScoreDisplay>());
	parts.AddColumn(t_("Score-sum"));
	parts.AddIndex("IDX");
	parts.ColumnWidths("8 16 6 6 3 1");
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

}

void PhrasePartAnalysis::Data() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;

	// Set attributes
	attrs.Set(0,0, "All");
	for(int i = 0; i < da.attrs.GetCount(); i++) {
		const AttrHeader& ah = da.attrs.GetKey(i);
		attrs.Set(1+i, 0, ah.group);
		attrs.Set(1+i, 1, ah.value);
	}
	INHIBIT_CURSOR(attrs);
	attrs.SetCount(da.attrs.GetCount());
	if (!attrs.IsCursor() && attrs.GetCount())
		attrs.SetCursor(0);


	DataAttribute();
}

void PhrasePartAnalysis::DataAttribute() {
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

void PhrasePartAnalysis::DataColor() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;


	uniq_acts.Clear();
	for (const ActionHeader& ah : da.actions.GetKeys()) {
		uniq_acts.GetAdd(ah.action).GetAdd(ah.arg, 0)++;
	}
	struct Sorter {
		bool operator()(const VectorMap<String, int>& a, const VectorMap<String, int>& b) const {
			return a.GetCount() > b.GetCount();
		}
	};
	SortByValue(uniq_acts, Sorter());
	for (auto& v : uniq_acts.GetValues())
		SortByValue(v, StdGreater<int>());

	actions.Set(0, 0, "All");
	actions.Set(0, 1, da.actions.GetCount());
	for(int i = 0; i < uniq_acts.GetCount(); i++) {
		actions.Set(1+i, 0, uniq_acts.GetKey(i));
		actions.Set(1+i, 1, uniq_acts[i].GetCount());
	}
	actions.SetCount(1+uniq_acts.GetCount());
	if (!actions.IsCursor() && actions.GetCount())
		actions.SetCursor(0);

	DataAction();
}

void PhrasePartAnalysis::DataAction() {
	if (!actions.IsCursor())
		return;

	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;

	if (actions.IsCursor()) {
		String action = actions.Get(0);
		int i = uniq_acts.Find(action);
		if (i < 0) {
			action_args.SetCount(1);
			action_args.Set(0, 0, "All");
			action_args.Set(0, 1, da.actions.GetCount());
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
	}

	DataActionHeader();
}

void PhrasePartAnalysis::DataActionHeader() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;

	int clr_i = colors.IsCursor() ? colors.GetCursor() : -1;
	int act_i = actions.IsCursor() ? actions.GetCursor() : -1;
	int arg_i = action_args.IsCursor() ? action_args.GetCursor() : -1;
	int attr_i = attrs.IsCursor() ? attrs.GetCursor() : -1;

	bool clr_filter = clr_i > 0;
	bool attr_filter = attr_i > 0;
	bool action_filter = act_i > 0;
	bool arg_filter = arg_i > 0;

	int match_attr = -1;
	if (attr_filter)
		match_attr = attr_i - 1;

	clr_i--;

	String action_str, arg_str;
	if (action_filter) {
		action_str = actions.Get(0);
		if (arg_filter)
			arg_str = action_args.Get(0);
	}

	int row = 0, max_rows = 10000;
	for(int i = 0; i < da.phrase_parts.GetCount(); i++) {
		PhrasePart& pp = da.phrase_parts[i];

		parts.Set(row, "IDX", i);

		String phrase = da.GetWordString(pp.words);
		parts.Set(row, 0,
			AttrText(phrase)
				.NormalPaper(Blend(pp.clr, White(), 128+64)).NormalInk(Black())
				.Paper(Blend(pp.clr, GrayColor())).Ink(White())
			);

		parts.Set(row, 1, da.GetActionString(pp.actions));


		// Filter by attribute
		if (attr_filter) {
			if (pp.attr >= 0) {
				if (match_attr != pp.attr)
					continue;
			}
			else continue;
		}
		if (pp.attr >= 0) {
			const AttrHeader& ah = da.attrs.GetKey(pp.attr);
			parts.Set(row, 2, ah.group);
			parts.Set(row, 3, ah.value);
		}
		else {
			parts.Set(row, 2, Value());
			parts.Set(row, 3, Value());
		}
		
		
		// Filter by color group
		if (clr_filter && GetColorGroup(pp.clr) != clr_i)
			continue;

		// Filter by action
		if (action_filter) {
			bool found = false;
			for (int ah_i : pp.actions) {
				const ActionHeader& ah = da.actions.GetKey(ah_i);
				if (ah.action == action_str) {
					if (arg_filter) {
						if (ah.arg == arg_str)
							found = true;
					}
					else found = true;
				}
			}
			if (!found)
				continue;
		}

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

void PhrasePartAnalysis::DoPhrases(int fn) {
	TextLib::TaskManager& tm = GetTaskManager();
	tm.DoPhrases(fn);
}


END_TEXTLIB_NAMESPACE
