#include "SimplifiedCtrl.h"



FineStructureCtrl::FineStructureCtrl() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << list << rhymes << notes << drawer;
	hsplit.SetPos(3000,0);
	hsplit.SetPos(5000,1);
	hsplit.SetPos(8000,2);
	
	list.AddColumn(t_("Name"));
	list.AddColumn(t_("Type"));
	list.AddColumn(t_("Bar-length"));
	list.AddColumn(t_("Rhyme-count"));
	LineListCtrl::Init();
	list.ColumnWidths("2 2 5 5");
	list.WhenCursor << THISBACK(DataPart);
	
	rhymes.AddColumn(t_("#"));
	rhymes.AddColumn(t_("Beat-length"));
	rhymes.AddColumn(t_("Note-count"));
	rhymes.ColumnWidths("1 2 2");
	rhymes.WhenCursor << THISBACK(DataRhyme);
	
	
	notes.AddColumn(t_("#"));
	notes.AddColumn(t_("Frozen word/syllable"));
	notes.AddColumn(t_("Only first syllable"));
	notes.AddColumn(t_("Must be singable"));
	
}

void FineStructureCtrl::DisableAll() {
	
}

void FineStructureCtrl::EnableAll() {
	
}

void FineStructureCtrl::Data() {
	Song& s = GetSong();
	
	for(int i = 0; i < s.parts.GetCount(); i++) {
		StaticPart& part = s.parts[i];
		
		list.Set(i, "PART_IDX", i);
		list.Set(i, "LINE_IDX", -1);
		
		Color clr = GetSongPartPaperColor(part.type);
		list.Set(i, 0, AttrText(part.name).NormalPaper(clr));
		
		DropList& dl = list.CreateCtrl<DropList>(i, 1);
		dl.Add(t_("Singing"));
		dl.Add(t_("Rapping"));
		dl.Add(t_("Poetry"));
		dl.Add(t_("Dialog"));
		dl.Add(t_("Skip"));
		int idx = max(0, min(dl.GetCount()-1, part.part_type));
		dl.SetIndex(idx);
		dl.WhenAction << [&part,i,&dl]() {part.part_type = dl.GetIndex();};
		
		EditIntSpin& bc = list.CreateCtrl<EditIntSpin>(i, 2);
		bc.MinMax(0, 100);
		bc.SetData(part.bar_length);
		bc.WhenAction << [this,&bc,&part]() {
			part.bar_length = bc.GetData();
			PostCallback(THISBACK(DataPart));
			drawer.Refresh();
		};
		
		EditIntSpin& rc = list.CreateCtrl<EditIntSpin>(i, 3);
		rc.MinMax(0, 100);
		rc.SetData(part.rhymes.GetCount());
		rc.WhenAction << [this,&rc,&part]() {
			int count = rc.GetData();
			part.rhymes.SetCount(count);
			PostCallback(THISBACK(DataPart));
			drawer.Refresh();
		};
		
	}
	list.SetCount(s.parts.GetCount());
	
	if (list.GetCount() && !list.IsCursor())
		list.SetCursor(0);
	
	DataPart();
}

void FineStructureCtrl::DataPart() {
	Song& s = GetSong();
	
	if (!list.IsCursor()) {
		rhymes.Clear();
		notes.Clear();
		return;
	}
	
	int part_i = list.Get("PART_IDX");
	StaticPart& part = s.parts[part_i];
	rhymes.SetCount(part.rhymes.GetCount());
	
	Database& db = Database::Single();
	db.ctx.ed.part = &part;
	
	for(int i = 0; i < part.rhymes.GetCount(); i++) {
		StaticRhyme& r = part.rhymes[i];
		
		rhymes.Set(i, 0, i);
		
		EditDoubleSpin& bc = rhymes.CreateCtrl<EditDoubleSpin>(i, 1);
		bc.SetData(r.beat_length);
		bc.MinMax(0, 64);
		bc.WhenAction << [this,&bc,&r]() {
			r.beat_length = bc.GetData();
			PostCallback(THISBACK(DataRhyme));
			drawer.Refresh();
		};
		
		EditIntSpin& e = rhymes.CreateCtrl<EditIntSpin>(i, 2);
		e.SetData(r.notes.GetCount());
		e.MinMax(0, 100);
		e.WhenAction << [this,&e,&r]() {
			int c = e.GetData();
			r.notes.SetCount(c);
			PostCallback(THISBACK(DataRhyme));
			drawer.Refresh();
		};
	}
	
	if (rhymes.GetCount() && !rhymes.IsCursor())
		rhymes.SetCursor(0);
	
	DataRhyme();
}

void FineStructureCtrl::DataRhyme() {
	Song& s = GetSong();
	
	if (!list.IsCursor() || !rhymes.IsCursor()) {
		notes.Clear();
		return;
	}
	
	int part_i = list.Get("PART_IDX");
	int rhyme_i = rhymes.GetCursor();
	StaticPart& part = s.parts[part_i];
	StaticRhyme& rhyme = part.rhymes[rhyme_i];
	
	notes.SetCount(rhyme.notes.GetCount());
	for(int i = 0; i < rhyme.notes.GetCount(); i++) {
		StaticNote& note = rhyme.notes[i];
		
		notes.Set(i, 0, i);
		
		{
			auto& ctrl = notes.CreateCtrl<EditString>(i, 1);
			ctrl.SetData(note.frozen_syllable);
			ctrl.WhenAction << [&ctrl,&note]() {
				note.frozen_syllable = ctrl.GetData();
			};
		}
		{
			auto& ctrl = notes.CreateCtrl<Option>(i, 2);
			ctrl.Set(note.only_first_syllable);
			ctrl.WhenAction << [&ctrl,&note]() {
				note.only_first_syllable = ctrl.Get();
			};
		}
		{
			auto& ctrl = notes.CreateCtrl<Option>(i, 3);
			ctrl.Set(note.long_singable_syllable);
			ctrl.WhenAction << [&ctrl,&note]() {
				note.long_singable_syllable = ctrl.Get();
			};
		}
	}
	
	drawer.Refresh();
}

void FineStructureCtrl::ToolMenu(Bar& bar) {
	SongToolCtrl::ToolMenu(bar);
}





PartRhymeDrawer::PartRhymeDrawer() {
	bg = GrayColor();
	line = GrayColor(64);
	rhyme_begin = LtRed();
	beat_begin = LtGreen();
	
}

void PartRhymeDrawer::Paint(Draw& d) {
	Size sz = GetSize();
	
	d.DrawRect(sz, bg);
	
	Database& db = Database::Single();
	if (!db.ctx.ed.song || !db.ctx.ed.part)
		return;
	Song& song = *db.ctx.ed.song;
	StaticPart& part = *db.ctx.ed.part;
	if (part.rhymes.IsEmpty())
		return;
	
	int rows = part.bar_length / bars_per_row + (part.bar_length % bars_per_row != 0 ? 1 : 0);
	if (!rows) return;
	
	double gridh = 30;
	double gridw = (double)(sz.cx - 2 * off) / bars_per_row;
	int totalh = rows * gridh;
	
	// Grid
	{
		int grid_lw_2 = grid_lw / 2;
		int goff = off - grid_lw_2;
		
		for(int i = 0; i <= bars_per_row; i++) {
			int x = goff + i * gridw;
			d.DrawLine(x, goff, x, totalh + grid_lw_2, grid_lw, line);
			
			for(int j = 0; j <= rows; j++) {
				int y = goff + j * gridh;
				
				d.DrawLine(goff, y, sz.cx - 2*off, y, grid_lw, line);
			}
		}
	}
	
	double pos = 0;
	for (StaticRhyme& r : part.rhymes) {
		if (r.notes.GetCount()) {
			double note_len = r.beat_length / r.notes.GetCount();
			int note_i = 0;
			for (StaticNote& n : r.notes) {
				DrawLine(d, pos, gridh, note_i);
				pos += note_len;
				note_i++;
			}
		}
		else {
			DrawLine(d, pos, gridh, 0);
			pos += r.beat_length;
		}
	}
	/*int ri = 0;
	StaticRhyme* r = &part.rhymes[ri];
	int rowh = rows > 0 ? min(30, sz.cy / rows) : 30;
	for(int i = 0; i < part.rhymes.GetCount(); i++) {
		int y = rowh * i;
		
	}*/
	
}

void PartRhymeDrawer::DrawLine(Draw& d, double pos, double gridh, int note_i) {
	Size sz = GetSize();
	int row = pos / (bars_per_row * 4);
	double col = fmod(pos, (double)(bars_per_row * 4));
	double colf = col / (double)(bars_per_row * 4);
	if (note_i == 0 && colf > 0.99) {
		colf = 0;
		row++;
	}
	
	int lw = note_i == 0 ? 3 : 1;
	int lw_2 = lw == 1 ? 2 : lw - lw / 2;
	int y0 = off + row * gridh;
	int y1 = off + (row + 1) * gridh;
	int x = off - lw_2 + colf * (sz.cx - 2 * off);
	Color c = note_i == 0 ? rhyme_begin : beat_begin;
	
	d.DrawLine(x, y0, x, y1, lw, c);
}