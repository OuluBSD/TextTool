#include "SongTool.h"

ScoringCtrl::ScoringCtrl() {
	Add(mainsplit.SizePos());
	mainsplit.Vert();
	mainsplit << plotter << list << presets;
	
	Attributes& g = Database::Single().attrs;
	list.AddIndex();
	list.AddIndex();
	list.AddColumn(t_("Position"));
	presets.AddColumn(t_("Name"));
	for (Attributes::ScoringType& t : g.scorings) {
		list.AddColumn(
			//g.Translate(t.klass) + ": " +
			g.Translate(t.axes0) + "/" +
			g.Translate(t.axes1)
			);
		presets.AddColumn(
			//g.Translate(t.klass) + ": " +
			g.Translate(t.axes0) + "/" +
			g.Translate(t.axes1)
			);
	}
	
	//part_list.AddColumn(t_("Name"));
	
	list.WhenBar << THISBACK(ListMenu);
	presets.WhenBar << THISBACK(PresetMenu);
	presets.WhenLeftDouble << THISBACK(ApplyPreset);
	
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
	if (db.active.song) {
		if (db.active_wholesong) {
			plotter.SetWholeSong(*db.active.song);
		}
		else {
			Part& p = *db.active.part;
			plotter.SetPart(p);
		}
	}
	
	DataList();
	DataPresets();
}

void ScoringCtrl::DataPresets() {
	Database& db = Database::Single();
	for(int i = 0; i < db.attrscores.presets.GetCount(); i++) {
		String key = db.attrscores.presets.GetKey(i);
		const auto& values = db.attrscores.presets[i];
		presets.Set(i, 0, key);
		for(int j = 0; j < values.GetCount(); j++)
			presets.Set(i, 1+j, values[j]);
	}
	presets.SetCount(db.attrscores.presets.GetCount());
}

void ScoringCtrl::DataList() {
	Database& db = Database::Single();
	if (!db.active.song)
		return;
	Song& o = *db.active.song;
	
	// Whole song
	if (db.active_wholesong) {
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
				list.Set(total, 0, part_i);
				list.Set(total, 1, j);
				list.Set(total, 2, part_name + ":" + IntStr(j));
				for(int j = 0; j < line.partscore.GetCount(); j++) {
					int value = line.partscore[i];
					int k1 = group_begin+j;
					int k2 = group_begin-2+j; // skip 2 index columns for visible Ctrls
					list.Set(total, k1, value);
					
					Ctrl* c = new EditIntNotNullSpin;
					*c <<= THISBACK2(ListValueChanged, total, j);
					list.SetCtrl(total, k2, c);
				}
				total++;
			}
		}
		list.SetCount(total);
	}
	else {
		if (!db.active.part)
			return;
		Part& part = *db.active.part;
		plotter.SetPart(part);
		String part_name = db.attrs.Translate(part.name);
		int part_i = db.GetActivePartIndex();
		for(int i = 0; i < part.lines.GetCount(); i++) {
			Line& line = part.lines[i];
			list.Set(i, 0, part_i);
			list.Set(i, 1, i);
			list.Set(i, 2, part_name + ":" + IntStr(i));
			for(int j = 0; j < line.partscore.GetCount(); j++) {
				int value = line.partscore[j];
				int k1 = group_begin+j;
				int k2 = group_begin-2+j; // skip 2 index columns for visible Ctrls
				list.Set(i, k1, value);
				
				Ctrl* c = new EditIntNotNullSpin;
				*c <<= THISBACK2(ListValueChanged, i, j);
				list.SetCtrl(i, k2, c);
			}
		}
		list.SetCount(part.lines.GetCount());
	}
}

void ScoringCtrl::ListValueChanged(int pos, int scoring) {
	Database& db = Database::Single();
	if (!db.active.part || !db.active.song)
		return;
	Song& o = *db.active.song;
	
	int part_i = list.Get(pos, 0);
	int i = list.Get(pos, 1);
	
	Part& part = o.parts[part_i];
	if (part.partscore.GetCount() != db.attrs.scorings.GetCount())
		part.partscore.SetCount(db.attrs.scorings.GetCount(), 0);
	
	auto& dst = part.partscore[i];
	int value = list.Get(pos, group_begin+scoring);
	dst = value;
	
	// Refresh duplicate values (for whole song)
	for(int j = 0; j < list.GetCount(); j++) {
		int part_i0 = list.Get(j, 0);
		int i0 = list.Get(j, 1);
		if (j != pos && part_i0 == part_i && i == i0) {
			list.Set(j, group_begin+scoring, value);
		}
	}
	
	plotter.Refresh();
}

void ScoringCtrl::ListMenu(Bar& bar) {
	bar.Add(t_("Save as preset"), THISBACK(SavePreset));
	bar.Add(t_("Update focused preset"), THISBACK(UpdatePreset));
}

void ScoringCtrl::PresetMenu(Bar& bar) {
	bar.Add(t_("Add preset"), THISBACK(AddPreset));
	bar.Add(t_("Apply preset"), THISBACK(ApplyPreset));
	bar.Add(t_("Remove preset"), THISBACK(RemovePreset));
	
}

void ScoringCtrl::SavePreset() {
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
	if (!list.IsCursor() || !presets.IsCursor())
		return;
	if (!db.active.part)
		return;
	
	AttrScore& a = db.attrscores;
	
	int c = db.attrs.scorings.GetCount();
	Vector<int>& scores = a.presets[presets.GetCursor()];
	scores.SetCount(c);
	
	
	int cur = list.GetCursor();
	int part_i = list.Get(cur, 0);
	int pos = list.Get(cur, 1);
	
	Part& o = *db.active.part;
	Line& line = o.lines[pos];
	line.partscore.SetCount(c, 0);
	
	for(int i = 0; i < c; i++) {
		line.partscore[i] = scores[i];
	}
	
	DataList();
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
