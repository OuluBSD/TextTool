#include "TextCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


ScriptReferenceMakerCtrl::ScriptReferenceMakerCtrl() : db0(*this), content(*this) {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << lsplit << tabs;
	
	lsplit.Vert() << parts << part;
	lsplit.SetPos(3333);
	
	CtrlLayout(part);
	
	parts.AddColumn("Part");
	parts.AddColumn("Element");
	
	parts.WhenBar << [this](Bar& b) {
		b.Add("Add part", THISBACK1(Do, 0));
		if (parts.IsCursor())
			b.Add("Remove part", THISBACK1(Do, 1));
	};
	parts.WhenCursor << THISBACK(DataPart);
	
	part.lines <<= THISBACK(OnValueChange);
	part.num <<= THISBACK(OnValueChange);
	part.type <<= THISBACK(OnValueChange);
	part.content.Add(content.SizePos());
	
	tabs.Add(db0.SizePos(), "Groups/Values");
	
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
	part.type.Clear();
	for(int i = 0; i < TXT_COUNT; i++) {
		part.type.Add(GetTextTypeString(i));
	}
	
	
	part.type.SetIndex((int)dp.text_type);
	part.num.SetData(dp.text_num+1);
	part.lines.SetData(dp.text_lines);
	
	content.Data();
	DataTab();
}

void ScriptReferenceMakerCtrl::DataTab() {
	int tab = tabs.Get();
	if (tab == 0)
		db0.Data();
}

void ScriptReferenceMakerCtrl::OnValueChange() {
	const EditorPtrs& p = GetPointers();
	if (!p.script || !parts.IsCursor())
		return;
	
	Script& s = *p.script;
	int part_i = parts.GetCursor();
	DynPart& dp = s.parts[part_i];
	
	dp.text_num = (int)part.num.GetData() - 1;
	dp.text_lines = part.lines.GetData();
	dp.text_type = (TextPartType)part.type.GetIndex();
}

void ScriptReferenceMakerCtrl::ToolMenu(Bar& bar) {
	
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

