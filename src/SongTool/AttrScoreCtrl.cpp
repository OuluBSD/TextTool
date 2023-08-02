#include "SongTool.h"

AttrScoreCtrl::AttrScoreCtrl() {
	CtrlLayout(*this);
	
	vsplit.Vert();
	vsplit << attrscores << scoregroup_data << entries;
	
	mainsplit.Vert();
	mainsplit << entrygroup_split << part_errors;
	mainsplit.SetPos(7500);
	
	entrygroup_split.Horz();
	entrygroup_split << src_entrygroups << src_entries;
	
	attrscores.AddColumn(t_("Scoring"));
	attrscores.AddColumn(t_("Entry count"));
	attrscores.ColumnWidths("4 1");
	entries.AddColumn(t_("Group"));
	entries.AddColumn(t_("Entry"));
	src_entrygroups.AddColumn(t_("Group"));
	src_entrygroups.AddColumn(t_("Entry count"));
	src_entries.AddColumn(t_("Entry"));
	src_entries.AddColumn(t_("Scoring"));
	src_entrygroups.ColumnWidths("4 1");
	
	//part_errors.AddColumn(t_("Undefined entry"));
	part_errors.Columns(4);
	
	scoregroup_data.SetEditable(false);
	
	prompt_scores <<= THISBACK(OpenPromptScores);
	attrscores <<= THISBACK(DataAttrScore);
	add_scoregroup <<= THISBACK(AddAttrScoreGroup);
	add_entry.WhenAction = [this](){AddAttrScore();};
	src_entrygroups <<= THISBACK(DataGroup);
	src_entries <<= THISBACK(OnEntrySel);
	src_entries.WhenBar << THISBACK(SrcEntryMenu);
	
	
	attrwords.Title(t_("Attributes to words wizard"));
	attrwords.Sizeable();
	CtrlLayout(attrwords);
	attrwords.list.AddColumn(t_("Key"));
	attrwords.list.AddColumn(t_("Value"));
	attrwords.list.ColumnWidths("2 1");
}

void AttrScoreCtrl::SrcEntryMenu(Bar& b) {
	b.Add(t_("Add to scoring group"), THISBACK(AddSrcEntryToScoringGroup));
}

void AttrScoreCtrl::Data() {
	Database& db = Database::Single();
	for(int i = 0; i < db.attrscores.groups.GetCount(); i++) {
		AttrScoreGroup& a = db.attrscores.groups[i];
		attrscores.Set(i, 0, a.GetName());
		attrscores.Set(i, 1, a.attrs.GetCount());
	}
	attrscores.SetCount(db.attrscores.groups.GetCount());
	
	int cursor = max(0, db.GetActiveScoreGroupIndex());
	if (cursor >= 0 && cursor < db.attrscores.groups.GetCount())
		attrscores.SetCursor(cursor);
	
	DataAttrList();
	CheckErrors();
	
	if (attrscores.IsCursor())
		DataAttrScore();
	
}

void AttrScoreCtrl::DataAttrList() {
	Database& db = Database::Single();
	
	db.attrscores.UpdateGroupsToScoring();
	
	Attributes& g = db.attrs;
	for(int i = 0; i < g.groups.GetCount(); i++) {
		Attributes::Group& gg = db.attrs.groups[i];
		src_entrygroups.Set(i, 0, Capitalize(g.Translate(gg.description)));
		src_entrygroups.Set(i, 1, gg.values.GetCount());
		
	}
	
	if (!src_entrygroups.IsCursor() && src_entrygroups.GetCount())
		src_entrygroups.SetCursor(0);
	
	DataGroup();
}

void AttrScoreCtrl::DataGroup() {
	if (!src_entrygroups.IsCursor())
		return;
	
	Database& db = Database::Single();
	Attributes& g = db.attrs;
	int cursor = src_entrygroups.GetCursor();
	Attributes::Group& gg = db.attrs.groups[cursor];
	
	for(int i = 0; i < gg.values.GetCount(); i++) {
		String v = gg.values[i];
		src_entries.Set(i, 0, Capitalize(g.Translate(v)));
		
		String score_txt;
		int score_i = db.attrscores.attr_to_score[cursor][i];
		if (score_i >= 0)
			score_txt = db.attrscores.groups[score_i].GetName();
		/*else {
			DUMP(cursor);
			DUMP(i);
			DUMP(score_i);
			DUMP(v);
		}*/
		src_entries.Set(i, 1, Capitalize(score_txt));
	}
	src_entries.SetCount(gg.values.GetCount());
	
}

void AttrScoreCtrl::DataAttrScore() {
	if (!attrscores.IsCursor())
		return;
	
	int cursor = attrscores.GetCursor();
	String title = attrscores.Get(cursor, 0);
	
	Database& db = Database::Single();
	Ptrs& p = db.ctx.p;
	AttrScoreGroup& o = db.attrscores.groups[cursor];
	db.ctx.active_scoregroup = &o;
	
	//structure.SetData(o.structure);
	scoregroup_data.SetData(o.ToString());
	
	for(int i = 0; i < o.attrs.GetCount(); i++) {
		const SnapAttr& a = o.attrs[i];
		const auto& gr = db.attrs.groups[a.group];
		entries.Set(i, 0, Capitalize(db.attrs.Translate(gr.description)));
		entries.Set(i, 1, Capitalize(db.attrs.Translate(gr.values[a.item])));
	}
	entries.SetCount(o.attrs.GetCount());
	
	//DataPatternTree();
}
/*
void AttrScoreCtrl::AddAttrScoreGroup() {
	Database& db = Database::Single();
	int exp_count = db.attrs.scorings.GetCount();
	
	String score_str;
	bool b = EditTextNotNull(
		score_str,
		t_("Score string of the group"),
		t_("Score string"),
		0
	);
	if (!b) return;
	
	Vector<String> scores = Split(score_str, " ");
	if (scores.GetCount() != exp_count) {
		PromptOK(DeQtf(Format(t_("error: got %d scores (expected %d)"), scores.GetCount(), exp_count)));
		return;
	}
	
	Vector<int> score_ints;
	for (String& s : scores) {
		int i = StrInt(s);
		if (i < -5 || i > +5) {
			PromptOK(DeQtf(Format(t_("error: expected values between -5 and +5 (got %d)"), i)));
			return;
		}
		score_ints.Add(i);
	}
	
	AttrScoreGroup& g = db.attrscores.GetAdd(score_ints);
	
	db.active_scoregroup = &g;
	
	Data();
}
*//*
bool AttrScoreCtrl::AddAttrScore() {
	Database& db = Database::Single();
	
	if (!db.active_scoregroup)
		return false;
	
	String entry_str;
	bool b = EditTextNotNull(
		entry_str,
		t_("Entry name"),
		t_("Name of the entry"),
		0
	);
	if (!b) return false;
	
	return AddAttrScoreEntry("", entry_str);
}*/

bool AttrScoreCtrl::AddAttrScoreEntry(String group, String entry_str) {
	group = ToLower(group);
	
	Database& db = Database::Single();
	
	bool found = false;
	SnapAttr a;
	Attributes& g = db.attrs;
	String lname = ToLower(entry_str);
	for(int i = 0; i < g.groups.GetCount(); i++) {
		Attributes::Group& gg = g.groups[i];
		
		//LOG("'" << group << "' vs '" << ToLower(gg.description) << "'");
		if (group.GetCount() && ToLower(gg.description) != group)
			continue;
		
		for(int j = 0; j < gg.values.GetCount(); j++) {
			if (ToLower(gg.values[j]) == lname) {
				a.group = i;
				a.item = j;
				found = true;
				break;
			}
		}
		if (found) break;
	}
	
	if (!found) {
		PromptOK(DeQtf(Format(t_("Entry was not found ('%s')"), entry_str)));
		return false;
	}
	
	AddAttrScoreId(a);
	
	return true;
}

void AttrScoreCtrl::AddAttrScoreId(const SnapAttr& a) {
	Database& db = Database::Single();
	//bool found = false;
	for (AttrScoreGroup& ag : db.attrscores.groups) {
		for(int i = 0; i < ag.attrs.GetCount(); i++) {
			const SnapAttr& a0 = ag.attrs[i];
			if (a0 == a) {
				//PromptOK(DeQtf(Format(t_("Entry was already in the group '%s'"), ag.GetName())));
				//return;
				//found = true;
				LOG("AttrScoreCtrl::AddAttrScoreId: error: entry " << a.ToString() << " was already in a group: " << ag.GetName());
				ag.attrs.Remove(i--);
			}
		}
	}
	
	int active_idx = db.GetActiveScoreGroupIndex();
	//if (!found)
	db.ctx.active_scoregroup->attrs.Add(a);
	
	db.attrscores.attr_to_score.Clear();
	db.attrscores.UpdateGroupsToScoring();
	
	DUMP(db.attrscores.attr_to_score[5][0]);
	/*auto& vv = db.attrscores.attr_to_score;
	if (a.group >= vv.GetCount())
		vv.SetCount(a.group+1);
	auto& v = vv[a.group];
	if (a.item >= v.GetCount())
		v.SetCount(a.item+1, -1);
	v[a.item] = active_idx;*/
	
	DataAttrScore();
	DataGroup();
	CheckErrors();
}

void AttrScoreCtrl::AddSrcEntryToScoringGroup() {
	if (!src_entries.IsCursor() || !src_entrygroups.IsCursor())
		return;
	SnapAttr a;
	a.group = src_entrygroups.GetCursor();
	a.item = src_entries.GetCursor();
	AddAttrScoreId(a);
}

void AttrScoreCtrl::OnEntrySel() {
	
	
	
}

void AttrScoreCtrl::CheckErrors() {
	Database& db = Database::Single();
	Ptrs& p = db.ctx.p;
	Attributes& g = db.attrs;
	if (!p.part)
		return;
	
	db.attrscores.UpdateGroupsToScoring();
	
	Index<SnapAttrStr> attrs;
	
	Part& part = *p.part;
	
	part.GetSnapAttributes(p.mode, attrs);
	
	
	int i = 0;
	part_errors.Clear();
	for (const SnapAttrStr& a : attrs.GetKeys()) {
		a.RealizeId();
		int score = db.attrscores.attr_to_score[a.group_i][a.item_i];
		if (score >= 0)
			continue;
		
		const Attributes::Group& gg = db.attrs.groups[a.group_i];
		String key = gg.values[a.item_i];
		//part_errors.Set(i, 0, Capitalize(g.Translate(key)));
		part_errors.Add(Capitalize(g.Translate(key)));
		i++;
	}
	//part_errors.SetCount(i);
	
}

void AttrScoreCtrl::OpenPromptScores() {
	Database& db = Database::Single();
	Attributes& g = db.attrs;
	
	for(int i = 0; i < g.scorings.GetCount(); i++) {
		const Attributes::ScoringType& t = g.scorings[i];
		attrwords.list.Set(i, 0,
			Capitalize(g.Translate(t.klass)) + ": " +
			Capitalize(g.Translate(t.axes0)) + "/" +
			Capitalize(g.Translate(t.axes1))
		);
		attrwords.list.Set(i, 1, 0);
		attrwords.list.SetCtrl(i, 1, new EditIntNotNullSpin);
	}
	
	attrwords.groups.Clear();
	for(int i = 0; i < g.groups.GetCount(); i++) {
		attrwords.groups.Add(g.Translate(g.groups[i].description));
	}
	attrwords.groups.SetIndex(0);
	attrwords.add_task.WhenAction = THISBACK(AddAttrTask);
	
	attrwords.Open();
}

void AttrScoreCtrl::AddAttrScoreGroup() {
	Database& db = Database::Single();
	int exp_count = db.attrs.scorings.GetCount();
	
	String score_str;
	bool b = EditTextNotNull(
		score_str,
		t_("Score string of the group"),
		t_("Score string"),
		0
	);
	if (!b) return;
	
	Vector<String> scores = Split(score_str, " ");
	if (scores.GetCount() != exp_count) {
		PromptOK(DeQtf(Format(t_("error: got %d scores (expected %d)"), scores.GetCount(), exp_count)));
		return;
	}
	
	Vector<int> score_ints;
	for (String& s : scores) {
		int i = StrInt(s);
		if (i < -5 || i > +5) {
			PromptOK(DeQtf(Format(t_("error: expected values between -5 and +5 (got %d)"), i)));
			return;
		}
		score_ints.Add(i);
	}
	
	AttrScoreGroup& g = db.attrscores.GetAdd(score_ints);
	
	db.ctx.active_scoregroup = &g;
	
	Data();
}

bool AttrScoreCtrl::AddAttrScore() {
	Database& db = Database::Single();
	
	if (!db.ctx.active_scoregroup)
		return false;
	
	String entry_str;
	bool b = EditTextNotNull(
		entry_str,
		t_("Entry name"),
		t_("Name of the entry"),
		0
	);
	if (!b) return false;
	
	return AddAttrScoreEntry("", entry_str);
}

void AttrScoreCtrl::AddAttrTask() {
	PromptOK("TODO");
}
