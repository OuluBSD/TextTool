#include "TextCtrl.h"
#include <TextTool/TextTool.h>


BEGIN_TEXTLIB_NAMESPACE


ComponentStructure::ComponentStructure() {
	Add(vsplit.SizePos());
	CtrlLayout(active);
	
	vsplit.Vert() << hsplit << active;
	
	active.parts.AddColumn(t_("Name"));
	//active.parts.AddColumn(t_("Type"));
	active.attrs.AddColumn(t_("Attribute"));
	
	
	hsplit.Horz() << params << structs << attr_split;
	hsplit.SetPos(2500, 0);
	hsplit.SetPos(6666, 1);
	
	attr_split.Vert() << attributes << parts;
	attr_split.SetPos(3333);
	
	params.AddColumn(t_("Key"));
	params.AddColumn(t_("Value"));
	params.ColumnWidths("3 2");
	
	structs.AddIndex();
	structs.AddColumn(t_("Name"));
	structs.AddColumn(t_("Structure"));
	structs.AddColumn(t_("Duration"));
	structs.ColumnWidths("5 9 2");
	structs.WhenCursor << THISBACK(DataSuggestionAttributes);
	
	attributes.AddColumn(t_("Attribute"));
	parts.AddColumn(t_("Part"));
	
	
	{
		params.Add(t_("User's structure"), "");
		EditString& e = params.CreateCtrl<EditString>(params.GetCount()-1, 1);
		e.WhenAction << [this,&e]() {if (IsScript()) GetScript().user_structure = e.GetData();};
	}
	{
		params.Add(t_("Required parts"), "");
		EditString& e = params.CreateCtrl<EditString>(params.GetCount()-1, 1);
		e.WhenAction << [this,&e]() {if (IsScript()) GetScript().required_parts =  e.GetData();};
	}
	{
		params.Add(t_("Avoid parts"), "");
		EditString& e = params.CreateCtrl<EditString>(params.GetCount()-1, 1);
		e.WhenAction << [this,&e]() {if (IsScript()) GetScript().avoid_parts = e.GetData();};
	}
	{
		params.Add(t_("Description for suggestions"), "");
		EditString& e = params.CreateCtrl<EditString>(params.GetCount()-1, 1);
		e.WhenAction << [this,&e]() {if (IsScript()) GetScript().structure_suggestion_description = e.GetData();};
	}
	{
		params.Add(t_("Parts in total for suggestions"), "");
		EditIntSpin& e = params.CreateCtrl<EditIntSpin>(params.GetCount()-1, 1);
		e.WhenAction << [this,&e]() {if (IsScript()) GetScript().parts_total = e.GetData();};
	}
	{
		params.Add(t_("Song bpm"), "");
		EditIntSpin& e = params.CreateCtrl<EditIntSpin>(params.GetCount()-1, 1);
		e.WhenAction << [this,&e]() {if (IsScript()) GetScript().bpm =  e.GetData(); DataActive(); DataSuggestions();};
	}
	{
		params.Add(t_("Verse length (2 rhymes, 4 bars)"), "");
		EditIntSpin& e = params.CreateCtrl<EditIntSpin>(params.GetCount()-1, 1);
		e.WhenAction << [this,&e]() {if (IsScript()) GetScript().verse_length = e.GetData();};
	}
	{
		params.Add(t_("Pre-chorus length (2 rhymes, 4 bars)"), "");
		EditIntSpin& e = params.CreateCtrl<EditIntSpin>(params.GetCount()-1, 1);
		e.WhenAction << [this,&e]() {if (IsScript()) GetScript().prechorus_length = e.GetData();};
	}
	{
		params.Add(t_("Chorus length (2 rhymes, 4 bars)"), "");
		EditIntSpin& e = params.CreateCtrl<EditIntSpin>(params.GetCount()-1, 1);
		e.WhenAction << [this,&e]() {if (IsScript()) GetScript().chorus_length = e.GetData();};
	}
	{
		params.Add(t_("Bridge length (2 rhymes, 4 bars)"), "");
		EditIntSpin& e = params.CreateCtrl<EditIntSpin>(params.GetCount()-1, 1);
		e.WhenAction << [this,&e]() {if (IsScript()) GetScript().bridge_length = e.GetData();};
	}
	
}

void ComponentStructure::DisableAll() {
	disabled = true;
	active.Disable();
	params.Disable();
	structs.Disable();
	attributes.Disable();
	parts.Disable();
}

void ComponentStructure::EnableAll() {
	disabled = false;
	active.Enable();
	params.Enable();
	structs.Enable();
	attributes.Enable();
	parts.Enable();
}

void ComponentStructure::Data() {
	active.lbl_comp_struct.SetLabel(GetAppModeLabel(AML_COMPONENT_STRUCTURE));
	params.Set(5, 0, GetAppModeLabel(AML_SPEED));
	
	DataActive();
	DataComponent();
}

void ComponentStructure::DataActive() {
	Script& l = GetScript();
	
	StructSuggestion& s = l.active_struct;
	
	//s.chords.SetCount(s.parts.GetCount());
	
	int bpm = GetScript().bpm;
	
	active.struct_name.SetData(s.name);
	active.struct_str.SetData(Join(s.parts, ", "));
	active.duration.SetData(ToMinSec(s.GetEstimatedDuration(bpm)));
	
	for(int i = 0; i < s.attrs.GetCount(); i++) {
		String attr = s.attrs[i];
		active.attrs.Set(i, 0, attr);
	}
	active.attrs.SetCount(s.attrs.GetCount());
	
	for(int i = 0; i < s.parts.GetCount(); i++) {
		StaticPart* sp = l.FindPartByType(s.parts[i]);
		String abbr = s.parts[i];
		active.parts.Set(i, 0,
			AttrText(GetComponentPartFromAbbr(GetAppMode(), abbr)).NormalPaper(GetComponentPartPaperColor(GetAppMode(), abbr)));
		
	}
	active.parts.SetCount(s.parts.GetCount());
	
}

void ComponentStructure::DataComponent() {
	Script& l = GetScript();
	
	params.Set(0, 1, l.user_structure);
	params.Set(1, 1, l.required_parts);
	params.Set(2, 1, l.avoid_parts);
	params.Set(3, 1, l.structure_suggestion_description);
	params.Set(4, 1, l.parts_total);
	params.Set(5, 1, l.bpm);
	params.Set(6, 1, l.verse_length);
	params.Set(7, 1, l.prechorus_length);
	params.Set(8, 1, l.chorus_length);
	params.Set(9, 1, l.bridge_length);
	
	DataSuggestions();
}

void ComponentStructure::DataSuggestions() {
	Script& l = GetScript();
	
	int bpm = l.bpm;
	
	int c = l.struct_suggs.GetCount();
	for(int i = 0; i < c; i++) {
		StructSuggestion& sug = l.struct_suggs[i];
		
		structs.Set(i, 0, i);
		structs.Set(i, 1, sug.name);
		structs.Set(i, 2, Join(sug.parts, ", "));
		structs.Set(i, 3, ToMinSec(sug.GetEstimatedDuration(bpm)));
	}
	structs.SetCount(c);
	structs.SetSortColumn(2, true);
	
	if (!structs.IsCursor() && c)
		structs.SetCursor(0);
	
	DataSuggestionAttributes();
}

void ComponentStructure::DataSuggestionAttributes() {
	Script& l = GetScript();
	
	if (!structs.IsCursor()) {
		attributes.Clear();
		parts.Clear();
		return;
	}
	
	int cur = structs.GetCursor();
	int idx = structs.Get(cur, 0);
	StructSuggestion& sug = l.struct_suggs[idx];
	
	for(int i = 0; i < sug.attrs.GetCount(); i++) {
		String& attr = sug.attrs[i];
		attributes.Set(i, 0, attr);
	}
	attributes.SetCount(sug.attrs.GetCount());
	
	for(int i = 0; i < sug.parts.GetCount(); i++) {
		String abbr = sug.parts[i];
		parts.Set(i, 0, AttrText(GetComponentPartFromAbbr(GetAppMode(), abbr)).NormalPaper(GetComponentPartPaperColor(GetAppMode(), abbr)));
	}
	parts.SetCount(sug.parts.GetCount());
}

void ComponentStructure::ToolMenu(Bar& bar) {
	bar.Add(t_("Load user's structure"), AppImg::BlueRing(), THISBACK(LoadUserStructure)).Key(K_CTRL_Q);
	bar.Separator();
	bar.Add(t_("Get structure suggestions"), AppImg::RedRing(), THISBACK(GetStructureSuggestions)).Key(K_F5);
	bar.Add(t_("Get attributes for suggestions"), AppImg::RedRing(), THISBACK(GetSuggestionAttributes)).Key(K_F6);
	bar.Add(t_("Load selected structure"), AppImg::RedRing(), THISBACK(LoadStructure)).Key(K_F7);
}

String ComponentStructure::GetStatusText() {
	return "";
}

void ComponentStructure::LoadStructure() {
	if (!structs.IsCursor())
		return;
	try {
		Script& l = GetScript();
		int cur = structs.GetCursor();
		int idx = structs.Get(cur, 0);
		StructSuggestion& sug = l.struct_suggs[idx];
		l.active_struct = sug;
		
		LoadActiveStruct();
	}
	catch (NoPointerExc e) {}
	PostCallback(THISBACK(DataActive));
	
}

void ComponentStructure::LoadStructureString(String struct_str) {
	if (struct_str.IsEmpty())
		return;
	
	try {
		Script& l = GetScript();
		StructSuggestion& sug = l.active_struct;
		sug.Clear();
		sug.name = "User's structure";
		sug.parts <<= Split(struct_str, ",");
		for (auto& s : l.active_struct.parts) s = TrimBoth(s);
		
		
		LoadActiveStruct();
	}
	catch (NoPointerExc e) {
		
	}
	PostCallback(THISBACK(DataActive));
}

void ComponentStructure::LoadActiveStruct() {
	Script& l = GetScript();
	
	Index<String> unique_parts;
	for(int i = 0; i < l.active_struct.parts.GetCount(); i++) {
		String abbr = l.active_struct.parts[i];
		unique_parts.FindAdd(abbr);
	}
	SortIndex(unique_parts, PartAbbrSorter());
	
	l.parts.Clear();
	for(int i = 0; i < unique_parts.GetCount(); i++) {
		String abbr = unique_parts[i];
		StaticPart& part = l.parts.Add();
		part.name = TrimBoth(GetComponentPartFromAbbr(GetAppMode(), abbr));
		part.type = abbr;
	}
}

void ComponentStructure::LoadUserStructure() {
	Script& l = GetScript();
	LoadStructureString(l.user_structure);
}

void ComponentStructure::GetStructureSuggestions() {
	if (disabled) return;
	Script& l = GetScript();
	
	DisableAll();
	
	{
		StructureArgs args;
		args.req = l.required_parts;
		args.avoid = l.avoid_parts;
		args.desc = l.structure_suggestion_description;
		args.total = l.parts_total;
		TaskMgr& m = TaskMgr::Single();
		m.GetStructureSuggestions(GetAppMode(), args, THISBACK1(OnStructureSuggestion, &l));
	}
}

void ComponentStructure::GetSuggestionAttributes() {
	if (disabled) return;
	Script& l = GetScript();
	
	DisableAll();
	
	{
		StructureArgs args;
		for (auto& sug : l.struct_suggs)
			args.structs << Join(sug.parts, ", ");
		TaskMgr& m = TaskMgr::Single();
		m.GetSuggestionAttributes(GetAppMode(), args, THISBACK1(OnSuggestionAttributes, &l));
	}
}

/*void ComponentStructure::SetParam(String key, String value) {
	Component& song = GetComponent();
	song.data.GetAdd(key) = value;
}

String ComponentStructure::GetParam(String key, String def) {
	Component& song = GetComponent();
	return song.data.Get(key, def);
}*/

void ComponentStructure::OnStructureSuggestion(String result, Script* l_) {
	PostCallback(THISBACK(EnableAll));
	//LOG(result);
	
	Script& l = *l_;
	l.struct_suggs.Clear();
	
	Vector<String> suggestions = Split(result, "\n");
	for (String& sug : suggestions) {
		if (sug.Left(1) == "-") sug = TrimBoth(sug.Mid(1));
		
		String name;
		int a = sug.Find(":");
		if (a < 0) a = sug.Find("=");
		if (a < 0) a = sug.Find("-");
		if (a >= 0) {
			name = TrimBoth(sug.Mid(a+1));
			sug = TrimBoth(sug.Left(a));
			if (name.Left(1) == "\"") name = TrimBoth(name.Mid(1, name.GetCount()-2));
		}
		if (sug.Left(1) == "\"") sug = TrimBoth(sug.Mid(1, sug.GetCount()-2));
		
		StructSuggestion& struct_sug = l.struct_suggs.Add();
		struct_sug.name = name;
		struct_sug.parts = Split(sug, ",");
		for (String& p : struct_sug.parts)
			p = TrimBoth(p);
	}
	
	PostCallback(THISBACK(DataSuggestions));
}

void ComponentStructure::OnSuggestionAttributes(String result, Script* l) {
	PostCallback(THISBACK(EnableAll));
	//LOG(result);
	
	Vector<String> parts = Split(result, "\n\n");
	
	if (parts.GetCount() != l->struct_suggs.GetCount()) {
		PromptOK("Result: part count mismatch: " + IntStr(parts.GetCount()) + " != " + IntStr(l->struct_suggs.GetCount()));
		return;
	}
	
	for(int i = 0; i < parts.GetCount(); i++) {
		String& part = parts[i];
		Vector<String> lines = Split(part, "\n");
		if (lines[0].Left(1) != "-")
			lines.Remove(0);
		
		StructSuggestion& sug = l->struct_suggs[i];
		sug.attrs.Clear();
		for (String& l : lines)
			sug.attrs << TrimBoth(l.Mid(1));
	}
	
	PostCallback(THISBACK(DataSuggestionAttributes));
}


END_TEXTLIB_NAMESPACE
