#include "AdvancedCtrl.h"


MaskScoringCtrl::MaskScoringCtrl() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song || !p.song->pipe)
		return;
	Pipe& pipe = *p.song->pipe;
	
	Add(mainsplit.SizePos());
	mainsplit.Vert();
	
	for (const SnapArg& a : ModeArgs()) {
		Splitter& vsplit = this->vsplit[a];
		Plotter& plotter = this->plotter[a];
		ArrayCtrl& list = this->list[a];
		
		plotter.list = &list;
		plotter.SetMode(a);
		plotter.SetSource(1);
		
		mainsplit << plotter << list;// << presets;
		
		Attributes& g = pipe;
		list.AddIndex();
		list.AddIndex();
		list.AddIndex();
		list.AddColumn(t_("Position"));
		for (Attr::ScoringType& t : g.attr_scorings) {
			list.AddColumn(
				//g.Translate(t.klass) + ": " +
				db.Translate(t.axes0) + "/" +
				db.Translate(t.axes1)
				);
		}
		
		list.WhenBar << THISBACK(ListMenu);
	}
	
}

void MaskScoringCtrl::Data() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song || !p.song->pipe)
		return;
	Pipe& pipe = *p.song->pipe;
	
	if (p.song) {
		if (db.ctx.active_wholesong) {
			for (const SnapArg& a : ModeArgs())
				plotter[a].SetWholeSong(pipe);
		}
		else {
			Part& part = *pipe.p.part;
			for (const SnapArg& a : ModeArgs())
				plotter[a].SetPart(part);
		}
	}
	
	DataListAll();
}

void MaskScoringCtrl::DataList(const SnapArg& a) {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song || !p.song->pipe)
		return;
	Song& song = *p.song;
	Pipe& pipe = *p.song->pipe;
	
	Splitter& vsplit = this->vsplit[a];
	Plotter& plotter = this->plotter[a];
	ArrayCtrl& list = this->list[a];
	
	int sc = pipe.attr_scorings.GetCount();
	
	//DUMPC(song.impacts.GetKeys());
	
	// Whole song
	{
		list.SetCount(0);
		plotter.SetWholeSong(pipe);
		int total = 0;
		for(int i = 0; i < pipe.structure.GetCount(); i++) {
			String part_key = pipe.structure[i];
			int part_i = pipe.FindPartIdx(part_key);
			if (part_i < 0) continue;
			Part& part = pipe.parts[part_i];
			String part_name = db.Translate(part.name);
			
			list.Set(total, 0, part_i);
			list.Set(total, 1, -1);
			list.Set(total, 2, -1);
			list.Set(total, 3, part_name);
			for(int j = 0; j < part.Get(a).maskscore.GetCount(); j++) {
				int value = part.Get(a).maskscore[j];
				int k1 = group_begin+j;
				int k2 = group_begin-3+j; // skip 3 index columns for visible Ctrls
				list.Set(total, k1, value);
				
				Ctrl* c = new EditIntNotNullSpin;
				*c <<= THISBACK3(ListValueChanged, a, total, j);
				list.SetCtrl(total, k2, c);
			}
			total++;
		}
		list.SetCount(total);
	}
}

void MaskScoringCtrl::ListValueChanged(const SnapArg& a, int pos, int scoring) {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	
	if (!p.song || !p.song->pipe)
		return;
	Song& song = *p.song;
	Pipe& pipe = *p.song->pipe;
	
	Splitter& vsplit = this->vsplit[a];
	Plotter& plotter = this->plotter[a];
	ArrayCtrl& list = this->list[a];
	
	int part_i = list.Get(pos, 0);
	int line_i = list.Get(pos, 1);
	
	Part& part = pipe.parts[part_i];
	Line& line = part.lines[line_i];
	
	if (line.Get(a).maskscore.GetCount() != pipe.attr_scorings.GetCount())
		line.Get(a).maskscore.SetCount(pipe.attr_scorings.GetCount(), 0);
	
	auto& dst = line.Get(a).maskscore[scoring];
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
