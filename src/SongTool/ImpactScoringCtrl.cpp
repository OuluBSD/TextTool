#include "SongTool.h"


ImpactScoringCtrl::ImpactScoringCtrl() {
	Add(mainsplit.SizePos());
	mainsplit.Vert();
	
	for (int mode = 0; mode < GENDER_COUNT; mode++) {
		Splitter& vsplit = this->vsplit[mode];
		Plotter& plotter = this->plotter[mode];
		ArrayCtrl& list = this->list[mode];
		
		plotter.list = &list;
		plotter.SetMode(mode);
		plotter.SetSource(0);
		
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

void ImpactScoringCtrl::Data() {
	Database& db = Database::Single();
	Ptrs& p = db.ctx[MALE];
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

void ImpactScoringCtrl::DataList(int mode) {
	Database& db = Database::Single();
	Ptrs& p = db.ctx[MALE];
	if (!p.song)
		return;
	Song& song = *p.song;
	
	Splitter& vsplit = this->vsplit[mode];
	Plotter& plotter = this->plotter[mode];
	ArrayCtrl& list = this->list[mode];
	
	int sc = db.attrs.scorings.GetCount();
	
	//DUMPC(song.impact_scores.GetKeys());
	
	// Whole song
	if (db.ctx.active_wholesong) {
		list.SetCount(0);
		plotter.SetWholeSong(song);
		int total = 0;
		for(int i = 0; i < song.structure.GetCount(); i++) {
			String part_key = song.structure[i];
			int part_i = song.FindPartIdx(part_key);
			if (part_i < 0) continue;
			Part& part = song.parts[part_i];
			String part_name = db.attrs.Translate(part.name);
			
			for(int j = 0; j < part.lines.GetCount(); j++) {
				Line& line = part.lines[j];
				for(int k = 0; k < line.breaks.GetCount(); k++) {
					Break& brk = line.breaks[k];
					
					list.Set(total, 0, part_i);
					list.Set(total, 1, j);
					list.Set(total, 2, k);
					list.Set(total, 3, part_name + ":" + IntStr(j) + ":" + IntStr(k));
					
					String impact = ToLower(brk.snap[mode].data.Get("impact", ""));
					int impact_i = song.impact_scores.Find(impact);
					for(int j = 0; j < sc; j++) {
						int k1 = group_begin+j;
						int k2 = group_begin-3+j; // skip 3 index columns for visible Ctrls
						if (impact_i < 0) {
							list.Set(total, k1, Value());
							//list.SetCtrl(total, k2, 0);
						}
						else {
							const Vector<int>& impact_score = song.impact_scores[impact_i];
							int value =  impact_score[j];
							list.Set(total, k1, value);
							
							Ctrl* c = new EditIntNotNullSpin;
							*c <<= THISBACK3(ListValueChanged, mode, total, j);
							list.SetCtrl(total, k2, c);
						}
					}
					total++;
				}
			}
		}
		list.SetCount(total);
	}
	else {
		if (!p.part)
			return;
		Part& part = *p.part;
		plotter.SetPart(part);
		String part_name = db.attrs.Translate(part.name);
		int part_i = p.GetActivePartIndex();
		int total = 0;
		for(int i = 0; i < part.lines.GetCount(); i++) {
			Line& line = part.lines[i];
			for(int k = 0; k < line.breaks.GetCount(); k++) {
				Break& brk = line.breaks[k];
				list.Set(total, 0, part_i);
				list.Set(total, 1, i);
				list.Set(total, 2, k);
				list.Set(total, 3, part_name + ":" + IntStr(i) + ":" + IntStr(k));
				
				String impact = ToLower(brk.snap[mode].data.Get("impact", ""));
				int impact_i = song.impact_scores.Find(impact);
				for(int j = 0; j < sc; j++) {
					int k1 = group_begin+j;
					int k2 = group_begin-3+j; // skip 3 index columns for visible Ctrls
					if (impact_i < 0) {
						list.Set(total, k1, Value());
						//list.SetCtrl(total, k2, 0);
					}
					else {
						const Vector<int>& impact_score = song.impact_scores[impact_i];
						int value =  impact_score[j];
						list.Set(total, k1, value);
						
						Ctrl* c = new EditIntNotNullSpin;
						*c <<= THISBACK3(ListValueChanged, mode, total, j);
						list.SetCtrl(total, k2, c);
					}
				}
				total++;
			}
		}
		list.SetCount(total);
	}
}

void ImpactScoringCtrl::ListValueChanged(int mode, int pos, int scoring) {
	Database& db = Database::Single();
	Ptrs& p = db.ctx[MALE];
	
	if (!p.song)
		return;
	Song& song = *p.song;
	
	Splitter& vsplit = this->vsplit[mode];
	Plotter& plotter = this->plotter[mode];
	ArrayCtrl& list = this->list[mode];
	
	int part_i = list.Get(pos, 0);
	int line_i = list.Get(pos, 1);
	int brk_i = list.Get(pos, 2);
	
	Part& part = song.parts[part_i];
	Line& line = part.lines[line_i];
	Break& brk = line.breaks[brk_i];
	
	if (brk.snap[mode].partscore.GetCount() != db.attrs.scorings.GetCount())
		brk.snap[mode].partscore.SetCount(db.attrs.scorings.GetCount(), 0);
	
	auto& dst = brk.snap[mode].partscore[scoring];
	int value = list.Get(pos, group_begin+scoring);
	dst = value;
	
	// Refresh duplicate values (for whole song)
	for(int j = 0; j < list.GetCount(); j++) {
		int part_i0 = list.Get(j, 0);
		int line_i0 = list.Get(j, 1);
		int brk_i0 = list.Get(j, 2);
		if (j != pos && part_i0 == part_i && line_i0 == line_i && brk_i0 == brk_i) {
			list.Set(j, group_begin+scoring, value);
		}
	}
	
	plotter.Refresh();
}

void ImpactScoringCtrl::ListMenu(Bar& bar) {
	
}
