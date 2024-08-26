#include "TextCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


ScriptReferenceMakerCtrl::ScriptReferenceMakerCtrl() : db0(*this), content(*this) {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << lsplit << tabs;
	
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
	
	part.split.Horz() << form << line_conf;
	part.content.Add(content.SizePos());
	
	form.lines <<= THISBACK(OnValueChange);
	form.lines_per_sub <<= THISBACK(OnValueChange);
	form.num <<= THISBACK(OnValueChange);
	form.type <<= THISBACK(OnValueChange);
	form.make_lines << THISBACK(MakeLines);
	
	line_conf.AddColumn("Key");
	line_conf.AddColumn("Value");
	line_conf.ColumnWidths("1 4");
	
	tabs.Add(db0.SizePos(), "Groups/Values");
	
	content.WhenCursor << THISBACK(DataLine);
	
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
	DataTab();
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
		line_conf.Clear();
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
		line_conf.Clear();
		return;
	}
	
	int i = 0;
	line_conf.Set(i, 0, "Element");
	line_conf.Set(i, 1, "");
	i++;
	
	line_conf.Set(i, 0, "Group");
	line_conf.Set(i, 1, "");
	i++;
	
	line_conf.Set(i, 0, "Value");
	line_conf.Set(i, 1, "");
	i++;
	
	line_conf.Set(i, 0, "Color");
	line_conf.Set(i, 1, "");
	i++;
	
	line_conf.Set(i, 0, "Action");
	line_conf.Set(i, 1, "");
	i++;
	
	line_conf.Set(i, 0, "Action-arg");
	line_conf.Set(i, 1, "");
	i++;
	
	line_conf.Set(i, 0, "Typeclass");
	line_conf.Set(i, 1, "");
	i++;
	
	line_conf.Set(i, 0, "Content");
	line_conf.Set(i, 1, "");
	i++;
	
	line_conf.SetCount(i);
	
	DatabaseBrowser& b = DatabaseBrowser::Single(GetAppMode());
	b.SetAll(el->attr, el->clr_i, el->act);
			
	int tab = tabs.Get();
	if (tab == 0) db0.Data();
}

void ScriptReferenceMakerCtrl::DataTab() {
	int tab = tabs.Get();
	if (tab == 0)
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
	bar.Add(t_("Jump to previous group value"), AppImg::BlueRing(), [this](){
		int tab = tabs.Get();
		if (tab == 0) db0.JumpToGroupValue(-1);
	}).Key(K_CTRL_W);
	bar.Add(t_("Jump to next group value"), AppImg::BlueRing(), [this](){
		int tab = tabs.Get();
		if (tab == 0) db0.JumpToGroupValue(+1);
	}).Key(K_CTRL_E);
	
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

