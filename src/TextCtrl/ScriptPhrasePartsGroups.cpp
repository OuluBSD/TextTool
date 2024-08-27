#include "TextCtrl.h"
#include <TextDataCtrl/TextDataCtrl.h>


BEGIN_TEXTLIB_NAMESPACE


ScriptPhrasePartsGroups::ScriptPhrasePartsGroups(ToolAppCtrl& o) : o(o) {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << vsplit << parts;
	hsplit.SetPos(3000);

	vsplit.Vert() << attr_groups << attr_values << colors << actions << action_args
		<< elements << typeclasses << contrasts;

	InitArray(attr_groups, "Attr Group", DatabaseBrowser::ATTR_GROUP);
	InitArray(attr_values, "Attr Value", DatabaseBrowser::ATTR_VALUE);
	InitArray(colors, "Color", DatabaseBrowser::COLOR);
	InitArray(actions, "Action", DatabaseBrowser::ACTION);
	InitArray(action_args, "Arg", DatabaseBrowser::ACTION_ARG);
	InitArray(elements, "Element", DatabaseBrowser::ELEMENT);
	InitArray(typeclasses, "Typeclass", DatabaseBrowser::TYPECLASS);
	InitArray(contrasts, "Contrast", DatabaseBrowser::CONTRAST);
	
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

void ScriptPhrasePartsGroups::InitArray(ArrayCtrl& arr, String title, DatabaseBrowser::ColumnType t) {
	arr.AddColumn(title);
	arr.AddColumn(t_("Count"));
	arr.AddIndex("IDX");
	arr.ColumnWidths("1 1");
	arr.WhenCursor << [this,&arr]() {
		if (!arr.IsCursor()) return;
		DatabaseBrowser& b = DatabaseBrowser::Single(this->o.GetAppMode());
		b.SetAttrGroup(arr.Get("IDX"));
		PostCallback(THISBACK(Data));
		WhenBrowserCursor();
	};
	arr.WhenBar << [this,&arr,title](Bar& b) {
		b.Add("Sort by " + ToLower(title), [this,&arr]{INHIBIT_CURSOR(arr); arr.SetSortColumn(0,false); sort[0] = 0;});
		b.Add("Sort by count", [this,&arr]{INHIBIT_CURSOR(arr); arr.SetSortColumn(1,true); sort[0] = 1;});
	};
}

void ScriptPhrasePartsGroups::Data() {
	DatabaseBrowser& b = DatabaseBrowser::Single(this->o.GetAppMode());
	b.SetCtrl(o);
	b.SetMode(0);
	
	FillArrayCtrl(DatabaseBrowser::ATTR_GROUP, attr_groups);
	FillArrayCtrl(DatabaseBrowser::ATTR_VALUE, attr_values);
	FillArrayCtrlColor(DatabaseBrowser::COLOR, colors);
	FillArrayCtrl(DatabaseBrowser::ACTION, actions);
	FillArrayCtrl(DatabaseBrowser::ACTION_ARG, action_args);
	FillArrayCtrl(DatabaseBrowser::ELEMENT, elements);
	FillArrayCtrl(DatabaseBrowser::TYPECLASS, typeclasses);
	FillArrayCtrl(DatabaseBrowser::CONTRAST, contrasts);
	
	
	DataList();
}

void ScriptPhrasePartsGroups::FillArrayCtrlColor(DatabaseBrowser::ColumnType t, ArrayCtrl& arr) {
	DatabaseBrowser& b = DatabaseBrowser::Single(this->o.GetAppMode());
	const auto& v = b.Get(t);
	INHIBIT_CURSOR_(arr, c);
	for(int i = 0; i < v.GetCount(); i++) {
		const auto& clr = v[i];
		Color c = GetGroupColor(clr.idx);
		arr.Set(i, "IDX", i);
		arr.Set(i, 0,
			AttrText("#" + IntStr(clr.idx))
				.NormalPaper(c).NormalInk(Black())
				.Paper(Blend(GrayColor(), GetGroupColor(i))).Ink(White()));
		arr.Set(i, 1, clr.count);
	}
	arr.SetCount(v.GetCount());
	if (sort[1] == 0) arr.SetSortColumn(0, false);
	if (sort[1] == 1) arr.SetSortColumn(1, true);
	if (set_cursor) {
		int cur = b.GetColumnCursor(t);
		if (cur >= 0 && cur < arr.GetCount())
			SetIndexCursor(cur, arr);
	}
}

void ScriptPhrasePartsGroups::FillArrayCtrl(DatabaseBrowser::ColumnType t, ArrayCtrl& arr) {
	DatabaseBrowser& b = DatabaseBrowser::Single(this->o.GetAppMode());
	const auto& v = b.Get(t);
	INHIBIT_CURSOR_(arr, ag);
	for(int i = 0; i < v.GetCount(); i++) {
		const auto& a = v[i];
		arr.Set(i, "IDX", i);
		arr.Set(i, 0, a.str);
		arr.Set(i, 1, a.count);
	}
	arr.SetCount(v.GetCount());
	if (sort[1] == 0) arr.SetSortColumn(0, false);
	if (sort[1] == 1) arr.SetSortColumn(1, true);
	if (set_cursor) {
		int cur = b.GetColumnCursor(t);
		if (cur >= 0 && cur < arr.GetCount())
			SetIndexCursor(cur, arr);
	}
}

void ScriptPhrasePartsGroups::DataList() {
	DatabaseBrowser& b = DatabaseBrowser::Single(this->o.GetAppMode());
	
	TextDatabase& db = o.GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;

	int row = 0, max_rows = 10000;
	for(int i = 0; i < b.phrase_parts.GetCount(); i++) {
		int pp_i = b.phrase_parts[i];
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

void ScriptPhrasePartsGroups::SetIndexCursor(int idx, ArrayCtrl& arr) {
	INHIBIT_CURSOR(arr);
	for(int i = 0; i < arr.GetCount(); i++) {
		if (arr.Get(i, "IDX") == idx) {
			arr.SetCursor(i);
			break;
		}
	}
}

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

