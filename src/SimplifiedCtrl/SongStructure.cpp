#include "SimplifiedCtrl.h"



SongStructure::SongStructure() {
	Add(vsplit.SizePos());
	CtrlLayout(active);
	
	vsplit.Vert() << hsplit << active;
	
	active.parts.AddColumn(t_("Name"));
	active.parts.AddColumn(t_("Type"));
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
		e.WhenAction << [this,&e]() {SetParam("USER_STRUCT", e.GetData());};
	}
	{
		params.Add(t_("Required parts"), "");
		EditString& e = params.CreateCtrl<EditString>(params.GetCount()-1, 1);
		e.WhenAction << [this,&e]() {SetParam("REQ_PARTS", e.GetData());};
	}
	{
		params.Add(t_("Avoid parts"), "");
		EditString& e = params.CreateCtrl<EditString>(params.GetCount()-1, 1);
		e.WhenAction << [this,&e]() {SetParam("AVOID_PARTS", e.GetData());};
	}
	{
		params.Add(t_("Description of the song for suggestions"), "");
		EditString& e = params.CreateCtrl<EditString>(params.GetCount()-1, 1);
		e.WhenAction << [this,&e]() {SetParam("DESC_FOR_STRUCT_SUGGS", e.GetData());};
	}
	{
		params.Add(t_("Parts in total for suggestions"), "");
		EditIntSpin& e = params.CreateCtrl<EditIntSpin>(params.GetCount()-1, 1);
		e.WhenAction << [this,&e]() {SetParam("PARTS_TOTAL", IntStr(e.GetData()));};
	}
	{
		params.Add(t_("Song bpm"), "");
		EditIntSpin& e = params.CreateCtrl<EditIntSpin>(params.GetCount()-1, 1);
		e.WhenAction << [this,&e]() {SetParam("BPM", IntStr(e.GetData())); DataActive(); DataSuggestions();};
	}
	
}

void SongStructure::DisableAll() {
	disabled = true;
	active.Disable();
	params.Disable();
	structs.Disable();
	attributes.Disable();
	parts.Disable();
}

void SongStructure::EnableAll() {
	disabled = false;
	active.Enable();
	params.Enable();
	structs.Enable();
	attributes.Enable();
	parts.Enable();
}

void SongStructure::Data() {
	DataActive();
	DataSong();
}

void SongStructure::DataActive() {
	Song& song = GetSong();
	
	Song::StructSuggestion& s = song.active_struct;
	
	s.part_types.SetCount(s.parts.GetCount(), 0);
	
	int bpm = StrInt(GetParam("BPM", "120"));
	
	active.struct_name.SetData(s.name);
	active.struct_str.SetData(Join(s.parts, ", "));
	active.duration.SetData(ToMinSec(s.GetEstimatedDuration(bpm)));
	
	for(int i = 0; i < s.attrs.GetCount(); i++) {
		String attr = s.attrs[i];
		active.attrs.Set(i, 0, attr);
	}
	active.attrs.SetCount(s.attrs.GetCount());
	
	for(int i = 0; i < s.parts.GetCount(); i++) {
		String abbr = s.parts[i];
		active.parts.Set(i, 0, 
			AttrText(GetSongPartFromAbbr(abbr)).NormalPaper(GetSongPartPaperColor(abbr)));
		
		DropList& dl = active.parts.CreateCtrl<DropList>(i, 1);
		dl.Add(t_("Singing"));
		dl.Add(t_("Rapping"));
		dl.Add(t_("Poetry"));
		dl.Add(t_("Dialog"));
		dl.Add(t_("Skip"));
		int idx = max(0, min(dl.GetCount()-1, s.part_types[i]));
		dl.SetIndex(idx);
		dl.WhenAction << [&song,i,&dl]() {song.active_struct.part_types[i] = dl.GetIndex();};
	}
	active.parts.SetCount(s.parts.GetCount());
	
}

void SongStructure::DataSong() {
	Song& song = GetSong();
	
	params.Set(0, 1, GetParam("USER_STRUCT"));
	params.Set(1, 1, GetParam("REQ_PARTS"));
	params.Set(2, 1, GetParam("AVOID_PARTS"));
	params.Set(3, 1, GetParam("DESC_FOR_STRUCT_SUGGS"));
	params.Set(4, 1, GetParam("PARTS_TOTAL", "0"));
	params.Set(5, 1, GetParam("BPM", "120"));
	
	DataSuggestions();
}

void SongStructure::DataSuggestions() {
	Song& song = GetSong();
	
	int bpm = StrInt(GetParam("BPM", "120"));
	
	int c = song.struct_suggs.GetCount();
	for(int i = 0; i < c; i++) {
		Song::StructSuggestion& sug = song.struct_suggs[i];
		
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

void SongStructure::DataSuggestionAttributes() {
	Song& song = GetSong();
	
	if (!structs.IsCursor())
		return;
	
	int cur = structs.GetCursor();
	int idx = structs.Get(cur, 0);
	Song::StructSuggestion& sug = song.struct_suggs[idx];
	
	for(int i = 0; i < sug.attrs.GetCount(); i++) {
		String& attr = sug.attrs[i];
		attributes.Set(i, 0, attr);
	}
	attributes.SetCount(sug.attrs.GetCount());
	
	for(int i = 0; i < sug.parts.GetCount(); i++) {
		String abbr = sug.parts[i];
		parts.Set(i, 0, AttrText(GetSongPartFromAbbr(abbr)).NormalPaper(GetSongPartPaperColor(abbr)));
	}
	parts.SetCount(sug.parts.GetCount());
}

void SongStructure::ToolMenu(Bar& bar) {
	bar.Add(t_("Load user's structure"), AppImg::RedRing(), THISBACK(LoadUserStructure)).Key(K_F5);
	bar.Add(t_("Get structure suggestions"), AppImg::BlueRing(), THISBACK(GetStructureSuggestions)).Key(K_F6);
	bar.Add(t_("Get attributes for suggestions"), AppImg::BlueRing(), THISBACK(GetSuggestionAttributes)).Key(K_F7);
	bar.Add(t_("Load selected structure"), AppImg::BlueRing(), THISBACK(LoadStructure)).Key(K_F8);
}

String SongStructure::GetStatusText() {
	return "";
}

void SongStructure::LoadStructure() {
	if (!structs.IsCursor())
		return;
	try {
		Song& song = GetSong();
		int cur = structs.GetCursor();
		int idx = structs.Get(cur, 0);
		Song::StructSuggestion& sug = song.struct_suggs[idx];
		song.active_struct = sug;
	}
	catch (NoPointerExc e) {}
	PostCallback(THISBACK(DataActive));
}

void SongStructure::LoadUserStructure() {
	String struct_str = GetParam("USER_STRUCT");
	LoadStructureString(struct_str);
}

void SongStructure::GetStructureSuggestions() {
	if (disabled) return;
	Song& song = GetSong();
	
	song.RealizePipe();
	
	DisableAll();
	
	{
		String req = GetParam("REQ_PARTS");
		String avoid = GetParam("AVOID_PARTS");
		String desc = GetParam("DESC_FOR_STRUCT_SUGGS");
		String total_str = GetParam("PARTS_TOTAL", "0");
		int total = StrInt(total_str);
		TaskMgr& m = *song.pipe;
		m.GetStructureSuggestions(req, avoid, desc, total, THISBACK1(OnStructureSuggestion, &song));
	}
}

void SongStructure::GetSuggestionAttributes() {
	if (disabled) return;
	Song& song = GetSong();
	
	song.RealizePipe();
	
	DisableAll();
	
	{
		Vector<String> structs;
		for (auto& sug : song.struct_suggs)
			structs << Join(sug.parts, ", ");
		TaskMgr& m = *song.pipe;
		m.GetSuggestionAttributes(structs, THISBACK1(OnSuggestionAttributes, &song));
	}
}

void SongStructure::SetParam(String key, String value) {
	Song& song = GetSong();
	song.data.GetAdd(key) = value;
}

String SongStructure::GetParam(String key, String def) {
	Song& song = GetSong();
	return song.data.Get(key, def);
}

void SongStructure::LoadStructureString(String struct_str) {
	if (struct_str.IsEmpty())
		return;
	
	try {
		Song& song = GetSong();
		song.active_struct.Clear();
		song.active_struct.parts <<= Split(struct_str, ",");
		for (auto& s : song.active_struct.parts) s = TrimBoth(s);
	}
	catch (NoPointerExc e) {
		
	}
	PostCallback(THISBACK(DataActive));
}

void SongStructure::OnStructureSuggestion(String result, Song* song_) {
	EnableAll();
	//LOG(result);
	
	Song& song = *song_;
	song.struct_suggs.Clear();
	
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
		
		Song::StructSuggestion& struct_sug = song.struct_suggs.Add();
		struct_sug.name = name;
		struct_sug.parts = Split(sug, ",");
		for (String& p : struct_sug.parts)
			p = TrimBoth(p);
	}
	
	PostCallback(THISBACK(DataSuggestions));
}

void SongStructure::OnSuggestionAttributes(String result, Song* song) {
	EnableAll();
	//LOG(result);
	
	Vector<String> parts = Split(result, "\n\n");
	
	if (parts.GetCount() != song->struct_suggs.GetCount()) {
		PromptOK("Result: part count mismatch: " + IntStr(parts.GetCount()) + " != " + IntStr(song->struct_suggs.GetCount()));
		return;
	}
	
	for(int i = 0; i < parts.GetCount(); i++) {
		String& part = parts[i];
		Vector<String> lines = Split(part, "\n");
		if (lines[0].Left(1) != "-")
			lines.Remove(0);
		
		Song::StructSuggestion& sug = song->struct_suggs[i];
		sug.attrs.Clear();
		for (String& l : lines)
			sug.attrs << TrimBoth(l.Mid(1));
	}
	
	PostCallback(THISBACK(DataSuggestionAttributes));
}
