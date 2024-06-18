#include "TextDataCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


StructuredScriptCtrl::StructuredScriptCtrl() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << names << lines;
	hsplit.SetPos(3333);
	
	names.AddColumn("Artist");
	names.AddColumn("Title");
	names.AddColumn("Structure");
	names.WhenCursor << THISBACK(DataScript);
	names.WhenBar = [this](Bar& bar) {
		bar.Add("Add script", [this]() {
			String entity;
			bool b = EditTextNotNull(
				entity,
				t_("Add Script"),
				t_("Entity's name"),
				0
			);
			if (!b) return;
			
			String title;
			b = EditTextNotNull(
				title,
				t_("Add Script"),
				t_("Script's title"),
				0
			);
			if (!b) return;
			
			TextDatabase& db = GetDatabase();
			StructuredScript& ss = db.structured_scripts.Add();
			ss.entity = entity;
			ss.title = title;
			
			PostCallback(THISBACK(Data));
		});
	};
	
	lines.AddColumn("Part");
	lines.AddColumn("Text");
	lines.ColumnWidths("1 4");
	
}

void StructuredScriptCtrl::Data() {
	TextDatabase& db = GetDatabase();
	
	for(int i = 0; i < db.structured_scripts.GetCount(); i++) {
		StructuredScript& ss = db.structured_scripts[i];
		names.Set(i, 0, ss.entity);
		names.Set(i, 1, ss.title);
		names.Set(i, 2, ss.structure_str);
	}
	INHIBIT_CURSOR(names);
	names.SetCount(db.structured_scripts.GetCount());
	if (names.GetCount() && !names.IsCursor())
		names.SetCursor(0);
	
	DataScript();
}

void StructuredScriptCtrl::DataScript() {
	if (!names.IsCursor())
		return;
	
	TextDatabase& db = GetDatabase();
	int ss_i = names.GetCursor();
	StructuredScript& ss = db.structured_scripts[ss_i];
	
	int row = 0;
	for(int i = 0; i < ss.script.GetCount(); i++) {
		String part = ss.script.GetKey(i);
		const auto& l = ss.script[i];
		for(int j = 0; j < l.GetCount(); j++) {
			lines.Set(row, 0, part);
			lines.Set(row, 1, l[j]);
			row++;
		}
	}
	lines.SetCount(row);
}

void StructuredScriptCtrl::ToolMenu(Bar& bar) {
	bar.Add(t_("Paste script"), AppImg::BlueRing(), THISBACK(PasteScript)).Key(K_F5);
}


void StructuredScriptCtrl::PasteScript() {
	if (!names.IsCursor())
		return;
	
	TextDatabase& db = GetDatabase();
	int ss_i = names.GetCursor();
	StructuredScript& ss = db.structured_scripts[ss_i];
	
	const VectorMap<String, String>& part_names = GetAppModeParts(GetAppMode());
	
	String s = ReadClipboardText();
	s.Replace("\r", "");
	
	Vector<String> parts = Split(s, "\n\n");
	
	Index<String> seen_parts;
	Vector<String> structure;
	
	ss.script.Clear();
	for (String& p : parts) {
		p = TrimBoth(p);
		
		if (p.Left(1) != "[") {
			PromptOK("Expected part name [name]");
			return;
		}
		
		int b = p.Find("]");
		int a = 1;
		
		String title = TrimBoth(p.Mid(a,b-a));
		
		a = title.Find(" ");
		String key = a >= 0 ? title.Left(a) : title;
		String num = a >= 0 ? TrimBoth(title.Mid(a)) : String();
		a = num.Find(":");
		if (a >= 0) num = TrimBoth(num.Left(a));
		String abbr;
		for(int i = 0; i < part_names.GetCount(); i++) {
			if (part_names[i] == key) {
				abbr = part_names.GetKey(i);
				break;
			}
		}
		
		if (abbr.IsEmpty()) {
			PromptOK("Unexpected part name: " + title);
			return;
		}
		abbr += num;
		structure << abbr;
		
		if (seen_parts.Find(title) >= 0)
			continue;
		seen_parts.Add(title);
		
		Vector<String> lines = Split(p, "\n");
		lines.Remove(0);
		auto& out = ss.script.GetAdd(title);
		for (String& l : lines)
			out << TrimBoth(l);
	}
	
	//DUMPC(structure);
	
	ss.structure_str = Join(structure, ",");
	names.Set(ss_i, 2, ss.structure_str);
	
	PostCallback(THISBACK(DataScript));
}


END_TEXTLIB_NAMESPACE

