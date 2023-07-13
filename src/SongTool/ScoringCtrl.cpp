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
	if (db.active_song) {
		if (db.active_wholesong) {
			list.SetCount(0);
			plotter.SetWholeSong(*db.active_song);
		}
		else {
			Part& p = *db.active_part;
			PartScore& part = p.score;
			plotter.SetPart(p.name, part);
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
	if (!db.active_part || !db.active_song)
		return;
	Part& part = *db.active_part;
	Song& o = *db.active_song;
	
	// Whole song
	if (db.active_wholesong) {
		list.SetCount(0);
		plotter.SetWholeSong(o);
		int total = 0;
		for(int i = 0; i < o.structure.GetCount(); i++) {
			String part_key = o.structure[i];
			int part_i = o.parts.Find(part_key);
			if (part_i < 0) continue;
			Part& part = o.parts[part_i];
			PartScore& ps = part.score;
			String part_name = db.attrs.Translate(part.name);
			
			for(int i = 0; i < ps.len; i++) {
				list.Set(total, 0, part_i);
				list.Set(total, 1, i);
				list.Set(total, 2, part_name + ":" + IntStr(i));
				for(int j = 0; j < ps.values.GetCount(); j++) {
					Vector<int>& d = ps.values[j];
					int value = i < d.GetCount() ? d[i] : 0;
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
		Part& p = *db.active_part;
		PartScore& part = p.score;
		plotter.SetPart(p.name, part);
		String part_name = db.attrs.Translate(p.name);
		int part_i = db.GetActivePartIndex();
		for(int i = 0; i < part.len; i++) {
			list.Set(i, 0, part_i);
			list.Set(i, 1, i);
			list.Set(i, 2, part_name + ":" + IntStr(i));
			for(int j = 0; j < part.values.GetCount(); j++) {
				Vector<int>& d = part.values[j];
				int value = i < d.GetCount() ? d[i] : 0;
				int k1 = group_begin+j;
				int k2 = group_begin-2+j; // skip 2 index columns for visible Ctrls
				list.Set(i, k1, value);
				
				Ctrl* c = new EditIntNotNullSpin;
				*c <<= THISBACK2(ListValueChanged, i, j);
				list.SetCtrl(i, k2, c);
			}
		}
		list.SetCount(part.len);
	}
}

void ScoringCtrl::ListValueChanged(int pos, int scoring) {
	Database& db = Database::Single();
	if (!db.active_part || !db.active_song)
		return;
	Part& p = *db.active_part;
	Song& o = *db.active_song;
	
	int part_i = list.Get(pos, 0);
	int i = list.Get(pos, 1);
	
	int value = list.Get(pos, group_begin+scoring);
	
	PartScore& part = p.score;
	
	if (scoring >= part.values.GetCount())
		part.values.SetCount(scoring+1);
	
	auto& v = part.values[scoring];
	if (i >= v.GetCount())
		v.SetCount(i+1, 0);
	
	v[i] = value;
	
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

void ScoringCtrl::GetScores(const PatternSnap& snap, Vector<int>& scores) {
	Database& db = Database::Single();
	const PatternSnap* s = &snap;
	int c = db.attrs.scorings.GetCount();
	scores.SetCount(c);
	for(auto& v : scores) v = 0;
	
	while (s) {
		for (const AttrScoreGroup& g : db.attrscores.groups) {
			SnapAttr a0;
			
			int match_count = 0;
			for (const SnapAttr& a0 : g.attrs) {
				for (const SnapAttrStr& a1 : s->attributes.GetKeys()) {
					if (a1 == a0)
						match_count++;
				}
			}
			
			if (match_count) {
				for(int i = 0; i < c; i++) {
					int sc = g.scores[i];
					if (0)
						sc = max(-1, min(+1, sc));
					scores[i] += match_count * sc;
				}
			}
		}
		
		s = s->owner;
	}
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
	if (!db.active_part)
		return;
	
	AttrScore& a = db.attrscores;
	
	int c = db.attrs.scorings.GetCount();
	Vector<int>& scores = a.presets[presets.GetCursor()];
	scores.SetCount(c);
	
	
	int cur = list.GetCursor();
	int part_i = list.Get(cur, 0);
	int pos = list.Get(cur, 1);
	
	Part& o = *db.active_part;
	PartScore& part = o.score;
	
	for(int i = 0; i < c; i++) {
		auto& vv = part.values[i];
		auto& v = vv[pos];
		v = scores[i];
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
