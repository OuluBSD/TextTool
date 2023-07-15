#include "SongTool.h"


PatternCtrl::PatternCtrl() {
	Add(txt.HSizePos(5).TopPos(0,20));
	Add(hsplit.HSizePos().VSizePos(20));
	
	hsplit.Horz() << vsplit0 << vsplit1;
	hsplit.SetPos(800);
	
	vsplit0 << tree;
	
	vsplit1.Vert() << list_placeholder << attr;
	vsplit1.SetPos(1000);
	
	tree.WhenSel << THISBACK(OnTreeSel);
	attr.WhenUpdate << THISBACK(DataList);
	//merge_owner <<= THISBACK(MergeOwner);
	
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
	
	tree_snaps.Clear();
	tree_parts.Clear();
	tree.Clear();
	tree.SetRoot(AppImg::Root(), "Root");
	
	for(int i = 0; i < o.parts.GetCount(); i++) {
		Part& part = o.parts[i];
		String s = o.parts.GetKey(i) + " :" + IntStr(part.lines.GetCount());
		int id = tree.Add(0, AppImg::Part(), s);
		DataPatternTreeNode(part, use_rev_snap ? part.rev_snap : part.snap, id);
	}
	tree.OpenDeep(0);
	tree.SetCursor(cursor);
}

void PatternCtrl::DataPatternTreeNode(Part& part, PatternSnap& snap, int parent) {
	tree_snaps.Add(parent, &snap);
	tree_parts.Add(parent, &part);
	
	if (snap.a) {
		String s = IntStr(snap.a->pos);
		if (snap.a->len > 1) s += ":" + IntStr(snap.a->len);
		int id = tree.Add(parent, AppImg::Snap(), s);
		DataPatternTreeNode(part, *snap.a, id);
	}
	if (snap.b) {
		String s = IntStr(snap.b->pos);
		if (snap.b->len > 1) s += ":" + IntStr(snap.b->len);
		int id = tree.Add(parent, AppImg::Snap(), s);
		DataPatternTreeNode(part, *snap.b, id);
	}
}

void PatternCtrl::OnTreeSel() {
	Database& db = Database::Single();
	int cursor = tree.GetCursor();
	int i = tree_snaps.Find(cursor);
	int j = tree_parts.Find(cursor);
	if (i >= 0 && j >= 0) {
		Part& part = *tree_parts[j];
		PatternSnap& snap = *tree_snaps[i];
		db.active_part = &part;
		db.active_snap = &snap;
		FocusList();
		DataList();
		DataPatternSnap();
	}
}

void PatternCtrl::OnListSel() {
	Database& db = Database::Single();
	int cursor = list->GetCursor();
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
	if (g.groups.IsEmpty()) return;
	
	Part& p = *db.active_part;
	PatternSnap& s = *db.active_snap;
	int level = s.GetLevel();
	
	
	// Update relate txt
	Vector<PatternSnap*> l0;
	s.GetSnapsLevel(0, l0);
	String txt;
	for (PatternSnap* s : l0) {
		if (!txt.IsEmpty()) txt << "; ";
		txt << s->txt;
	}
	this->txt.SetLabel(txt);
	
	
	// Update columns
	
	// fucking ugly u++: column reset requires removing old ArrayCtrl
	if (this->list) {
		list_placeholder.RemoveChild(&*this->list);
		this->list.Clear();
	}
	ArrayCtrl& list = this->list.Create();
	list_placeholder.Add(list.SizePos());
	
	{
		group_items.Clear();
		group_types.Clear();
		p.mask.GetGroupItems(group_items);
		db.attrs.FindGroupTypes(group_items.GetKeys(), group_types);
		
		list.AddColumn(t_("Position"));
		
		for(int i = 0; i < group_items.GetCount(); i++) {
			int group_i = group_items.GetKey(i);
			const Vector<int>& items = group_items[i];
			if (!items.IsEmpty()) {
				Attributes::Group& gg = g.groups[group_i];
				String key = g.Translate(gg.description);
				list.AddColumn(key);
			}
		}
		
		attr.Layout();
	}
	
	level_snaps.SetCount(0);
	(use_rev_snap ? p.rev_snap : p.snap).GetSnapsLevel(level, level_snaps);
	
	list.SetCount(level_snaps.GetCount());
	Index<String> skip_list;
	for(int i = 0; i < level_snaps.GetCount(); i++) {
		PatternSnap& s0 = *level_snaps[i];
		String pos = s0.part->name + " :" + IntStr(s0.pos);
		if (s0.len > 1)
			pos += ":" + IntStr(s0.len);
		list.Set(i, 0, pos);
		
		for(int j = 0, k = 1; j < group_items.GetCount(); j++) {
			int group_i = group_items.GetKey(j);
			const Vector<int>& items = group_items[j];
			if (items.IsEmpty()) continue;
			
			skip_list.Clear();
			String s = GetSnapGroupString(s0, group_i, skip_list);
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
			list.Set(i, k, s);
			k++;
		}
		
	}
	
	if (!list.IsCursor())
		list.SetCursor(0);
	list.WhenSel << THISBACK(OnListSel);
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
			list->SetCursor(i);
			break;
		}
	}
}

String GetSnapGroupString(PatternSnap& snap, int group_i, Index<String>& skip_list) {
	Attributes& g = Database::Single().attrs;
	Attributes::Group& gg = g.groups[group_i];
	
	String s;
	for(const SnapAttrStr& a : snap.attributes.GetKeys()) {
		a.RealizeId();
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

