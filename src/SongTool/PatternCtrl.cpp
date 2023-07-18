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
	Ptrs& p = db.ctx[MALE];
	Ptrs& p_rev = db.ctx[MALE_REVERSED];
	if ((!use_rev_snap && !p.song) || (use_rev_snap && !p_rev.song))
		return;
	
	Song& o = *(use_rev_snap ? p_rev.song : p.song);
	int cursor = tree.IsCursor() ? tree.GetCursor() : 0;
	
	tree_snaps.Clear();
	tree_parts.Clear();
	tree.Clear();
	tree.SetRoot(AppImg::Root(), "Root");
	
	for(int i = 0; i < o.parts.GetCount(); i++) {
		Part& part = o.parts[i];
		String s = part.name + " :" + IntStr(part.lines.GetCount());
		int id = tree.Add(0, AppImg::Part(), s);
		DataPatternTreeNode(part, part, id);
	}
	tree.OpenDeep(0);
	tree.SetCursor(cursor);
}

void PatternCtrl::OnTreeSel() {
	Database& db = Database::Single();
	int cursor = tree.GetCursor();
	int i = tree_snaps.Find(cursor);
	int j = tree_parts.Find(cursor);
	if (i >= 0 && j >= 0) {
		Part& part = *tree_parts[j];
		SnapContext& ctx = *tree_snaps[i];
		for(int k = 0; k < PTR_COUNT; k++) {
			Ptrs& p = db.ctx[k];
			p.part = &part;
			p.snap = &ctx.snap[k];
		}
		FocusList();
		DataList();
		DataPatternSnap();
	}
}

void PatternCtrl::OnListSel() {
	Database& db = Database::Single();
	Ptrs& p = db.ctx[MALE];
	int cursor = list->GetCursor();
	if (cursor >= 0 && cursor < level_snaps.GetCount()) {
		PatternSnap& snap = *level_snaps[cursor];
		p.snap = &snap;
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
	Ptrs& p = db.ctx[MALE];
	Ptrs& p_rev = db.ctx[MALE_REVERSED];
	if (!use_rev_snap && (!p.part || !p.snap))
		return;
	if (use_rev_snap && (!p.part || !p.snap))
		return;
	
	Database& d = Database::Single();
	Attributes& g = d.attrs;
	if (g.groups.IsEmpty()) return;
	
	Part& part = *(use_rev_snap ? p_rev.part : p.part);
	PatternSnap& s = *(use_rev_snap ? p_rev.snap : p.snap);
	int level = s.GetLevel();
	
	#if 0
	
	// Update relate txt
	String line = s.txt;
	line.Replace("\r", "");
	line.Replace("\n", ";");
	this->txt.SetLabel(line);
	
	
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
		part.GetGroupItems(group_items);
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
	part.GetSnapsLevel(level, level_snaps);
	
	list.SetCount(level_snaps.GetCount());
	Index<String> skip_list;
	for(int i = 0; i < level_snaps.GetCount(); i++) {
		PatternSnap& s0 = *level_snaps[i];
		String pos = s0.part->name + " :" + IntStr(s0.id);
		//if (s0.len > 1)
		//	pos += ":" + IntStr(s0.len);
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
	#endif
}

void PatternCtrl::MergeOwner() {
	Database& db = Database::Single();
	Ptrs& p = db.ctx[MALE];
	if (!p.part || !p.snap)
		return;
	p.part->MergeOwner();
	Data();
}

void PatternCtrl::FocusTree() {
	Database& db = Database::Single();
	Ptrs& p = db.ctx[MALE];
	for(int i = 0; i < tree_snaps.GetCount(); i++) {
		if (&tree_snaps[i]->snap[MALE] == p.snap) {
			tree.SetCursor(tree_snaps.GetKey(i));
			break;
		}
	}
}

void PatternCtrl::FocusList() {
	Database& db = Database::Single();
	Ptrs& p = db.ctx[MALE];
	for(int i = 0; i < level_snaps.GetCount(); i++) {
		if (level_snaps[i] == p.snap) {
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

