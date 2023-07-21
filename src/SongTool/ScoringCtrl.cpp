#include "SongTool.h"

ScoringCtrl::ScoringCtrl() {
	Add(mainsplit.SizePos());
	mainsplit.Vert();
	
	for (int mode = 0; mode < GENDER_COUNT; mode++) {
		Splitter& vsplit = this->vsplit[mode];
		Plotter& plotter = this->plotter[mode];
		ArrayCtrl& list = this->list[mode];
		
		plotter.list = &list;
		plotter.SetMode(mode);
		
		mainsplit << plotter << list;// << presets;
		
		Attributes& g = Database::Single().attrs;
		list.AddIndex();
		list.AddIndex();
		list.AddIndex();
		list.AddColumn(t_("Position"));
		//presets.AddColumn(t_("Name"));
		for (Attributes::ScoringType& t : g.scorings) {
			list.AddColumn(
				//g.Translate(t.klass) + ": " +
				g.Translate(t.axes0) + "/" +
				g.Translate(t.axes1)
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

void ScoringCtrl::AddPreset() {
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
}

void ScoringCtrl::Data() {
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
	//DataPresets();
}

/*void ScoringCtrl::DataPresets() {
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

void ScoringCtrl::DataList(int mode) {
	Database& db = Database::Single();
	Ptrs& p = db.ctx[MALE];
	if (!p.song)
		return;
	Song& o = *p.song;
	
	Splitter& vsplit = this->vsplit[mode];
	Plotter& plotter = this->plotter[mode];
	ArrayCtrl& list = this->list[mode];
	
	// Whole song
	if (db.ctx.active_wholesong) {
		list.SetCount(0);
		plotter.SetWholeSong(o);
		int total = 0;
		for(int i = 0; i < o.structure.GetCount(); i++) {
			String part_key = o.structure[i];
			int part_i = o.FindPartIdx(part_key);
			if (part_i < 0) continue;
			Part& part = o.parts[part_i];
			String part_name = db.attrs.Translate(part.name);
			
			for(int j = 0; j < part.lines.GetCount(); j++) {
				Line& line = part.lines[j];
				for(int k = 0; k < line.breaks.GetCount(); k++) {
					Break& brk = line.breaks[k];
					list.Set(total, 0, part_i);
					list.Set(total, 1, j);
					list.Set(total, 2, k);
					list.Set(total, 3, part_name + ":" + IntStr(j) + ":" + IntStr(k));
					for(int j = 0; j < brk.snap[mode].partscore.GetCount(); j++) {
						int value = brk.snap[mode].partscore[j];
						int k1 = group_begin+j;
						int k2 = group_begin-3+j; // skip 3 index columns for visible Ctrls
						list.Set(total, k1, value);
						
						Ctrl* c = new EditIntNotNullSpin;
						*c <<= THISBACK3(ListValueChanged, mode, total, j);
						list.SetCtrl(total, k2, c);
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
				for(int j = 0; j < brk.snap[mode].partscore.GetCount(); j++) {
					int value = brk.snap[mode].partscore[j];
					int k1 = group_begin+j;
					int k2 = group_begin-3+j; // skip 3 index columns for visible Ctrls
					list.Set(total, k1, value);
					
					Ctrl* c = new EditIntNotNullSpin;
					*c <<= THISBACK3(ListValueChanged, mode, total, j);
					list.SetCtrl(total, k2, c);
				}
				total++;
			}
		}
		list.SetCount(total);
	}
}

void ScoringCtrl::ListValueChanged(int mode, int pos, int scoring) {
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

void ScoringCtrl::ListMenu(Bar& bar) {
	//bar.Add(t_("Save as preset"), THISBACK(SavePreset));
	//bar.Add(t_("Update focused preset"), THISBACK(UpdatePreset));
}

#if 0
void ScoringCtrl::PresetMenu(Bar& bar) {
	bar.Add(t_("Add preset"), THISBACK(AddPreset));
	bar.Add(t_("Apply preset"), THISBACK(ApplyPreset));
	bar.Add(t_("Remove preset"), THISBACK(RemovePreset));
	
}

void ScoringCtrl::SavePreset(int mode) {
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
	
	int c = db.attrs.scorings.GetCount();
	Vector<int>& scores = a.presets.Add(name);
	scores.SetCount(c);
	
	int row = list.GetCursor();
	for(int i = 0; i < c; i++) {
		int j = group_begin + i;
		scores[i] = list.Get(row, j);
	}
	
	DataPresets();
}

void ScoringCtrl::UpdatePreset() {
	if (!list.IsCursor() || !presets.IsCursor())
		return;
	
	Database& db = Database::Single();
	AttrScore& a = db.attrscores;
	
	int c = db.attrs.scorings.GetCount();
	Vector<int>& scores = a.presets[presets.GetCursor()];
	scores.SetCount(c);
	
	int row = list.GetCursor();
	for(int i = 0; i < c; i++) {
		int j = group_begin + i;
		scores[i] = list.Get(row, j);
	}
	
	DataPresets();
}

void ScoringCtrl::ApplyPreset() {
	Database& db = Database::Single();
	Ptrs& p = db.ctx[MALE];
	if (!list.IsCursor() || !presets.IsCursor())
		return;
	if (!p.part)
		return;
	
	AttrScore& a = db.attrscores;
	
	int c = db.attrs.scorings.GetCount();
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

void ScoringCtrl::RemovePreset() {
	if (!presets.IsCursor())
		return;
	
	Database& db = Database::Single();
	AttrScore& a = db.attrscores;
	
	int cur = presets.GetCursor();
	
	a.presets.Remove(cur);
	
	DataPresets();
}

#endif
