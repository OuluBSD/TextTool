#include "TextCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


ScriptReferenceMakerCtrl::ScriptReferenceMakerCtrl() : db0(*this), content(*this) {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << lsplit << db0;
	
	lsplit.Vert() << parts << part;
	lsplit.SetPos(3333);
	
	CtrlLayout(part);
	CtrlLayout(form);
	
	parts.AddColumn("Part");
	parts.AddColumn("Element");
	
	parts.WhenBar << [this](Bar& b) {
		b.Add("Add part", THISBACK1(Do, 0));
		if (parts.IsCursor())
			b.Add("Remove part", THISBACK1(Do, 1));
	};
	parts.WhenCursor << THISBACK(DataPart);
	
	part.split.Horz() << form;
	part.content.Add(content.SizePos());
	
	form.lines <<= THISBACK(OnValueChange);
	form.lines_per_sub <<= THISBACK(OnValueChange);
	form.num <<= THISBACK(OnValueChange);
	form.type <<= THISBACK(OnValueChange);
	form.make_lines << THISBACK(MakeLines);
	
	content.WhenCursor << THISBACK(DataLine);
	
	db0.WhenBrowserCursor << THISBACK(OnBrowserCursor);
	db0.mode.WhenAction << THISBACK(UpdateMode);
	
}

void ScriptReferenceMakerCtrl::Data() {
	Script& s = GetScript();
	int appmode = GetAppMode();
	
	for(int i = 0; i < s.parts.GetCount(); i++) {
		const DynPart& p = s.parts[i];
		String name = p.GetName(appmode);
		parts.Set(i, 0, name);
		parts.Set(i, 1, Capitalize(p.element));
	}
	parts.SetCount(s.parts.GetCount());
	INHIBIT_CURSOR(parts);
	if (!parts.IsCursor() && parts.GetCount())
		parts.SetCursor(0);
	
	DataPart();
}

void ScriptReferenceMakerCtrl::DataPart() {
	const EditorPtrs& p = GetPointers();
	if (!p.script || !parts.IsCursor())
		return;
	
	Script& s = *p.script;
	int part_i = parts.GetCursor();
	const DynPart& dp = s.parts[part_i];
	
	int appmode = GetAppMode();
	form.type.Clear();
	for(int i = 0; i < TXT_COUNT; i++) {
		form.type.Add(GetTextTypeString(i));
	}
	
	
	form.type.SetIndex((int)dp.text_type);
	form.num.SetData(dp.text_num+1);
	form.lines.SetData(dp.text_lines);
	form.lines_per_sub.SetData(dp.text_lines_per_sub);
	
	content.Data();
	db0.Data();
	DataLine();
}

void ScriptReferenceMakerCtrl::DataLine() {
	const EditorPtrs& p = GetPointers();
	if (!p.script || !parts.IsCursor())
		return;
	
	Script& s = *p.script;
	int part_i = parts.GetCursor();
	int line_i = content.GetCursor();
	if (line_i < 0) {
		return;
	}
	const DynPart& dp = s.parts[part_i];
	const PartLineCtrl& pl = content.Get(line_i);
	
	const LineElement* el = 0;
	if (pl.sub_i < 0 && pl.line_i < 0) {
		el = &dp.el;
	}
	else if (pl.sub_i >= 0) {
		const DynSub& ds = dp.sub[pl.sub_i];
		if (pl.line_i < 0) {
			el = &ds.el;
		}
		else {
			const DynLine& dl = ds.lines[pl.line_i];
			el = &dl.el;
		}
	}
	
	if (!el) {
		return;
	}
	
	int mode_cursor = 1 + DatabaseBrowser::FindMode(el->sorter);
	db0.SetModeCursor(mode_cursor);
	
	DatabaseBrowser& b = DatabaseBrowser::Single(GetAppMode());
	b.SetAll(el->sorter, el->element, el->attr, el->clr_i, el->act, el->typeclass_i, el->con_i);
		
	db0.Data();
}

void ScriptReferenceMakerCtrl::MakeLines() {
	const EditorPtrs& p = GetPointers();
	if (!p.script || !parts.IsCursor())
		return;
	
	Script& s = *p.script;
	int part_i = parts.GetCursor();
	DynPart& dp = s.parts[part_i];
	
	if (dp.text_lines_per_sub <= 0)
		dp.text_lines_per_sub = 1;
	
	int sub_count = max(0, (dp.text_lines + dp.text_lines_per_sub - 1) / dp.text_lines_per_sub);
	dp.sub.SetCount(sub_count);
	for(int i = 0; i < dp.sub.GetCount(); i++) {
		DynSub& ds = dp.sub[i];
		ds.lines.SetCount(dp.text_lines_per_sub);
	}
	
	PostCallback(THISBACK(DataPart));
}

void ScriptReferenceMakerCtrl::OnBrowserCursor() {
	DatabaseBrowser& b = DatabaseBrowser::Single(this->GetAppMode());
	
	const EditorPtrs& p = GetPointers();
	if (!p.script || !parts.IsCursor())
		return;
	
	Script& s = *p.script;
	int part_i = parts.GetCursor();
	DynPart& dp = s.parts[part_i];
	
	LineElement* el = 0;
	PartLineCtrl* plp = 0;
	for(int i = 0; i < content.lines.GetCount(); i++) {
		PartLineCtrl& pl = content.lines[i];
		
		if (pl.IsSelected()) {
			plp = &pl;
			if (pl.sub_i < 0 && pl.line_i < 0) {
				el = &dp.el;
			}
			else if (pl.sub_i >= 0) {
				DynSub& ds = dp.sub[pl.sub_i];
				if (pl.line_i < 0) {
					el = &ds.el;
				}
				else {
					DynLine& dl = ds.lines[pl.line_i];
					el = &dl.el;
				}
			}
		}
	}
	if (!el || !plp)
		return;
	
	/*int attr_i = b.GetCur(0);
	int clr_i = b.GetCur(1);
	int action_i = b.GetCur(2);
	int arg_i = b.GetCur(3);*/
	
	el->sorter = db0.GetModeHash();
	if (db0.elements.IsCursor()) {
		el->element		= db0.elements.Get("STR");
	}
	if (db0.attr_groups.IsCursor()) {
		el->attr.group	= db0.attr_groups.Get("STR");
	}
	if (db0.attr_values.IsCursor()) {
		el->attr.value	= db0.attr_values.Get("STR");
	}
	if (db0.colors.IsCursor()) {
		el->clr_i		= db0.colors.Get("INT");
	}
	if (db0.actions.IsCursor()) {
		el->act.action	= db0.actions.Get("STR");
	}
	if (db0.action_args.IsCursor()) {
		el->act.arg		= db0.action_args.Get("STR");
	}
	if (db0.typeclasses.IsCursor()) {
		el->typeclass_i	= db0.typeclasses.Get("INT");
	}
	if (db0.contents.IsCursor()) {
		el->con_i		= db0.contents.Get("INT");
	}
	
	
	plp->Refresh();
}

void ScriptReferenceMakerCtrl::OnValueChange() {
	const EditorPtrs& p = GetPointers();
	if (!p.script || !parts.IsCursor())
		return;
	
	Script& s = *p.script;
	int part_i = parts.GetCursor();
	DynPart& dp = s.parts[part_i];
	
	dp.text_num = (int)form.num.GetData() - 1;
	dp.text_lines = form.lines.GetData();
	dp.text_lines_per_sub = form.lines_per_sub.GetData();
	dp.text_type = (TextPartType)form.type.GetIndex();
	
	
	String name = dp.GetName(GetAppMode());
	parts.Set(0, name);
	parts.Set(1, Capitalize(dp.element));
}

void ScriptReferenceMakerCtrl::ToolMenu(Bar& bar) {
	/*bar.Add(t_("Jump to previous group value"), AppImg::BlueRing(), [this](){
		int tab = tabs.Get();
		if (tab == 0) db0.JumpToGroupValue(-1);
	}).Key(K_CTRL_W);
	bar.Add(t_("Jump to next group value"), AppImg::BlueRing(), [this](){
		int tab = tabs.Get();
		if (tab == 0) db0.JumpToGroupValue(+1);
	}).Key(K_CTRL_E);
	bar.Separator();*/
	bar.Add(t_("Set as line text"), AppImg::BlueRing(), THISBACK(SetLineText)).Key(K_F4);
	
}

void ScriptReferenceMakerCtrl::SetLineText() {
	DatabaseBrowser& b = DatabaseBrowser::Single(this->GetAppMode());
	
	if (!db0.parts.IsCursor())
		return;
	int line_i = content.GetCursor();
	if (line_i < 0 || line_i >= content.lines.GetCount())
		return;
	auto& line = content.Get(line_i);
	
	DynLine* dl = line.GetDynLine();
	if (!dl)
		return;
	AttrText at = db0.parts.Get(0);
	dl->text = at.text.ToString();
	line.Refresh();
}

void ScriptReferenceMakerCtrl::Do(int fn) {
	const EditorPtrs& p = GetPointers();
	
	// Add part
	if (fn == 0) {
		if (!p.script) return;
		Script& s = *p.script;
		s.parts.Add();
		PostCallback(THISBACK(Data));
	}
	// Remove part
	if (fn == 1) {
		if (!p.script || !parts.IsCursor()) return;
		Script& s = *p.script;
		int part_i = parts.GetCursor();
		if (part_i >= 0 && part_i < s.parts.GetCount()) {
			s.parts.Remove(part_i);
			PostCallback(THISBACK(Data));
		}
	}
	
}


END_TEXTLIB_NAMESPACE

