#include "TextCtrl.h"
#include <TextTool/TextTool.h>


BEGIN_TEXTLIB_NAMESPACE


ComponentStructure::ComponentStructure() {
	Add(hsplit.SizePos());
	
	hsplit.Vert() << script_split << ref_split;
	
	script_split.SetPos(2500);
	script_split.Horz() << script_parts << script_lines;
	
	script_parts.AddColumn("Part name");
	script_parts.AddColumn("Person");
	script_parts.WhenCursor << THISBACK(DataPart);
	script_lines.AddColumn("Text");
	script_lines.AddColumn("Phonetic");
	
	ref_split.Horz() << ref_names << ref_lines;
	ref_split.SetPos(2500);
	
	ref_names.AddColumn("Artist");
	ref_names.AddColumn("Title");
	ref_names.WhenCursor << THISBACK(DataScript);
	ref_lines.AddColumn("Part");
	ref_lines.AddColumn("Text");
	ref_lines.ColumnWidths("1 4");
	
	
	
	#if 0
	{
		params.Add(t_("Reference song"), "");
		EditInt& e = params.CreateCtrl<EditInt>(params.GetCount()-1, 1);
		e.WhenAction << [this,&e]() {if (IsScript()) GetScript().active_struct.structured_script_i = e.GetData();};
	}
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
	{
		params.Add(t_("Singer A: name"), "");
		EditString& e = params.CreateCtrl<EditString>(params.GetCount()-1, 1);
		e.WhenAction << [this,&e]() {if (IsScript()) GetScript().singer0_name = e.GetData();};
	}
	{
		params.Add(t_("Singer B: name"), "");
		EditString& e = params.CreateCtrl<EditString>(params.GetCount()-1, 1);
		e.WhenAction << [this,&e]() {if (IsScript()) GetScript().singer1_name = e.GetData();};
	}
	{
		params.Add(t_("Singer C: name"), "");
		EditString& e = params.CreateCtrl<EditString>(params.GetCount()-1, 1);
		e.WhenAction << [this,&e]() {if (IsScript()) GetScript().singer2_name = e.GetData();};
	}
	{
		params.Add(t_("Singer A: parts"), "");
		EditString& e = params.CreateCtrl<EditString>(params.GetCount()-1, 1);
		e.WhenAction << [this,&e]() {if (IsScript()) GetScript().singer0_parts = e.GetData();};
	}
	{
		params.Add(t_("Singer B: parts"), "");
		EditString& e = params.CreateCtrl<EditString>(params.GetCount()-1, 1);
		e.WhenAction << [this,&e]() {if (IsScript()) GetScript().singer1_parts = e.GetData();};
	}
	{
		params.Add(t_("Singer C: parts"), "");
		EditString& e = params.CreateCtrl<EditString>(params.GetCount()-1, 1);
		e.WhenAction << [this,&e]() {if (IsScript()) GetScript().singer2_parts = e.GetData();};
	}
	#endif
}

void ComponentStructure::Data() {
	TextDatabase& db = GetDatabase();
	
	EnterAppMode(GetAppMode());
	
	for(int i = 0; i < db.structured_scripts.GetCount(); i++) {
		StructuredScript& ss = db.structured_scripts[i];
		ref_names.Set(i, 0, ss.entity);
		ref_names.Set(i, 1, ss.title);
	}
	INHIBIT_CURSOR(ref_names);
	ref_names.SetCount(db.structured_scripts.GetCount());
	if (ref_names.GetCount() && !ref_names.IsCursor())
		ref_names.SetCursor(0);
	
	
	DataScript();
	DataActive();
	DataComponent();
}

void ComponentStructure::DataScript() {
	if (!ref_names.IsCursor())
		return;
	
	TextDatabase& db = GetDatabase();
	int ss_i = ref_names.GetCursor();
	StructuredScript& ss = db.structured_scripts[ss_i];
	
	int row = 0;
	for(int i = 0; i < ss.parts.GetCount(); i++) {
		const auto& l = ss.parts[i];
		for(int j = 0; j < l.lines.GetCount(); j++) {
			ref_lines.Set(row, 0, l.name);
			ref_lines.Set(row, 1, l.lines[j]);
			row++;
		}
	}
	ref_lines.SetCount(row);
}

void ComponentStructure::DataActive() {
	Script& l = GetScript();
	
	TODO
	#if 0
	ScriptStructure& s = l.active_struct;
	
	//s.chords.SetCount(s.parts.GetCount());
	script_parts.Clear();
	for(int i = 0; i < s.parts.GetCount(); i++) {
		String key = s.parts[i];
		StaticPart* sp = l.FindPartByType(key);
		if (!sp)
			sp = l.FindPartByName(key);
		script_parts.Set(i, 0, key);
		if (sp) {
			EditString* e = new EditString;
			//e->SetData(sp->singer);
			e->WhenAction << [this,e,sp]() {
				sp->singer = e->GetData();
			};
			script_parts.SetCtrl(i, 1, e);
			script_parts.Set(i, 1, sp->singer);
		}
		else
			script_parts.Set(i, 1, Value());
	}
	INHIBIT_CURSOR(script_parts);
	script_parts.SetCount(s.parts.GetCount());
	if (script_parts.GetCount() && !script_parts.IsCursor())
		script_parts.SetCursor(0);
	
	DataPart();
	#endif
}

void ComponentStructure::DataPart() {
	Script& l = GetScript();
	if (!script_parts.IsCursor()) {
		script_lines.Clear();
		return;
	}
	
	int line_i = script_parts.GetCursor();
	String part_name = script_parts.Get(0);
	
	StaticPart* sp = 0; TODO//l.FindPartByName(part_name);
	if (!sp)
		return;
	
	const auto& lines = sp->reference.Get();
	for(int i = 0; i < lines.GetCount(); i++) {
		const RhymeContainer::Line& l = lines[i];
		script_lines.Set(i, 0, l.AsText());
		script_lines.Set(i, 1, l.AsPhonetic());
	}
	INHIBIT_CURSOR(script_lines);
	script_lines.SetCount(lines.GetCount());
	
}

void ComponentStructure::DataComponent() {
	Script& l = GetScript();
	
	#if 0
	int i = 0;
	params.Set(i++, 1, l.active_struct.structured_script_i);
	params.Set(i++, 1, l.user_structure);
	params.Set(i++, 1, l.required_parts);
	params.Set(i++, 1, l.avoid_parts);
	params.Set(i++, 1, l.structure_suggestion_description);
	params.Set(i++, 1, l.parts_total);
	params.Set(i++, 1, l.bpm);
	params.Set(i++, 1, l.verse_length);
	params.Set(i++, 1, l.prechorus_length);
	params.Set(i++, 1, l.chorus_length);
	params.Set(i++, 1, l.bridge_length);
	params.Set(i++, 1, l.singer0_name);
	params.Set(i++, 1, l.singer1_name);
	params.Set(i++, 1, l.singer2_name);
	params.Set(i++, 1, l.singer0_parts);
	params.Set(i++, 1, l.singer1_parts);
	params.Set(i++, 1, l.singer2_parts);
	#endif
	
	DataSuggestions();
}

void ComponentStructure::DataSuggestions() {
	Script& l = GetScript();
	
	#if 0
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
	#endif
	
	DataSuggestionAttributes();
}

void ComponentStructure::DataSuggestionAttributes() {
	Script& l = GetScript();
	
	#if 0
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
	#endif
	
}

void ComponentStructure::ToolMenu(Bar& bar) {
	bar.Add(t_("Load reference script"), AppImg::BlueRing(), THISBACK(LoadReference)).Key(K_F5);
	#if 0
	bar.Add(t_("Load user's structure"), AppImg::BlueRing(), THISBACK(LoadUserStructure)).Key(K_CTRL_Q);
	bar.Add(t_("Load singers"), AppImg::BlueRing(), THISBACK(LoadSingers)).Key(K_CTRL_W);
	bar.Add(t_("Load reference song"), AppImg::BlueRing(), THISBACK(LoadReference)).Key(K_CTRL_E);
	bar.Separator();
	bar.Add(t_("Get structure suggestions"), AppImg::RedRing(), THISBACK(GetStructureSuggestions)).Key(K_F5);
	bar.Add(t_("Get attributes for suggestions"), AppImg::RedRing(), THISBACK(GetSuggestionAttributes)).Key(K_F6);
	bar.Add(t_("Load selected structure"), AppImg::RedRing(), THISBACK(LoadStructure)).Key(K_F7);
	#endif
}

String ComponentStructure::GetStatusText() {
	return "";
}

void ComponentStructure::LoadReference() {
	if (!ref_names.IsCursor())
		return;
	
	TextDatabase& db = GetDatabase();
	int ss_i = ref_names.GetCursor();
	StructuredScript& ss = db.structured_scripts[ss_i];
	Script& s = GetScript();
	
	int appmode = GetAppMode();
	
	Vector<TextSolver*> v;
	for(int i = 0; i < ss.parts.GetCount(); i++) {
		const auto& l = ss.parts[i];
		
		for(int j = 0; j < l.lines.GetCount(); j++) {
			TextSolver& ts = TextSolver::Get(appmode, l.lines[j]);
			v << &ts;
		}
	}
	for(int i = 0; i < v.GetCount()-1; i++) {
		TextSolver& ts0 = *v[i];
		TextSolver* ts1 = v[i+1];
		ts0.WhenReady << [this,ts1]() {
			ts1->Start();
		};
	}
	if (v.IsEmpty())
		return;
	
	TODO
	#if 0
	v.Top()->WhenReady << [this,ss_i,appmode]() {
		TextDatabase& db = GetDatabase();
		StructuredScript& ss = db.structured_scripts[ss_i];
		Script& s = GetScript();
		s.parts.Clear();
		s.active_struct.Clear();
		s.active_struct.structured_script_i = ss_i;
		s.active_struct.parts.Clear();
		
		Index<String> added;
		for(int i = 0; i < ss.parts.GetCount(); i++) {
			const auto& l = ss.parts[i];
			s.active_struct.parts << l.name;
			if (added.Find(l.name) >= 0)
				continue;
			added.Add(l.name);
			StaticPart& sp = s.parts.Add();
			sp.name = l.name;
			sp.type = GetAbbrPartFromComponent(appmode, l.name);
			sp.singer = l.person;
			sp.reference.Clear();
			
			for(int j = 0; j < l.lines.GetCount(); j++) {
				TextSolver& ts = TextSolver::Get(appmode, l.lines[j]);
				const auto& src = ts.rc.Get();
				for(int k = 0; k < src.GetCount(); k++) {
					sp.reference.Get().Add() = src[k];
				}
			}
		}
		
		PostCallback(THISBACK(Data));
	};
	v[0]->Start();
	#endif
}

#if 0
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

void ComponentStructure::LoadReference() {
	if (!names.IsCursor())
		return;
	
	TextDatabase& db = GetDatabase();
	int ss_i = names.GetCursor();
	StructuredScript& ss = db.structured_scripts[ss_i];
	Script& l = GetScript();
	
	l.user_structure = ss.structure_str;
	LoadStructureString(l.user_structure);
	l.active_struct.structured_script_i = ss_i;
	
	PostCallback(THISBACK(Data));
}

void ComponentStructure::LoadSingers() {
	Script& l = GetScript();
	
	for (StaticPart& part : l.parts)
		part.singer.Clear();
	
	for(int i = 0; i < 3; i++) {
		String name, parts;
		switch (i) {
			case 0: name = l.singer0_name; parts = l.singer0_parts; break;
			case 1: name = l.singer1_name; parts = l.singer1_parts; break;
			case 2: name = l.singer2_name; parts = l.singer2_parts; break;
		}
		if (name.IsEmpty())
			continue;
		
		Vector<String> abbrs = Split(parts, ",");
		for(String& abbr : abbrs) {
			String sp_name = TrimBoth(GetComponentPartFromAbbr(GetAppMode(), abbr));
			
			for (StaticPart& sp : l.parts) {
				if (sp.type == abbr) {
					sp.singer = name;
				}
			}
		}
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
#endif

END_TEXTLIB_NAMESPACE
