#include "TextCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


ScriptTextSolverCtrl::ScriptTextSolverCtrl() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << editor << tabs;
	
	tabs.WhenSet << THISBACK(Data);
	
	editor.owner = this;
	editor.WhenCursor << THISBACK(OnEditorCursor);
	
	
	// Wizard tab
	tabs.Add(wizard_tab.SizePos(), "Wizard song");
	
	
	// Whole song -tab
	tabs.Add(whole_tab.SizePos(), "Whole song");
	
	whole_tab.Add(whole_split.VSizePos(0,30).HSizePos());
	whole_tab.Add(whole_prog.BottomPos(0,30).HSizePos(300));
	whole_tab.Add(whole_remaining.BottomPos(0,30).LeftPos(0,300));
	whole_prog.Set(0,1);
	
	whole_split.Vert() << whole_hsplit0 << whole_hsplit1;
	
	whole_hsplit0.Horz() << colors << attrs;
	whole_hsplit1.Horz() << actions << phrases;
	
	colors.AddColumn("Color");
	
	attrs.AddColumn("Group");
	attrs.AddColumn("Positive");
	attrs.AddColumn("Negative");
	
	actions.AddColumn("Action");
	actions.AddColumn("Arg");
	
	phrases.AddColumn("Phrase");
	phrases.SetLineCy(30);
	
	
	// Part -tab
	tabs.Add(part_tab.SizePos(), "Part");
	
	
	
	// Sub -tab
	tabs.Add(sub_tab.SizePos(), "Sub");
	
	
	
	// Line -tab
	tabs.Add(line_tab.SizePos(), "Line");
	
	
	
}

void ScriptTextSolverCtrl::ToolMenu(Bar& bar) {
	bar.Add(t_("Update Data"), AppImg::BlueRing(), THISBACK(Data)).Key(K_CTRL_Q);
	bar.Separator();
	bar.Add(t_("Start"), AppImg::RedRing(), THISBACK1(Do, 0)).Key(K_F5);
	bar.Add(t_("Stop"), AppImg::RedRing(), THISBACK1(Do, 1)).Key(K_F6);
}

void ScriptTextSolverCtrl::Do(int fn) {
	int tab = tabs.Get();
	if (tab == 1) DoWhole(fn);
}

void ScriptTextSolverCtrl::DoWhole(int fn) {
	ScriptGenerator& sdi = ScriptGenerator::Get(GetAppMode(), GetEntity(), GetScript());
	whole_prog.Attach(sdi);
	sdi.WhenRemaining << [this](String s) {
		PostCallback([this,s](){whole_remaining.SetLabel(s); Refresh();});
	};
	
	if (fn == 0)
		sdi.Start();
	else
		sdi.Stop();
}

void ScriptTextSolverCtrl::OnEditorCursor() {
	if (editor.selected_line == 0 &&
		editor.selected_sub == 0 &&
		editor.selected_part == 0) {
		tabs.Set(1);
	}
	else if (editor.selected_line == 0 &&
		editor.selected_sub == 0 &&
		editor.selected_part != 0) {
		tabs.Set(2);
	}
	else if (editor.selected_line == 0 &&
		editor.selected_sub != 0 &&
		editor.selected_part == 0) {
		tabs.Set(3);
	}
	else if (editor.selected_line != 0 &&
		editor.selected_sub == 0 &&
		editor.selected_part == 0) {
		tabs.Set(4);
	}
}

void ScriptTextSolverCtrl::Data() {
	if (!HasFocusDeep())
		editor.SetFocus();
	
	editor.Update();
	
	int tab = tabs.Get();
	if (tab == 1) DataWhole();
	
}

void ScriptTextSolverCtrl::DataWhole() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	int appmode = GetAppMode();
	Script& s = GetScript();
	Entity& a = GetEntity();
	ComponentAnalysis& sa = da.GetComponentAnalysis(appmode, a.file_title + " - " + s.file_title);
	
	
	for(int i = 0; i < s.clr_list.GetCount(); i++) {
		int clr_i = s.clr_list[i];
		Color clr = GetGroupColor(clr_i);
		colors.Set(i, 0, AttrText("#" + IntStr(clr_i))
			.NormalPaper(clr).Paper(clr));
	}
	SetCountWithDefaultCursor(colors, s.clr_list.GetCount());
	
	
	int row = 0;
	for(int i = 0; i < s.simple_attrs.GetCount(); i++) {
		bool negative = s.simple_attrs[i];
		const ExportSimpleAttr& t = da.simple_attrs[i];
		String group = da.simple_attrs.GetKey(i);
		String attr_s0 = da.attrs.GetKey(t.attr_i0).value;
		String attr_s1 = da.attrs.GetKey(t.attr_i1).value;
		attrs.Set(row, 0, Capitalize(group));
		
		if (!negative)
			attrs.Set(row, 1, AttrText(Capitalize(attr_s0)).NormalPaper(Blend(LtGreen(), White())).Paper(Green()));
		else
			attrs.Set(row, 1, Capitalize(attr_s0));
		
		if (negative)
			attrs.Set(row, 2, AttrText(Capitalize(attr_s1)).NormalPaper(Blend(LtRed(), White())).Paper(Red()));
		else
			attrs.Set(row, 2, Capitalize(attr_s1));
		
		row++;
	}
	SetCountWithDefaultCursor(attrs, row);
	
	
	row = 0;
	for(int i = 0; i < s.actions_enabled.GetCount(); i++) {
		if (!s.actions_enabled[i])
			continue;
		const ActionHeader& ah = da.actions.GetKey(i);
		actions.Set(row, 0, ah.action);
		actions.Set(row, 1, ah.arg);
		row++;
	}
	SetCountWithDefaultCursor(actions, row);
	
	
	
	row = 0;
	for(int i = 0; i < ContentType::PART_COUNT; i++) {
		for(int j = 0; j < s.phrase_parts[i].GetCount(); j++) {
			int pp_i = s.phrase_parts[i][j];
			const PhrasePart& pp = sa.phrase_parts[i][j];
			const TokenText& tt = da.token_texts[pp.tt_i];
			String s = da.GetTokenTextString(tt);
			s.Replace("/", "\n");
			phrases.Set(row, 0, s);
			row++;
		}
	}
	SetCountWithDefaultCursor(phrases, row);
	
	
}


END_TEXTLIB_NAMESPACE
