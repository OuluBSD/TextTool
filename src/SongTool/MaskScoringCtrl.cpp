#include "SongTool.h"


MaskScoringCtrl::MaskScoringCtrl() {
	Add(mainsplit.SizePos());
	mainsplit.Vert();
	
	for (int mode = 0; mode < GENDER_COUNT; mode++) {
		Splitter& vsplit = this->vsplit[mode];
		Plotter& plotter = this->plotter[mode];
		ArrayCtrl& list = this->list[mode];
		
		plotter.list = &list;
		plotter.SetMode(mode);
		plotter.SetSource(1);
		
		mainsplit << plotter << list;// << presets;
		
		Attributes& g = Database::Single().attrs;
		list.AddIndex();
		list.AddIndex();
		list.AddIndex();
		list.AddColumn(t_("Position"));
		for (Attributes::ScoringType& t : g.scorings) {
			list.AddColumn(
				//g.Translate(t.klass) + ": " +
				g.Translate(t.axes0) + "/" +
				g.Translate(t.axes1)
				);
		}
		
		list.WhenBar << THISBACK(ListMenu);
	}
	
}

void MaskScoringCtrl::Data() {
	Database& db = Database::Single();
	Ptrs& p = db.ctx.p;
	if (p.song) {
		if (db.ctx.active_wholesong) {
			for (int mode = 0; mode < GENDER_COUNT; mode++)
				plotter[mode].SetWholeSong(*p.song);
		}
		else {
			Part& part = *p.part;
			for (int mode = 0; mode < GENDER_COUNT; mode++)
				plotter[mode].SetPart(part);
		}
	}
	
	DataListAll();
}

void MaskScoringCtrl::DataList(int mode) {
	Database& db = Database::Single();
	Ptrs& p = db.ctx.p;
	if (!p.song)
		return;
	Song& song = *p.song;
	
	Splitter& vsplit = this->vsplit[mode];
	Plotter& plotter = this->plotter[mode];
	ArrayCtrl& list = this->list[mode];
	
	int sc = db.attrs.scorings.GetCount();
	
	//DUMPC(song.impacts.GetKeys());
	
	// Whole song
	{
		list.SetCount(0);
		plotter.SetWholeSong(song);
		int total = 0;
		for(int i = 0; i < song.structure.GetCount(); i++) {
			String part_key = song.structure[i];
			int part_i = song.FindPartIdx(part_key);
			if (part_i < 0) continue;
			Part& part = song.parts[part_i];
			String part_name = db.attrs.Translate(part.name);
			
			list.Set(total, 0, part_i);
			list.Set(total, 1, -1);
			list.Set(total, 2, -1);
			list.Set(total, 3, part_name);
			for(int j = 0; j < part.snap[mode].maskscore.GetCount(); j++) {
				int value = part.snap[mode].maskscore[j];
				int k1 = group_begin+j;
				int k2 = group_begin-3+j; // skip 3 index columns for visible Ctrls
				list.Set(total, k1, value);
				
				Ctrl* c = new EditIntNotNullSpin;
				*c <<= THISBACK3(ListValueChanged, mode, total, j);
				list.SetCtrl(total, k2, c);
			}
			total++;
		}
		list.SetCount(total);
	}
}

void MaskScoringCtrl::ListValueChanged(int mode, int pos, int scoring) {
	Database& db = Database::Single();
	Ptrs& p = db.ctx.p;
	
	if (!p.song)
		return;
	Song& song = *p.song;
	
	Splitter& vsplit = this->vsplit[mode];
	Plotter& plotter = this->plotter[mode];
	ArrayCtrl& list = this->list[mode];
	
	int part_i = list.Get(pos, 0);
	int line_i = list.Get(pos, 1);
	
	Part& part = song.parts[part_i];
	Line& line = part.lines[line_i];
	
	if (line.snap[mode].maskscore.GetCount() != db.attrs.scorings.GetCount())
		line.snap[mode].maskscore.SetCount(db.attrs.scorings.GetCount(), 0);
	
	auto& dst = line.snap[mode].maskscore[scoring];
	int value = list.Get(pos, group_begin+scoring);
	dst = value;
	
	// Refresh duplicate values (for whole song)
	for(int j = 0; j < list.GetCount(); j++) {
		int part_i0 = list.Get(j, 0);
		int line_i0 = list.Get(j, 1);
		if (j != pos && part_i0 == part_i && line_i0 == line_i) {
			list.Set(j, group_begin+scoring, value);
		}
	}
	
	plotter.Refresh();
}

void MaskScoringCtrl::ListMenu(Bar& bar) {
	
}
