#include "AdvancedCtrl.h"

PatternScoringCtrl::PatternScoringCtrl() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	
	if (!p.song || !p.song->pipe)
		return;
	Song& song = *p.song;
	Pipe& pipe = *p.song->pipe;
	
	Add(mainsplit.SizePos());
	mainsplit.Vert();
	
	for (const SnapArg& a : ModeArgs()) {
		Splitter& vsplit = this->vsplit[a];
		Plotter& plotter = this->plotter[a];
		ArrayCtrl& list = this->list[a];
		
		plotter.list = &list;
		plotter.SetMode(a);
		plotter.SetSource(2);
		
		mainsplit << plotter << list;// << presets;
		
		Attributes& g = pipe;
		list.AddIndex();
		list.AddIndex();
		list.AddIndex();
		list.AddColumn(t_("Position"));
		//presets.AddColumn(t_("Name"));
		for (Attr::ScoringType& t : g.attr_scorings) {
			list.AddColumn(
				//g.Translate(t.klass) + ": " +
				db.Translate(t.axes0) + "/" +
				db.Translate(t.axes1)
				);
			/*presets.AddColumn(
				//g.Translate(t.klass) + ": " +
				g.Translate(t.axes0) + "/" +
				g.Translate(t.axes1)
				);*/
		}
		
		//part_list.AddColumn(t_("Name"));
		
		list.WhenBar << THISBACK(ListMenu);
		//presets.WhenBar << THISBACK(PresetMenu);
		//presets.WhenLeftDouble << THISBACK(ApplyPreset);
	}
	
}
/*
void PatternScoringCtrl::AddPreset() {
	String preset_name;
	bool b = EditTextNotNull(
		preset_name,
		t_("Preset's name"),
		t_("Preset's name"),
		0
	);
	if (b) {
		DUMP(preset_name);
	}
}*/

void PatternScoringCtrl::Data() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	
	if (p.song) {
		Song& song = *p.song;
		if (!song.pipe) return;
		Pipe& pipe = *song.pipe;
		
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
	//DataPresets();
}

/*void PatternScoringCtrl::DataPresets() {
	Database& db = Database::Single();
	for(int i = 0; i < db.attrscores.presets.GetCount(); i++) {
		String key = db.attrscores.presets.GetKey(i);
		const auto& values = db.attrscores.presets[i];
		presets.Set(i, 0, key);
		for(int j = 0; j < values.GetCount(); j++)
			presets.Set(i, 1+j, values[j]);
	}
	presets.SetCount(db.attrscores.presets.GetCount());
}*/

void PatternScoringCtrl::DataList(const SnapArg& a) {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song || !p.song->pipe)
		return;
	Song& song = *p.song;
	Pipe& pipe = *p.song->pipe;
	PipePtrs& pp = pipe.p;
	
	Splitter& vsplit = this->vsplit[a];
	Plotter& plotter = this->plotter[a];
	ArrayCtrl& list = this->list[a];
	
	// Whole song
	if (db.ctx.active_wholesong) {
		list.SetCount(0);
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
					list.Set(total, 0, part_i);
					list.Set(total, 1, j);
					list.Set(total, 2, k);
					list.Set(total, 3, part_name + ":" + IntStr(j) + ":" + IntStr(k));
					for(int j = 0; j < brk.Get(a).partscore.GetCount(); j++) {
						int value = brk.Get(a).partscore[j];
						int k1 = group_begin+j;
						int k2 = group_begin-3+j; // skip 3 index columns for visible Ctrls
						list.Set(total, k1, value);
						
						Ctrl* c = new EditIntNotNullSpin;
						*c <<= THISBACK3(ListValueChanged, a, total, j);
						list.SetCtrl(total, k2, c);
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
				list.Set(total, 0, part_i);
				list.Set(total, 1, i);
				list.Set(total, 2, k);
				list.Set(total, 3, part_name + ":" + IntStr(i) + ":" + IntStr(k));
				for(int j = 0; j < brk.Get(a).partscore.GetCount(); j++) {
					int value = brk.Get(a).partscore[j];
					int k1 = group_begin+j;
					int k2 = group_begin-3+j; // skip 3 index columns for visible Ctrls
					list.Set(total, k1, value);
					
					Ctrl* c = new EditIntNotNullSpin;
					*c <<= THISBACK3(ListValueChanged, a, total, j);
					list.SetCtrl(total, k2, c);
				}
				total++;
			}
		}
		list.SetCount(total);
	}
}

void PatternScoringCtrl::ListValueChanged(const SnapArg& a, int pos, int scoring) {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song || !p.song->pipe)
		return;
	Song& song = *p.song;
	Pipe& pipe = *p.song->pipe;
	PipePtrs& pp = pipe.p;
	
	Splitter& vsplit = this->vsplit[a];
	Plotter& plotter = this->plotter[a];
	ArrayCtrl& list = this->list[a];
	
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

void PatternScoringCtrl::ListMenu(Bar& bar) {
	//bar.Add(t_("Save as preset"), THISBACK(SavePreset));
	//bar.Add(t_("Update focused preset"), THISBACK(UpdatePreset));
}

#if 0
void PatternScoringCtrl::PresetMenu(Bar& bar) {
	bar.Add(t_("Add preset"), THISBACK(AddPreset));
	bar.Add(t_("Apply preset"), THISBACK(ApplyPreset));
	bar.Add(t_("Remove preset"), THISBACK(RemovePreset));
	
}

void PatternScoringCtrl::SavePreset(const SnapArg& a) {
	Splitter& vsplit = this->vsplit[mode];
	Plotter& plotter = this->plotter[mode];
	ArrayCtrl& list = this->list[mode];
	
	if (!list.IsCursor())
		return;
	
	Database& db = Database::Single();
	AttrScore& a = db.attrscores;
	String name;
	bool b = EditTextNotNull(
		name,
		t_("Name of the preset"),
		t_("Preset's name"),
		0
	);
	if (!b) return;
	
	if (a.presets.Find(name) >= 0) {
		PromptOK(DeQtf(Format(t_("Preset '%s' already exists"), name)));
		return;
	}
	
	int c = db.attr_scorings.GetCount();
	Vector<int>& scores = a.presets.Add(name);
	scores.SetCount(c);
	
	int row = list.GetCursor();
	for(int i = 0; i < c; i++) {
		int j = group_begin + i;
		scores[i] = list.Get(row, j);
	}
	
	DataPresets();
}

void PatternScoringCtrl::UpdatePreset() {
	if (!list.IsCursor() || !presets.IsCursor())
		return;
	
	Database& db = Database::Single();
	AttrScore& a = db.attrscores;
	
	int c = db.attr_scorings.GetCount();
	Vector<int>& scores = a.presets[presets.GetCursor()];
	scores.SetCount(c);
	
	int row = list.GetCursor();
	for(int i = 0; i < c; i++) {
		int j = group_begin + i;
		scores[i] = list.Get(row, j);
	}
	
	DataPresets();
}

void PatternScoringCtrl::ApplyPreset() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!list.IsCursor() || !presets.IsCursor())
		return;
	if (!p.part)
		return;
	
	AttrScore& a = db.attrscores;
	
	int c = db.attr_scorings.GetCount();
	Vector<int>& scores = a.presets[presets.GetCursor()];
	scores.SetCount(c);
	
	
	int cur = list.GetCursor();
	int part_i = list.Get(cur, 0);
	int pos = list.Get(cur, 1);
	
	Part& part = *p.part;
	Line& line = part.lines[pos];
	line.snap[mode].partscore.SetCount(c, 0);
	
	for(int i = 0; i < c; i++) {
		line.snap[mode].partscore[i] = scores[i];
	}
	
	DataListAll();
}

void PatternScoringCtrl::RemovePreset() {
	if (!presets.IsCursor())
		return;
	
	Database& db = Database::Single();
	AttrScore& a = db.attrscores;
	
	int cur = presets.GetCursor();
	
	a.presets.Remove(cur);
	
	DataPresets();
}

#endif
