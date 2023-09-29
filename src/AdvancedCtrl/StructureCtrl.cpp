#include "AdvancedCtrl.h"


StructureCtrl::StructureCtrl() {
	CtrlLayout(import_str);
	
	drawer.ctrl = this;
	
	Add(import_str.HSizePos().TopPos(0,56));
	Add(hsplit.HSizePos().VSizePos(56,0));
	
	hsplit.Horz() << vsplit0 << vsplit1;
	hsplit.SetPos(2000);
	
	vsplit0.Vert() << part_list << line_list << break_list;
	
	vsplit1.Vert() << drawer << list;
	vsplit1.SetPos(1000);
	
	part_list.AddIndex();
	part_list.AddColumn(t_("#"));
	part_list.AddColumn(t_("Part name"));
	part_list.AddColumn(t_("Length"));
	part_list.ColumnWidths("1 3 1");
	part_list <<= THISBACK(DataPart);
	
	line_list.AddColumn(t_("#"));
	line_list.AddColumn(t_("Line text"));
	line_list.AddColumn(t_("Length"));
	line_list.ColumnWidths("1 3 1");
	line_list <<= THISBACK(DataLine);
	
	break_list.AddColumn(t_("#"));
	break_list.AddColumn(t_("Break text"));
	break_list.AddColumn(t_("Length"));
	break_list.ColumnWidths("1 3 1");
	break_list <<= THISBACK(DataBreak);
	
	list.AddColumn(t_("Source"));
	list.AddColumn(t_("Key"));
	list.AddColumn(t_("Value"));
	list.ColumnWidths("1 1 6");
	
	// Mode droplist
	for(const SnapArg& a : HumanInputTextArgs()) {
		import_str.mode.Add(GetSnapString(a));
	}
	import_str.mode.SetIndex(a.Get());
	import_str.mode.WhenAction << [this]() {
		this->a = HumanInputTextArgs()[import_str.mode.GetIndex()];
		Data();
	};
	
}

void StructureCtrl::Data() {
	DataSong();
}

void StructureCtrl::DataSong() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song || !p.song->pipe) {
		part_list.Clear();
		line_list.Clear();
		break_list.Clear();
		return;
	}
	Artist& a = *p.artist;
	Release& r = *p.release;
	Song& s = *p.song;
	Pipe& e = *s.pipe;
	
	import_str.structure.SetData(s.structure_str);
	
	for(int i = 0; i < s.structure.GetCount(); i++) {
		String key = s.structure[i];
		int j = e.FindPartIdx(key);
		if (j < 0) {
			part_list.Set(i, 0, -1);
			part_list.Set(i, 1, i);
			part_list.Set(i, 2, "<error>");
		}
		else {
			Part& part = e.parts[j];
			part_list.Set(i, 0, j);
			part_list.Set(i, 1, i);
			part_list.Set(i, 2, part.name);
			part_list.Set(i, 3, part.GetLength(this->a));
		}
	}
	part_list.SetCount(e.parts.GetCount());
	
	if (!part_list.IsCursor() && part_list.GetCount())
		part_list.SetCursor(0);
	
	DataPart();
}

void StructureCtrl::DataPart() {
	if (!part_list.IsCursor())
		return;
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	Song& s = *p.song;
	Pipe& e = *s.pipe;
	
	int c = part_list.GetCursor();
	int part_i = part_list.Get(c, 0);
	if (part_i < 0) {
		line_list.Clear();
	}
	else {
		Part& part = e.parts[part_i];
		for(int i = 0; i < part.lines.GetCount(); i++) {
			Line& line = part.lines[i];
			
			line_list.Set(i, 0, i);
			line_list.Set(i, 1, line.Get(a).txt);
			line_list.Set(i, 2, line.GetLength(a));
		}
		line_list.SetCount(part.lines.GetCount());
		
		if (!line_list.IsCursor() && line_list.GetCount())
			line_list.SetCursor(0);
	}
	
	DataLine();
}

void StructureCtrl::DataLine() {
	if (!part_list.IsCursor() || !line_list.IsCursor())
		return;
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	Song& s = *p.song;
	Pipe& e = *s.pipe;
	
	int part_c = part_list.GetCursor();
	int part_i = part_list.Get(part_c, 0);
	int line_i = line_list.GetCursor();
	if (part_i < 0 || line_i < 0) {
		line_list.Clear();
	}
	else {
		Part& part = e.parts[part_i];
		Line& line = part.lines[line_i];
		
		for(int i = 0; i < line.breaks.GetCount(); i++) {
			Break& brk = line.breaks[i];
			
			break_list.Set(i, 0, i);
			break_list.Set(i, 1, brk.Get(a).txt);
			break_list.Set(i, 2, brk.GetLength(a));
			
			EditIntSpin* edit = new EditIntSpin;
			edit->WhenAction << [&, this, edit, line_i, part_c]() {
				brk.Get(a).syllables = edit->GetData();
				line_list.Set(line_i, 2, line.GetLength(a));
				part_list.Set(part_c, 3, part.GetLength(a));
				this->drawer.Refresh();
			};
			break_list.SetCtrl(i, 2, edit);
		}
		break_list.SetCount(line.breaks.GetCount());
		
		if (!break_list.IsCursor() && break_list.GetCount())
			break_list.SetCursor(0);
	}
	
	DataBreak();
}

void StructureCtrl::DataBreak() {
	if (!part_list.IsCursor() || !line_list.IsCursor() || !break_list.IsCursor())
		return;
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	Song& s = *p.song;
	Pipe& e = *s.pipe;
	
	int c = part_list.GetCursor();
	int part_i = part_list.Get(c, 0);
	int line_i = line_list.GetCursor();
	int brk_i = break_list.GetCursor();
	if (part_i < 0 || line_i < 0 || brk_i < 0) {
		break_list.Clear();
	}
	else {
		Part& part = e.parts[part_i];
		Line& line = part.lines[line_i];
		Break& brk = line.breaks[brk_i];
		
		
	}
	
	DataList();
}

void StructureCtrl::DataList() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	Song& song = *p.song;
	Pipe& pipe = *song.pipe;
	
	int c = part_list.GetCursor();
	int part_i = part_list.Get(c, 0);
	int line_i = line_list.GetCursor();
	int brk_i = break_list.GetCursor();
	if (part_i < 0 || line_i < 0 || brk_i < 0) {
		break_list.Clear();
	}
	else {
		Part& part = pipe.parts[part_i];
		Line& line = part.lines[line_i];
		Break& brk = line.breaks[brk_i];
		
		int list_i = 0;
		
		list.Set(list_i, 0, t_("Break"));
		list.Set(list_i, 1, t_("Text"));
		list.Set(list_i, 2, brk.Get(a).txt);
		list_i++;
		
		if (brk.Get(a).impact.GetCount()) {
			list.Set(list_i, 0, t_("Break"));
			list.Set(list_i, 1, t_("Impact"));
			list.Set(list_i, 2, brk.Get(a).impact);
			list_i++;
		}
		
		for(int i = 0; i < brk.Get(a).data.GetCount(); i++) {
			list.Set(list_i, 0, t_("Break"));
			list.Set(list_i, 1, brk.Get(a).data.GetKey(i));
			list.Set(list_i, 2, brk.Get(a).data[i]);
			list_i++;
		}
		
		list.Set(list_i, 0, t_("Line"));
		list.Set(list_i, 1, t_("Text"));
		list.Set(list_i, 2, line.Get(a).txt);
		list_i++;
		
		for(int i = 0; i < line.Get(a).data.GetCount(); i++) {
			list.Set(list_i, 0, t_("Line"));
			list.Set(list_i, 1, line.Get(a).data.GetKey(i));
			list.Set(list_i, 2, line.Get(a).data[i]);
			list_i++;
		}
		
		list.Set(list_i, 0, t_("Part"));
		list.Set(list_i, 1, t_("Text"));
		list.Set(list_i, 2, part.Get(a).txt);
		list_i++;
		
		for(int i = 0; i < part.Get(a).data.GetCount(); i++) {
			list.Set(list_i, 0, t_("Part"));
			list.Set(list_i, 1, part.Get(a).data.GetKey(i));
			list.Set(list_i, 2, part.Get(a).data[i]);
			list_i++;
		}
		
		for(int i = 0; i < pipe.Get(a).data.GetCount(); i++) {
			list.Set(list_i, 0, t_("Song"));
			list.Set(list_i, 1, pipe.Get(a).data.GetKey(i));
			list.Set(list_i, 2, pipe.Get(a).data[i]);
			list_i++;
		}
		
		
		list.SetCount(list_i);
		
		//list.SetSortColumn(0, true);
	}
	
	
	
}





StructureDrawer::StructureDrawer() {
	
}

void StructureDrawer::Paint(Draw& d) {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	SnapArg a = ctrl->a;
	
	Size sz = GetSize();
	
	d.DrawRect(sz, White());
	
	if (!p.song || !p.song->pipe)
		return;
	Song& song = *p.song;
	Pipe& pipe = *song.pipe;
	
	items.SetCount(0);
	int total_len = 0;
	for(int i = 0; i < song.structure.GetCount(); i++) {
		const String& key = song.structure[i];
		Part* part = pipe.FindPart(key);
		if (!part) continue;
		
		Item& item = items.Add();
		item.part = part;
		item.len = part->GetLength(a);
		item.clr = GetPartColor(key, Color(Random(256), Random(256), Random(256)));
		
		total_len += item.len;
	}
	
	if (!total_len) return;
	
	Font fnt = SansSerif(13);
	double cx = sz.cx / (double)total_len;
	int len_sum = 0;
	for (Item& item : items) {
		int x = (int)(len_sum * cx);
		int w = (int)((len_sum + item.len) * cx);
		d.DrawRect(RectC(x, 0, w, sz.cy), Blend(item.clr, White(), 128));
		String key = Capitalize(item.part->name);
		d.DrawText(x+5,5, key, fnt, Black());
		d.DrawText(x+4,4, key, fnt, White());
		if (len_sum > 0)
			d.DrawLine(x, 0, x, sz.cy, 1, Black());
		len_sum += item.len;
	}
}
