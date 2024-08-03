#include "TextCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


ScriptTextSolverCtrl::ScriptTextSolverCtrl() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << editor << tabs;
	
	tabs.WhenSet << THISBACK(Data);
	
	editor.owner = this;
	editor.WhenCursor << THISBACK(OnEditorCursor);
	
	
	// Wizard tab
	tabs.Add(wizard_tab.SizePos(), "Wizard");
	
	
	// Suggestions -tab
	tabs.Add(sugg_tab.SizePos(), "Suggestions");
	sugg_tab.Add(sugg_split.VSizePos(0,30).HSizePos());
	sugg_tab.Add(sugg_prog.BottomPos(0,30).HSizePos(300));
	sugg_tab.Add(sugg_remaining.BottomPos(0,30).LeftPos(0,300));
	sugg_split.Vert() << sugg_list << sugg_lyrics;
	sugg_split.SetPos(2500);
	sugg_list.AddColumn("#");
	sugg_list.WhenCursor << [this]() {
		if (!sugg_list.IsCursor()) return;
		int i = sugg_list.GetCursor();
		Vector<String> lines = Split(GetScript().__suggestions[i], "\n");
		for(int i = 0; i < lines.GetCount(); i++) {
			sugg_lyrics.Set(i, 0, lines[i]);
		}
		sugg_lyrics.SetCount(lines.GetCount());
	};
	sugg_list.WhenBar << [this](Bar& b) {
		b.Add("Set edit text", [this]() {
			Script& s = GetScript();
			int i = sugg_list.GetCursor();
			s.SetEditText(GetScript().__suggestions[i]);
			editor.ShowEditText();
		});
	};
	sugg_lyrics.AddColumn("Txt");
	
	
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
	
	CtrlLayout(part_form);
	part_tab.Add(part_split.SizePos());
	part_split.Vert() << part_form << part_suggs;
	part_split.SetPos(2000);
	part_form.element <<= THISBACK(OnValueChange);
	part_form.text_type <<= THISBACK(OnValueChange);
	part_form.text_num <<= THISBACK(OnValueChange);
	
	
	// Sub -tab
	tabs.Add(sub_tab.SizePos(), "Sub");
	CtrlLayout(sub_form);
	sub_tab.Add(sub_split.SizePos());
	sub_split.Vert() << sub_form << sub_suggs;
	sub_split.SetPos(2000);
	sub_form.element0 <<= THISBACK(OnValueChange);
	sub_form.element1 <<= THISBACK(OnValueChange);
	
	
	
	// Line -tab
	tabs.Add(line_tab.SizePos(), "Line");
	
	line_tab.Add(line_split.SizePos());
	line_split.Vert() << line_ref_lines << line_suggs;
	
	line_ref_lines.AddColumn("Selected");
	line_ref_lines.AddColumn("Reference line");
	line_ref_lines.AddColumn("Morphed line");
	line_ref_lines.AddColumn("Edit line");
	line_ref_lines.ColumnWidths("1 6 6 6");
	
	line_suggs.AddColumn("#");
	line_suggs.AddColumn("Suggestion");
	line_suggs.ColumnWidths("1 7");
	
}

void ScriptTextSolverCtrl::ToolMenu(Bar& bar) {
	bar.Add(t_("Update Data"), AppImg::BlueRing(), THISBACK(Data)).Key(K_CTRL_Q);
	bar.Add(t_("Switch editor text"), AppImg::BlueRing(), THISBACK(SwitchEditorText)).Key(K_CTRL_W);
	bar.Separator();
	bar.Add(t_("Start"), AppImg::RedRing(), THISBACK1(Do, 0)).Key(K_F5);
	bar.Add(t_("Stop"), AppImg::RedRing(), THISBACK1(Do, 1)).Key(K_F6);
	bar.Separator();
	bar.Add(t_("Fn"), AppImg::RedRing(), THISBACK1(Do, 2)).Key(K_F7);
	bar.Add(t_("Fn"), AppImg::RedRing(), THISBACK1(Do, 3)).Key(K_F8);
}

void ScriptTextSolverCtrl::SwitchEditorText() {
	editor.SwitchTextSource();
}

void ScriptTextSolverCtrl::Do(int fn) {
	int tab = tabs.Get();
	if (tab == 1) DoSuggestions(fn);
	if (tab == 2) DoWhole(fn);
	if (tab == 5) DoLine(fn);
}

void ScriptTextSolverCtrl::DoSuggestions(int fn) {
	ScriptSolver& sdi = ScriptSolver::Get(GetAppMode(), GetEntity(), GetScript());
	sugg_prog.Attach(sdi);
	sdi.WhenRemaining << [this](String s) {
		PostCallback([this,s](){sugg_remaining.SetLabel(s); Refresh();});
	};
	
	if (fn == 0)
		sdi.Start();
	else
		sdi.Stop();
}

void ScriptTextSolverCtrl::DoWhole(int fn) {
	if (fn >= 0 && fn <= 1) {
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
	else if (fn == 2) {
		WriteClipboardText(GetScript().GetUserText(GetAppMode()));
	}
}

void ScriptTextSolverCtrl::OnEditorCursor() {
	if (editor.selected_line == 0 &&
		editor.selected_sub == 0 &&
		editor.selected_part == 0) {
		tabs.Set(2);
	}
	else if (editor.selected_line == 0 &&
		editor.selected_sub == 0 &&
		editor.selected_part != 0) {
		tabs.Set(3);
	}
	else if (editor.selected_line == 0 &&
		editor.selected_sub != 0 &&
		editor.selected_part == 0) {
		tabs.Set(4);
	}
	else if (editor.selected_line != 0 &&
		editor.selected_sub == 0 &&
		editor.selected_part == 0) {
		tabs.Set(5);
	}
	Data();
}

void ScriptTextSolverCtrl::Data() {
	editor.CheckClearSelected();
	
	if (!HasFocusDeep())
		editor.SetFocus();
	
	editor.Update();
	
	int tab = tabs.Get();
	if (tab == 1) DataSuggestions();
	if (tab == 2) DataWhole();
	if (tab == 3) DataPart();
	if (tab == 4) DataSub();
	if (tab == 5) DataLine();
	
}

void ScriptTextSolverCtrl::DataSuggestions() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	int appmode = GetAppMode();
	Script& s = GetScript();
	
	
	for(int i = 0; i < s.__suggestions.GetCount(); i++) {
		sugg_list.Set(i, 0, i);
	}
	sugg_list.SetCount(s.__suggestions.GetCount());
	
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
	SetCountForArray(colors, s.clr_list.GetCount());
	
	
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
	SetCountForArray(attrs, row);
	
	
	row = 0;
	for(int i = 0; i < s.actions_enabled.GetCount(); i++) {
		if (!s.actions_enabled[i])
			continue;
		const ActionHeader& ah = da.actions.GetKey(i);
		actions.Set(row, 0, ah.action);
		actions.Set(row, 1, ah.arg);
		row++;
	}
	SetCountForArray(actions, row);
	
	
	
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
	SetCountForArray(phrases, row);
	
	
}

void ScriptTextSolverCtrl::DataPart() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	Script& s = GetScript();
	
	if (part_form.element.GetCount() == 0) {
		part_form.element.Add("");
		for(int i = 0; i < da.element_keys.GetCount(); i++) {
			part_form.element.Add(da.element_keys[i]);
		}
	}
	
	if (part_form.text_type.GetCount() == 0) {
		for(int i = 0; i < TXT_COUNT; i++) {
			part_form.text_type.Add(GetTextTypeString(i));
		}
	}
	
	if (editor.selected_part) {
		const DynPart& part = *editor.selected_part;
		int el_i = da.element_keys.Find(part.element) + 1;
		part_form.element.SetIndex(el_i);
		part_form.text_num.SetData(part.text_num+1);
		part_form.text_type.SetIndex((int)part.text_type);
	}
	
}

void ScriptTextSolverCtrl::DataSub() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	Script& s = GetScript();
	
	if (sub_form.element0.GetCount() == 0) {
		sub_form.element0.Add("");
		sub_form.element1.Add("");
		for(int i = 0; i < da.element_keys.GetCount(); i++) {
			sub_form.element0.Add(da.element_keys[i]);
			sub_form.element1.Add(da.element_keys[i]);
		}
	}
	
	if (editor.selected_sub) {
		const DynSub& sub = *editor.selected_sub;
		int el0_i = da.element_keys.Find(sub.element0) + 1;
		int el1_i = da.element_keys.Find(sub.element1) + 1;
		sub_form.element0.SetIndex(el0_i);
		sub_form.element1.SetIndex(el1_i);
	}
	
}

void ScriptTextSolverCtrl::DoLine(int fn) {
	
	if (fn == 2) {
		const DynPart* part = 0;
		const DynSub* sub = 0;
		Vector<const DynLine*> g = GetLineGroup(&part, &sub);
		if (g.IsEmpty()) return;
		ScriptSolver& sdi = ScriptSolver::Get(GetAppMode(), GetEntity(), GetScript());
		if (sdi.IsRunning()) {
			PromptOK("Wait until ScriptSolver has ended");
			return;
		}
		sdi.GetSuggestions(*part, *sub, g, [this](){
			PostCallback([this]() {
				editor.Refresh();
				DataLine();
			});
		});
	}
	else if (fn == 3) {
		if (!line_suggs.IsCursor()) return;
		int sugg_i = line_suggs.GetCursor();
		Vector<const DynLine*> g = GetLineGroup();
		for (const DynLine* l : g) {
			DynLine& dl = const_cast<DynLine&>(*l);
			if (sugg_i < dl.suggs.GetCount())
				dl.user_text = dl.suggs[sugg_i];
			else
				dl.user_text = "";
		}
		editor.ShowUserText();
		DataLine();
	}
}

void ScriptTextSolverCtrl::DataLine() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	Script& s = GetScript();
	
	
	
	if (editor.selected_line) {
		Vector<const DynLine*> g = GetLineGroup();
		
		for(int i = 0; i < g.GetCount(); i++) {
			const DynLine* dl = g[i];
			
			line_ref_lines.Set(i, 0, dl == editor.selected_line ? "X" : "");
			line_ref_lines.Set(i, 1, dl->text);
			line_ref_lines.Set(i, 2, dl->alt_text);
			line_ref_lines.Set(i, 3, dl->edit_text);
		}
		line_ref_lines.SetCount(g.GetCount());
		
		
		int sugg_i = 0;
		while (1) {
			String sugg;
			for(int i = 0; i < g.GetCount(); i++) {
				const DynLine* dl = g[i];
				if (sugg_i < dl->suggs.GetCount()) {
					if (!sugg.IsEmpty()) sugg << "\n";
					sugg << dl->suggs[sugg_i];
				}
				else {
					break;
				}
			}
			if (sugg.IsEmpty())
				break;
			line_suggs.Set(sugg_i, 0, sugg_i+1);
			line_suggs.Set(sugg_i, 1, sugg);
			sugg_i++;
		}
		line_suggs.SetCount(sugg_i);
		line_suggs.ArrayCtrl::SetLineCy(g.GetCount() * 20);
	}
	
}

void ScriptTextSolverCtrl::OnValueChange() {
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	Script& s = GetScript();
	
	if (editor.selected_part) {
		DynPart& part = *const_cast<DynPart*>(editor.selected_part);
		int el_i = part_form.element.GetIndex();
		part.element = el_i >= 0 ? da.element_keys[el_i] : String();
		part.text_num = (int)part_form.text_num.GetData() - 1;
		part.text_type = (TextPartType)part_form.text_type.GetIndex();
		
		editor.Refresh();
	}
	
	if (editor.selected_sub) {
		DynSub& sub = *const_cast<DynSub*>(editor.selected_sub);
		int el0_i = sub_form.element0.GetIndex() - 1;
		int el1_i = sub_form.element1.GetIndex() - 1;
		sub.element0 = el0_i >= 0 ? da.element_keys[el0_i] : String();
		sub.element1 = el1_i >= 0 ? da.element_keys[el1_i] : String();
		
		editor.Refresh();
	}
}

Vector<const DynLine*> ScriptTextSolverCtrl::GetLineGroup(const DynPart** part, const DynSub** sub) {
	Vector<const DynLine*> ret;
	auto selected_line = editor.selected_line;
	if (!selected_line) return ret;
	Script& s = GetScript();
	for(int i = 0; i < s.parts.GetCount(); i++) {
		const DynPart& dp = s.parts[i];
		
		int sel_line_i = -1;
		int line_i = 0;
		for(int j = 0; j < dp.sub.GetCount(); j++) {
			const DynSub& ds = dp.sub[j];
			
			for(int k = 0; k < ds.lines.GetCount(); k++) {
				const DynLine& dl = ds.lines[k];
				
				if (&dl == selected_line) {
					if (part && !*part) *part = &dp;
					if (sub && !*sub) *sub = &ds;
					ret << &dl;
					sel_line_i = line_i;
					break;
				}
				line_i++;
			}
			if (sel_line_i != -1)
				break;
		}
		
		if (sel_line_i == -1)
			continue;
		
		int alt_line;
		bool is_even = line_i % 2 == 0;
		if (is_even) alt_line = line_i+1;
		else alt_line = line_i-1;
		
		line_i = 0;
		for(int j = 0; j < dp.sub.GetCount(); j++) {
			const DynSub& ds = dp.sub[j];
			
			for(int k = 0; k < ds.lines.GetCount(); k++) {
				const DynLine& dl = ds.lines[k];
				
				if (line_i == alt_line) {
					if (part && !*part) *part = &dp;
					if (sub && !*sub) *sub = &ds;
					if (is_even)
						ret.Add(&dl);
					else
						ret.Insert(0, &dl);
					return ret;
				}
				line_i++;
			}
		}
		break;
	}
	return ret;
}

const DynLine* ScriptTextSolverCtrl::GetAltLine() {
	Vector<const DynLine*> g = GetLineGroup();
	if (g.GetCount() < 2) return 0;
	if (editor.selected_line == g[0]) return g[1];
	else return g[0];
}


END_TEXTLIB_NAMESPACE
