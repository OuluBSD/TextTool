#include "SongTool.h"


PatternCtrl::PatternCtrl() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << vsplit0 << vsplit1;
	hsplit.SetPos(1500);
	
	vsplit0 << tree;
	
	vsplit1.Vert() << list << attr;
	vsplit1.SetPos(1000);
	
	tree.WhenSel << THISBACK(OnTreeSel);
	list.WhenSel << THISBACK(OnListSel);
	attr.WhenUpdate << THISBACK(DataList);
	//merge_owner <<= THISBACK(MergeOwner);
	
	list.AddColumn(t_("Position"));
	Database& db = Database::Single();
	Attributes& g = db.attrs;
	for(int i = 0; i < g.groups.GetCount(); i++) {
		Attributes::Group& gg = g.groups[i];
		String key = g.Translate(gg.description);
		list.AddColumn(key);
	}
}

void PatternCtrl::Data() {
	DataPatternTree();
	DataPatternSnap();
	DataList();
}

void PatternCtrl::DataPatternTree() {
	Database& db = Database::Single();
	if (!db.active_part)
		return;
	
	Song& o = *db.active_song;
	
	int cursor = tree.IsCursor() ? tree.GetCursor() : 0;
	
	tree.Clear();
	tree.SetRoot(AppImg::Root(), "Root");
	
	for(int i = 0; i < o.parts.GetCount(); i++) {
		Part& part = o.parts[i];
		String s = o.parts.GetKey(i) + " :" + IntStr(part.lines.GetCount());
		int id = tree.Add(0, AppImg::Part(), s);
		DataPatternTreeNode(part.snap, id);
	}
	tree.OpenDeep(0);
	tree.SetCursor(cursor);
}

void PatternCtrl::DataPatternTreeNode(PatternSnap& snap, int parent) {
	tree_snaps.Add(parent, &snap);
	
	if (snap.a) {
		String s = IntStr(snap.a->pos);
		if (snap.a->len > 1) s += ":" + IntStr(snap.a->len);
		int id = tree.Add(parent, AppImg::Snap(), s);
		DataPatternTreeNode(*snap.a, id);
	}
	if (snap.b) {
		String s = IntStr(snap.b->pos);
		if (snap.b->len > 1) s += ":" + IntStr(snap.b->len);
		int id = tree.Add(parent, AppImg::Snap(), s);
		DataPatternTreeNode(*snap.b, id);
	}
}

void PatternCtrl::OnTreeSel() {
	Database& db = Database::Single();
	int cursor = tree.GetCursor();
	int i = tree_snaps.Find(cursor);
	if (i >= 0) {
		PatternSnap& snap = *tree_snaps[i];
		db.active_snap = &snap;
		FocusList();
		DataPatternSnap();
		DataList();
	}
}

void PatternCtrl::OnListSel() {
	Database& db = Database::Single();
	int cursor = list.GetCursor();
	if (cursor >= 0 && cursor < level_snaps.GetCount()) {
		PatternSnap& snap = *level_snaps[cursor];
		db.active_snap = &snap;
		FocusTree();
		DataPatternSnap();
	}
}

void PatternCtrl::DataPatternSnap() {
	attr.Load();
	attr.Refresh();
}

void PatternCtrl::DataList() {
	Database& db = Database::Single();
	if (!db.active_part || !db.active_snap)
		return;
	
	Database& d = Database::Single();
	Attributes& g = d.attrs;
	
	Part& o = *db.active_part;
	PatternSnap& s = *db.active_snap;
	int level = s.GetLevel();
	
	level_snaps.SetCount(0);
	o.snap.GetSnapsLevel(level, level_snaps);
	
	list.SetCount(level_snaps.GetCount());
	Index<String> skip_list;
	for(int i = 0; i < level_snaps.GetCount(); i++) {
		PatternSnap& s0 = *level_snaps[i];
		String pos = s0.part->name + " :" + IntStr(s0.pos);
		if (s0.len > 1)
			pos += ":" + IntStr(s0.len);
		list.Set(i, 0, pos);
		
		for(int j = 0; j < g.GetCount(); j++) {
			skip_list.Clear();
			String s = GetSnapGroupString(s0, j, skip_list);
			PatternSnap* owner = s0.owner;
			String inherited;
			while (owner) {
				String s2 = GetSnapGroupString(*owner, j, skip_list);
				if (s2.GetCount()) {
					if (!inherited.IsEmpty()) inherited << ", ";
					inherited += s2;
				}
				owner = owner->owner;
			}
			if (inherited.GetCount()) {
				if (s.GetCount()) s << ", ";
				s << "[" << inherited << "]";
			}
			list.Set(i, 1 + j, s);
		}
		
	}
	
	if (!list.IsCursor())
		list.SetCursor(0);
}

void PatternCtrl::MergeOwner() {
	Database& db = Database::Single();
	if (!db.active_part || !db.active_snap)
		return;
	db.active_part->snap.MergeOwner();
	Data();
}

void PatternCtrl::FocusTree() {
	Database& db = Database::Single();
	for(int i = 0; i < tree_snaps.GetCount(); i++) {
		if (tree_snaps[i] == db.active_snap) {
			tree.SetCursor(tree_snaps.GetKey(i));
			break;
		}
	}
}

void PatternCtrl::FocusList() {
	Database& db = Database::Single();
	for(int i = 0; i < level_snaps.GetCount(); i++) {
		if (level_snaps[i] == db.active_snap) {
			list.SetCursor(i);
			break;
		}
	}
}

String GetSnapGroupString(PatternSnap& snap, int group_i, Index<String>& skip_list) {
	Attributes& g = Database::Single().attrs;
	Attributes::Group& gg = g.groups[group_i];
	
	String s;
	for(const SnapAttrStr& a : snap.attributes.GetKeys()) {
		ASSERT(a.has_id);
		if (a.group_i != group_i)
			continue;
		
		String item = g.Translate(gg.values[a.item_i]);
		
		if (skip_list.Find(item) < 0) {
			if (!s.IsEmpty())
				s << ", ";
			skip_list.Add(item);
			s << item;
		}
	}
	return s;
}

