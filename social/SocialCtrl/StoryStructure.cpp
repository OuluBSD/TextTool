#include "SocialCtrl.h"
#include <SocialTool/SocialTool.h>


StoryStructure::StoryStructure() {
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
		e.WhenAction << [this,&e]() {GetStory().user_structure = e.GetData();};
	}
	{
		params.Add(t_("Required parts"), "");
		EditString& e = params.CreateCtrl<EditString>(params.GetCount()-1, 1);
		e.WhenAction << [this,&e]() {GetStory().required_parts =  e.GetData();};
	}
	{
		params.Add(t_("Avoid parts"), "");
		EditString& e = params.CreateCtrl<EditString>(params.GetCount()-1, 1);
		e.WhenAction << [this,&e]() {GetStory().avoid_parts = e.GetData();};
	}
	{
		params.Add(t_("Description of the program for suggestions"), "");
		EditString& e = params.CreateCtrl<EditString>(params.GetCount()-1, 1);
		e.WhenAction << [this,&e]() {GetStory().structure_suggestion_description = e.GetData();};
	}
	{
		params.Add(t_("Parts in total for suggestions"), "");
		EditIntSpin& e = params.CreateCtrl<EditIntSpin>(params.GetCount()-1, 1);
		e.WhenAction << [this,&e]() {GetStory().parts_total = e.GetData();};
	}
	{
		params.Add(t_("Program bpm"), "");
		EditIntSpin& e = params.CreateCtrl<EditIntSpin>(params.GetCount()-1, 1);
		e.WhenAction << [this,&e]() {GetStory().bpm =  e.GetData(); DataActive(); DataSuggestions();};
	}
	{
		params.Add(t_("Verse length (2 rhymes, 4 bars)"), "");
		EditIntSpin& e = params.CreateCtrl<EditIntSpin>(params.GetCount()-1, 1);
		e.WhenAction << [this,&e]() {GetStory().verse_length = e.GetData();};
	}
	{
		params.Add(t_("Pre-chorus length (2 rhymes, 4 bars)"), "");
		EditIntSpin& e = params.CreateCtrl<EditIntSpin>(params.GetCount()-1, 1);
		e.WhenAction << [this,&e]() {GetStory().prechorus_length = e.GetData();};
	}
	{
		params.Add(t_("Chorus length (2 rhymes, 4 bars)"), "");
		EditIntSpin& e = params.CreateCtrl<EditIntSpin>(params.GetCount()-1, 1);
		e.WhenAction << [this,&e]() {GetStory().chorus_length = e.GetData();};
	}
	{
		params.Add(t_("Bridge length (2 rhymes, 4 bars)"), "");
		EditIntSpin& e = params.CreateCtrl<EditIntSpin>(params.GetCount()-1, 1);
		e.WhenAction << [this,&e]() {GetStory().bridge_length = e.GetData();};
	}
	
}

void StoryStructure::DisableAll() {
	disabled = true;
	active.Disable();
	params.Disable();
	structs.Disable();
	attributes.Disable();
	parts.Disable();
}

void StoryStructure::EnableAll() {
	disabled = false;
	active.Enable();
	params.Enable();
	structs.Enable();
	attributes.Enable();
	parts.Enable();
}

void StoryStructure::Data() {
	DataActive();
	DataProgram();
}

void StoryStructure::DataActive() {
	Story& l = GetStory();
	
	StoryStructSuggestion& s = l.active_struct;
	
	//s.chords.SetCount(s.parts.GetCount());
	
	int bpm = GetStory().bpm;
	
	active.struct_name.SetData(s.name);
	active.struct_str.SetData(Join(s.parts, ", "));
	active.duration.SetData(ToMinSec(s.GetEstimatedDuration(bpm)));
	
	for(int i = 0; i < s.attrs.GetCount(); i++) {
		String attr = s.attrs[i];
		active.attrs.Set(i, 0, attr);
	}
	active.attrs.SetCount(s.attrs.GetCount());
	
	for(int i = 0; i < s.parts.GetCount(); i++) {
		StoryPart* sp = l.FindPartByType(s.parts[i]);
		String abbr = s.parts[i];
		active.parts.Set(i, 0,
			AttrText(GetProgramPartFromAbbr(abbr)).NormalPaper(GetProgramPartPaperColor(abbr)));
		
	}
	active.parts.SetCount(s.parts.GetCount());
	
}

void StoryStructure::DataProgram() {
	Story& l = GetStory();
	
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

void StoryStructure::DataSuggestions() {
	Story& l = GetStory();
	
	int bpm = l.bpm;
	
	int c = l.struct_suggs.GetCount();
	for(int i = 0; i < c; i++) {
		StoryStructSuggestion& sug = l.struct_suggs[i];
		
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

void StoryStructure::DataSuggestionAttributes() {
	Story& l = GetStory();
	
	if (!structs.IsCursor()) {
		attributes.Clear();
		parts.Clear();
		return;
	}
	
	int cur = structs.GetCursor();
	int idx = structs.Get(cur, 0);
	StoryStructSuggestion& sug = l.struct_suggs[idx];
	
	for(int i = 0; i < sug.attrs.GetCount(); i++) {
		String& attr = sug.attrs[i];
		attributes.Set(i, 0, attr);
	}
	attributes.SetCount(sug.attrs.GetCount());
	
	for(int i = 0; i < sug.parts.GetCount(); i++) {
		String abbr = sug.parts[i];
		parts.Set(i, 0, AttrText(GetProgramPartFromAbbr(abbr)).NormalPaper(GetProgramPartPaperColor(abbr)));
	}
	parts.SetCount(sug.parts.GetCount());
}

void StoryStructure::ToolMenu(Bar& bar) {
	bar.Add(t_("Load user's structure"), AppImg::BlueRing(), THISBACK(LoadUserStructure)).Key(K_CTRL_Q);
	bar.Separator();
	bar.Add(t_("Get structure suggestions"), AppImg::RedRing(), THISBACK(GetStructureSuggestions)).Key(K_F5);
	bar.Add(t_("Get attributes for suggestions"), AppImg::RedRing(), THISBACK(GetSuggestionAttributes)).Key(K_F6);
	bar.Add(t_("Load selected structure"), AppImg::RedRing(), THISBACK(LoadStructure)).Key(K_F7);
}

String StoryStructure::GetStatusText() {
	return "";
}

void StoryStructure::LoadStructure() {
	if (!structs.IsCursor())
		return;
	try {
		Story& l = GetStory();
		int cur = structs.GetCursor();
		int idx = structs.Get(cur, 0);
		StoryStructSuggestion& sug = l.struct_suggs[idx];
		l.active_struct = sug;
		
		LoadActiveStruct();
	}
	catch (NoPointerExc e) {}
	PostCallback(THISBACK(DataActive));
	
}

void StoryStructure::LoadStructureString(String struct_str) {
	if (struct_str.IsEmpty())
		return;
	
	try {
		Story& l = GetStory();
		StoryStructSuggestion& sug = l.active_struct;
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

void StoryStructure::LoadActiveStruct() {
	Story& l = GetStory();
	
	Index<String> unique_parts;
	for(int i = 0; i < l.active_struct.parts.GetCount(); i++) {
		String abbr = l.active_struct.parts[i];
		unique_parts.FindAdd(abbr);
	}
	SortIndex(unique_parts, PartAbbrSorter());
	
	l.parts.Clear();
	for(int i = 0; i < unique_parts.GetCount(); i++) {
		String abbr = unique_parts[i];
		StoryPart& part = l.parts.Add();
		part.name = TrimBoth(GetProgramPartFromAbbr(abbr));
		part.type = abbr;
	}
}

void StoryStructure::LoadUserStructure() {
	Story& l = GetStory();
	LoadStructureString(l.user_structure);
}

void StoryStructure::GetStructureSuggestions() {
	if (disabled) return;
	Story& l = GetStory();
	
	DisableAll();
	
	{
		String req = l.required_parts;
		String avoid = l.avoid_parts;
		String desc = l.structure_suggestion_description;
		int total = l.parts_total;
		TaskMgr& m = TaskMgr::Single();
		m.GetStructureSuggestions(req, avoid, desc, total, THISBACK1(OnStructureSuggestion, &l));
	}
}

void StoryStructure::GetSuggestionAttributes() {
	if (disabled) return;
	Story& l = GetStory();
	
	DisableAll();
	
	{
		Vector<String> structs;
		for (auto& sug : l.struct_suggs)
			structs << Join(sug.parts, ", ");
		TaskMgr& m = TaskMgr::Single();
		m.GetSuggestionAttributes(structs, THISBACK1(OnSuggestionAttributes, &l));
	}
}

/*void StoryStructure::SetParam(String key, String value) {
	Program& program = GetProgram();
	program.data.GetAdd(key) = value;
}

String StoryStructure::GetParam(String key, String def) {
	Program& program = GetProgram();
	return program.data.Get(key, def);
}*/

void StoryStructure::OnStructureSuggestion(String result, Story* l_) {
	PostCallback(THISBACK(EnableAll));
	//LOG(result);
	
	Story& l = *l_;
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
		
		StoryStructSuggestion& struct_sug = l.struct_suggs.Add();
		struct_sug.name = name;
		struct_sug.parts = Split(sug, ",");
		for (String& p : struct_sug.parts)
			p = TrimBoth(p);
	}
	
	PostCallback(THISBACK(DataSuggestions));
}

void StoryStructure::OnSuggestionAttributes(String result, Story* l) {
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
		
		StoryStructSuggestion& sug = l->struct_suggs[i];
		sug.attrs.Clear();
		for (String& l : lines)
			sug.attrs << TrimBoth(l.Mid(1));
	}
	
	PostCallback(THISBACK(DataSuggestionAttributes));
}
