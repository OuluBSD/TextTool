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
		parts.Set(i, 1, Capitalize(p.el.element));
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

void NavigatorState::Clear() {
	line = 0;
	depth = -1;
	el = 0;
	sorter = 0;
	element.Clear();
	attr.group.Clear();
	attr.value.Clear();
	clr_i = -1;
	act.action.Clear();
	act.arg.Clear();
	ActionHeader act;
	typeclass_i = -1;
	con_i = -1;
}

void ScriptReferenceMakerCtrl::ReadNavigatorState(NavigatorState& state, int depth_limit) {
	state.Clear();
	
	const EditorPtrs& p = GetPointers();
	if (!p.script || !parts.IsCursor())
		return;
	
	Script& s = *p.script;
	int part_i = parts.GetCursor();
	int line_i = content.GetCursor();
	if (line_i < 0 || line_i >= content.lines.GetCount())
		return;
	
	PartLineCtrl& pl = content.Get(line_i);
	
	DynPart& dp = s.parts[part_i];
	if (line_i >= content.GetLineCount())
		return;
	
	::ReadNavigatorState(s, part_i, pl.sub_i, pl.line_i, state, depth_limit);
	
	state.line = &pl;
}

void ScriptReferenceMakerCtrl::DataLine() {
	NavigatorState s;
	ReadNavigatorState(s);
	
	int mode_cursor = 1 + DatabaseBrowser::FindMode(s.el ? s.el->sorter : 0);
	db0.SetModeCursor(mode_cursor);
	
	DatabaseBrowser& b = DatabaseBrowser::Single(GetAppMode());
	b.SetAll(s.sorter, s.element, s.attr, s.clr_i, s.act, s.typeclass_i, s.con_i);
		
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
	NavigatorState s;
	ReadNavigatorState(s); // Get current state
	
	if (!s.el || !s.line)
		return;
	auto& el = *s.el;
	auto& line = *s.line;
	
	ReadNavigatorState(s, s.depth-1); // Get inherited state only
	
	
	// Compare to inherited state, and clear value, if same as inherited
	#define ITEM_STR(x, arr) {String str = db0.arr.Get("STR"); el.x = str == s.x ? String() : str;}
	#define ITEM_INT(x, arr) {int i = db0.arr.Get("INT"); el.x = i == s.x ? -1 : i;}
	
	{
		hash_t sorter = db0.GetModeHash();
		el.sorter = sorter == s.sorter ? 0 : sorter;
	}
	
	if (db0.elements.IsCursor()) {
		ITEM_STR(element, elements)
	}
	if (db0.attr_groups.IsCursor()) {
		ITEM_STR(attr.group, attr_groups)
	}
	if (db0.attr_values.IsCursor()) {
		ITEM_STR(attr.value, attr_values)
	}
	if (db0.colors.IsCursor()) {
		ITEM_INT(clr_i, colors);
	}
	if (db0.actions.IsCursor()) {
		ITEM_STR(act.action, actions)
	}
	if (db0.action_args.IsCursor()) {
		ITEM_STR(act.arg, action_args)
	}
	if (db0.typeclasses.IsCursor()) {
		ITEM_INT(typeclass_i, typeclasses);
	}
	if (db0.contents.IsCursor()) {
		ITEM_INT(con_i, contents);
	}
	
	
	line.Refresh();
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
	parts.Set(1, Capitalize(dp.el.element));
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

