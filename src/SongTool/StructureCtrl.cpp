#include "SongTool.h"


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
	mode = MALE;
	for(int i = 0; i < PTR_COUNT; i++) {
		import_str.mode.Add(GetModeString(i));
	}
	import_str.mode.SetIndex(mode);
	import_str.mode.WhenAction << [this]() {this->mode = import_str.mode.GetIndex(); Data();};
	
}

void StructureCtrl::Data() {
	DataSong();
}

void StructureCtrl::DataSong() {
	Database& db = Database::Single();
	Ptrs& p = db.ctx[MALE];
	if (!p.song) {
		part_list.Clear();
		line_list.Clear();
		break_list.Clear();
		return;
	}
	Song& s = *p.song;
	Artist& a = *p.artist;
	Release& r = *p.release;
	
	
	for(int i = 0; i < s.structure.GetCount(); i++) {
		String key = s.structure[i];
		int j = s.FindPartIdx(key);
		if (j < 0) {
			part_list.Set(i, 0, -1);
			part_list.Set(i, 1, i);
			part_list.Set(i, 2, "<error>");
		}
		else {
			Part& part = s.parts[j];
			part_list.Set(i, 0, j);
			part_list.Set(i, 1, i);
			part_list.Set(i, 2, part.name);
			part_list.Set(i, 3, part.GetLength(mode));
		}
	}
	part_list.SetCount(s.parts.GetCount());
	
	if (!part_list.IsCursor() && part_list.GetCount())
		part_list.SetCursor(0);
	
	DataPart();
}

void StructureCtrl::DataPart() {
	if (!part_list.IsCursor())
		return;
	Database& db = Database::Single();
	Ptrs& p = db.ctx[MALE];
	Song& s = *p.song;
	
	int c = part_list.GetCursor();
	int part_i = part_list.Get(c, 0);
	if (part_i < 0) {
		line_list.Clear();
	}
	else {
		Part& part = s.parts[part_i];
		for(int i = 0; i < part.lines.GetCount(); i++) {
			Line& line = part.lines[i];
			
			line_list.Set(i, 0, i);
			line_list.Set(i, 1, line.snap[mode].txt);
			line_list.Set(i, 2, line.GetLength(mode));
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
	Ptrs& p = db.ctx[MALE];
	Song& s = *p.song;
	
	int part_c = part_list.GetCursor();
	int part_i = part_list.Get(part_c, 0);
	int line_i = line_list.GetCursor();
	if (part_i < 0 || line_i < 0) {
		line_list.Clear();
	}
	else {
		Part& part = s.parts[part_i];
		Line& line = part.lines[line_i];
		
		for(int i = 0; i < line.breaks.GetCount(); i++) {
			Break& brk = line.breaks[i];
			
			break_list.Set(i, 0, i);
			break_list.Set(i, 1, brk.snap[mode].txt);
			break_list.Set(i, 2, brk.GetLength(mode));
			
			EditIntSpin* edit = new EditIntSpin;
			edit->WhenAction << [&, this, edit, line_i, part_c]() {
				brk.snap[this->mode].syllables = edit->GetData();
				line_list.Set(line_i, 2, line.GetLength(mode));
				part_list.Set(part_c, 3, part.GetLength(mode));
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
	Ptrs& p = db.ctx[MALE];
	Song& s = *p.song;
	
	int c = part_list.GetCursor();
	int part_i = part_list.Get(c, 0);
	int line_i = line_list.GetCursor();
	int brk_i = break_list.GetCursor();
	if (part_i < 0 || line_i < 0 || brk_i < 0) {
		break_list.Clear();
	}
	else {
		Part& part = s.parts[part_i];
		Line& line = part.lines[line_i];
		Break& brk = line.breaks[brk_i];
		
		
	}
	
	DataList();
}

void StructureCtrl::DataList() {
	Database& db = Database::Single();
	Ptrs& p = db.ctx[MALE];
	Song& song = *p.song;
	
	int c = part_list.GetCursor();
	int part_i = part_list.Get(c, 0);
	int line_i = line_list.GetCursor();
	int brk_i = break_list.GetCursor();
	if (part_i < 0 || line_i < 0 || brk_i < 0) {
		break_list.Clear();
	}
	else {
		Part& part = song.parts[part_i];
		Line& line = part.lines[line_i];
		Break& brk = line.breaks[brk_i];
		
		int list_i = 0;
		
		list.Set(list_i, 0, t_("Break"));
		list.Set(list_i, 1, t_("Text"));
		list.Set(list_i, 2, brk.snap[mode].txt);
		list_i++;
		
		for(int i = 0; i < brk.snap[mode].data.GetCount(); i++) {
			list.Set(list_i, 0, t_("Break"));
			list.Set(list_i, 1, brk.snap[mode].data.GetKey(i));
			list.Set(list_i, 2, brk.snap[mode].data[i]);
			list_i++;
		}
		
		list.Set(list_i, 0, t_("Line"));
		list.Set(list_i, 1, t_("Text"));
		list.Set(list_i, 2, line.snap[mode].txt);
		list_i++;
		
		for(int i = 0; i < line.snap[mode].data.GetCount(); i++) {
			list.Set(list_i, 0, t_("Line"));
			list.Set(list_i, 1, line.snap[mode].data.GetKey(i));
			list.Set(list_i, 2, line.snap[mode].data[i]);
			list_i++;
		}
		
		list.Set(list_i, 0, t_("Part"));
		list.Set(list_i, 1, t_("Text"));
		list.Set(list_i, 2, part.snap[mode].txt);
		list_i++;
		
		for(int i = 0; i < part.snap[mode].data.GetCount(); i++) {
			list.Set(list_i, 0, t_("Part"));
			list.Set(list_i, 1, part.snap[mode].data.GetKey(i));
			list.Set(list_i, 2, part.snap[mode].data[i]);
			list_i++;
		}
		
		for(int i = 0; i < song.snap[mode].data.GetCount(); i++) {
			list.Set(list_i, 0, t_("Song"));
			list.Set(list_i, 1, song.snap[mode].data.GetKey(i));
			list.Set(list_i, 2, song.snap[mode].data[i]);
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
	Ptrs& p = db.ctx[MALE];
	int mode = ctrl->mode;
	
	Size sz = GetSize();
	
	d.DrawRect(sz, White());
	
	Song& song = *p.song;
	
	items.SetCount(0);
	int total_len = 0;
	for(int i = 0; i < song.structure.GetCount(); i++) {
		const String& key = song.structure[i];
		Part* part = song.FindPart(key);
		if (!part) continue;
		
		Item& item = items.Add();
		item.part = part;
		item.len = part->GetLength(mode);
		
		if (key.Find("verse") == 0)
			item.clr = Color(226, 85, 0);
		else if (key.Find("chorus") == 0)
			item.clr = Color(141, 255, 0);
		else if (key.Find("bridge") == 0)
			item.clr = Color(56, 170, 255);
		else
			item.clr = Color(Random(256), Random(256), Random(256));
		
		total_len += item.len;
	}
	
	if (!total_len) return;
	
	Font fnt = SansSerif(13);
	double cx = sz.cx / (double)total_len;
	int len_sum = 0;
	for (Item& item : items) {
		int x = len_sum * cx;
		int w = (len_sum + item.len) * cx;
		d.DrawRect(RectC(x, 0, w, sz.cy), Blend(item.clr, White(), 128));
		String key = Capitalize(item.part->name);
		d.DrawText(x+5,5, key, fnt, Black());
		d.DrawText(x+4,4, key, fnt, White());
		if (len_sum > 0)
			d.DrawLine(x, 0, x, sz.cy, 1, Black());
		len_sum += item.len;
	}
}
