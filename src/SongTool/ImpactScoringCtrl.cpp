#include "SongTool.h"


ImpactScoringCtrl::ImpactScoringCtrl() {
	Add(mainsplit.SizePos());
	mainsplit.Vert();
	
}

void ImpactScoringCtrl::Realize(Pipe& p) {
	Database& db = Database::Single();
	Attributes& g = p;
	
	if (last_pipe == &p)
		return;
	last_pipe = &p;
	
	mainsplit.Clear();
	
	int tabs = 0;
	
	for (const SnapArg& a : ModeArgs()) {
		Splitter& vsplit = this->vsplit[a];
		Plotter& plotter = this->plotter[a];
		One<ArrayCtrl>& list0 = this->list[a];
		list0.Clear();
		list0.Create();
		ArrayCtrl& list = *list0;
		
		plotter.list = &list;
		plotter.SetMode(a);
		plotter.SetSource(0);
		
		mainsplit << plotter << list;// << presets;
		tabs += 2;
		
		list.AddIndex();
		list.AddIndex();
		list.AddIndex();
		list.AddColumn(t_("Position"));
		for (Attr::ScoringType& t : g.attr_scorings) {
			list.AddColumn(
				//db.Translate(t.klass) + ": " +
				db.Translate(t.axes0) + "/" +
				db.Translate(t.axes1)
				);
		}
		
		list.WhenBar << THISBACK(ListMenu);
	}
	
	int cy = 10000 / tabs;
	for(int i = 1; i < tabs; i+=2) {
		int pos = i * cy + cy * 29 / 30;
		mainsplit.SetPos(pos, i-1);
	}
}

void ImpactScoringCtrl::Data() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (p.song && p.song->pipe) {
		Pipe& pipe = *p.song->pipe;
		PipePtrs& pp = pipe.p;
		Realize(pipe);
		//p.song->RealizeImpacts();
		
		if (db.ctx.active_wholesong) {
			for (const SnapArg& a : ModeArgs()) {
				plotter[a].SetWholeSong(pipe);
			}
		}
		else {
			Part& part = *pp.part;
			for (const SnapArg& a : ModeArgs()) {
				plotter[a].SetPart(part);
			}
		}
		
		DataListAll();
	}
	
}

void ImpactScoringCtrl::DataList(const SnapArg& a) {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song || !p.song->pipe)
		return;
	Song& song = *p.song;
	Pipe& pipe = *song.pipe;
	PipePtrs& pp = pipe.p;
	
	Splitter& vsplit = this->vsplit[a];
	Plotter& plotter = this->plotter[a];
	One<ArrayCtrl>& list0 = this->list[a];
	list0.Clear();
	list0.Create();
	ArrayCtrl& list = *list0;
	
	int sc = pipe.attr_scorings.GetCount();
	
	//DUMPC(song.impacts.GetKeys());
	
	// Whole song
	if (db.ctx.active_wholesong) {
		list.SetCount(0);
		if (list.GetColumnCount() == 0)
			return;
		plotter.SetWholeSong(pipe);
		int total = 0;
		for(int i = 0; i < pipe.structure.GetCount(); i++) {
			String part_key = pipe.structure[i];
			int part_i = pipe.FindPartIdx(part_key);
			if (part_i < 0) continue;
			Part& part = pipe.parts[part_i];
			String part_name = db.Translate(part.name);
			
			for(int j = 0; j < part.lines.GetCount(); j++) {
				Line& line = part.lines[j];
				for(int k = 0; k < line.breaks.GetCount(); k++) {
					Break& brk = line.breaks[k];
					PatternSnap& snap = brk.Get(a);
					
					list.Set(total, 0, part_i);
					list.Set(total, 1, j);
					list.Set(total, 2, k);
					list.Set(total, 3, part_name + ":" + IntStr(j) + ":" + IntStr(k));
					
					//String impact = ToLower(brk.snap[mode].impact);
					//Impact* im = song.FindImpact(impact);
					//Impact& im = brk.snap[mode].impact;
					for(int j = 0; j < sc; j++) {
						int k1 = group_begin+j;
						int k2 = group_begin-3+j; // skip 3 index columns for visible Ctrls
						if (snap.impact_score.IsEmpty()) {
							list.Set(total, k1, Value());
							//list.SetCtrl(total, k2, 0);
						}
						else {
							const Vector<int>& impact_score = snap.impact_score;
							if (impact_score.IsEmpty()) continue;
							int value =  impact_score[j];
							list.Set(total, k1, value);
							
							Ctrl* c = new EditIntNotNullSpin;
							*c <<= THISBACK3(ListValueChanged, a, total, j);
							list.SetCtrl(total, k2, c);
						}
						//list.Set(total, k1, (int)Random(10));
					}
					total++;
				}
			}
		}
		list.SetCount(total);
	}
	else {
		if (!pp.part)
			return;
		Part& part = *pp.part;
		plotter.SetPart(part);
		String part_name = db.Translate(part.name);
		int part_i = pp.GetActivePartIndex();
		int total = 0;
		for(int i = 0; i < part.lines.GetCount(); i++) {
			Line& line = part.lines[i];
			for(int k = 0; k < line.breaks.GetCount(); k++) {
				Break& brk = line.breaks[k];
				PatternSnap& snap = brk.Get(a);
				
				list.Set(total, 0, part_i);
				list.Set(total, 1, i);
				list.Set(total, 2, k);
				list.Set(total, 3, part_name + ":" + IntStr(i) + ":" + IntStr(k));
				
				//String impact = ToLower(brk.snap[mode].impact);
				//Impact* im = song.FindImpact(impact);
				//Impact& im = brk.snap[mode].impact;
				for(int j = 0; j < sc; j++) {
					int k1 = group_begin+j;
					int k2 = group_begin-3+j; // skip 3 index columns for visible Ctrls
					if (snap.impact_score.IsEmpty()) {
						list.Set(total, k1, Value());
						//list.SetCtrl(total, k2, 0);
					}
					else {
						const Vector<int>& impact_score = snap.impact_score;
						int value =  impact_score[j];
						list.Set(total, k1, value);
						
						Ctrl* c = new EditIntNotNullSpin;
						*c <<= THISBACK3(ListValueChanged, a, total, j);
						list.SetCtrl(total, k2, c);
					}
				}
				total++;
			}
		}
		list.SetCount(total);
	}
}

void ImpactScoringCtrl::ListValueChanged(const SnapArg& a, int pos, int scoring) {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	
	if (!p.song || !p.song->pipe)
		return;
	Pipe& pipe = *p.song->pipe;
	
	Splitter& vsplit = this->vsplit[a];
	Plotter& plotter = this->plotter[a];
	One<ArrayCtrl>& list0 = this->list[a];
	list0.Clear();
	list0.Create();
	ArrayCtrl& list = *list0;
	
	int part_i = list.Get(pos, 0);
	int line_i = list.Get(pos, 1);
	int brk_i = list.Get(pos, 2);
	
	Part& part = pipe.parts[part_i];
	Line& line = part.lines[line_i];
	Break& brk = line.breaks[brk_i];
	
	if (brk.Get(a).partscore.GetCount() != pipe.attr_scorings.GetCount())
		brk.Get(a).partscore.SetCount(pipe.attr_scorings.GetCount(), 0);
	
	auto& dst = brk.Get(a).partscore[scoring];
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
