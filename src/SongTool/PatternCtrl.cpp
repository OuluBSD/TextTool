#include "SongTool.h"


PatternCtrl::PatternCtrl() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << tree << vsplit;
	hsplit.SetPos(800);
	
	vsplit.Vert();
	for(const SnapArg& a : ModeArgs()) {
		AttrCtrl& attr = this->attr[a];
		Label& txt = this->txt[a];
		Ctrl& ctrl = this->ctrl[a];
		
		attr.SetMode(a);
		
		ctrl.Add(txt.HSizePos().TopPos(0,15));
		ctrl.Add(attr.HSizePos().VSizePos(115,0));
		vsplit << ctrl;
		
		//attr.WhenAction << THISBACK1(DataList, mode);
	}
	
	tree.WhenSel << THISBACK(OnTreeSel);
	//merge_owner <<= THISBACK(MergeOwner);
	
}

void PatternCtrl::Data() {
	for(SnapArg a : ModeArgs()) {
		AttrCtrl& attr = this->attr[a];
		if (use_rev_snap)
			a.InverseDir();
		attr.SetMode(a);
	}
	
	DataPatternTree();
	DataPatternSnapAll();
	DataListAll();
}

void PatternCtrl::DataPatternTree() {
	Database& db = Database::Single();
	Ptrs& p = db.ctx.p;
	if (!p.song)
		return;
	
	Song& o = *p.song;
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
		SnapContext& ctx = *tree_snaps[i];
		Ptrs& p = db.ctx.p;
		p.part = tree_parts[j];
		for(const SnapArg& a : AllArgs()) {
			db.ctx.snap[a] = &ctx.Get(a);
		}
		FocusListAll();
		DataListAll();
		DataPatternSnapAll();
	}
}

void PatternCtrl::OnListSel(const SnapArg& a) {
	Database& db = Database::Single();
	Ptrs& p = db.ctx.p;
	int cursor = list[a]->GetCursor();
	if (cursor >= 0 && cursor < level_snaps[a].GetCount()) {
		PatternSnap& snap = *level_snaps[a][cursor];
		db.ctx.snap[a] = &snap;
		FocusTree(a);
		DataPatternSnap(a);
	}
}

void PatternCtrl::DataPatternSnap(const SnapArg& a) {
	//NO! attr[mode].SetMode(mode);
	attr[a].Load();
	attr[a].Refresh();
}

void PatternCtrl::DataList(const SnapArg& a) {
	Database& db = Database::Single();
	Ptrs& p = db.ctx.p;
	
	if (!p.part || !db.ctx.snap[a])
		return;
	
	SnapArg src = a;
	src.InverseDir();
	int src_mode = src.Get();
	
	Label& txt = this->txt[a];
	One<ArrayCtrl>& list0 = this->list[a];
	Ctrl& ctrl = this->ctrl[a];
	
	Database& d = Database::Single();
	Attributes& g = d.attrs;
	if (g.groups.IsEmpty()) return;
	
	Part& part = *p.part;
	PatternSnap& s = *db.ctx.snap[a];
	int level = s.GetLevel();
	
	Vector<PatternSnap*>& level_snaps = this->level_snaps[a];
	Index<int>& group_types = this->group_types[a];
	VectorMap<int, Vector<int>>& group_items = this->group_items[a];
	
	// Update relate txt
	String line = s.txt;
	line.Replace("\r", "");
	line.Replace("\n", ";");
	txt.SetLabel(line);
	
	
	// Update columns
	
	// fucking ugly u++: column reset requires removing old ArrayCtrl
	int cursor = 0;
	if (list0) {
		cursor = list0->GetCursor();
		ctrl.RemoveChild(&*list0);
		list0.Clear();
	}
	ArrayCtrl& list = list0.Create();
	ctrl.Add(list.HSizePos().TopPos(15,100));
	
	level_snaps.SetCount(0);
	part.GetSnapsLevel(src, level, level_snaps);
	
	{
		group_items.Clear();
		group_types.Clear();
		part.Get(src).GetGroupItems(group_items);
		db.attrs.FindGroupTypes(group_items.GetKeys(), group_types);
		
		list.AddColumn(t_("Position"));
		String col_width_str = "2";
		for(int i = 0; i < group_items.GetCount(); i++) {
			int group_i = group_items.GetKey(i);
			const Vector<int>& items = group_items[i];
			if (!items.IsEmpty()) {
				bool has_items = false;
				for (PatternSnap* snap : level_snaps) {
					if (HasSnapGroupString(*snap, group_i)) {
						has_items = true;
						break;
					}
					PatternSnap* owner = snap->owner;
					while (owner) {
						if (HasSnapGroupString(*owner, group_i)) {
							has_items = true;
							break;
						}
						owner = owner->owner;
					}
				}
				if (has_items) {
					Attributes::Group& gg = g.groups[group_i];
					String key = g.Translate(gg.description);
					list.AddColumn(key);
					col_width_str << " 1";
				}
				else
					group_items.Remove(i--);
			}
		}
		list.ColumnWidths(col_width_str);
		
		attr[a].Layout();
	}
	
	list.SetCount(level_snaps.GetCount());
	Index<String> skip_list;
	for(int i = 0; i < level_snaps.GetCount(); i++) {
		PatternSnap& s0 = *level_snaps[i];
		String pos = s0.part->name + " :" + IntStr(s0.id);
		
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
				String s2 = GetSnapGroupString(*owner, group_i, skip_list);
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
		list.SetCursor(min(cursor, list.GetCount()));
	list.WhenSel << THISBACK1(OnListSel, a);
}

void PatternCtrl::MergeOwner() {
	Database& db = Database::Single();
	Ptrs& p = db.ctx.p;
	if (!p.part)
		return;
	p.part->MergeOwner();
	Data();
}

void PatternCtrl::FocusTree(const SnapArg& a) {
	SnapArg src = a;
	if (use_rev_snap) src.InverseDir();
	Database& db = Database::Single();
	Ptrs& p = db.ctx.p;
	for(int i = 0; i < tree_snaps.GetCount(); i++) {
		if (&tree_snaps[i]->Get(src) == db.ctx.snap[a]) {
			tree.SetCursor(tree_snaps.GetKey(i));
			break;
		}
	}
}

void PatternCtrl::FocusList(const SnapArg& a) {
	Database& db = Database::Single();
	Ptrs& p = db.ctx.p;
	for(int i = 0; i < level_snaps[a].GetCount(); i++) {
		if (level_snaps[a][i] == db.ctx.snap[a]) {
			list[a]->SetCursor(i);
			break;
		}
	}
}

void PatternCtrl::SelectLine(const SnapArg& match) {
	
	// Select same kay in other lists too
	if (match >= 0 && match < MODE_COUNT) {
		if (!this->list[match])
			return;
		ArrayCtrl& list = *this->list[match];
		if (list.IsCursor()) {
			String key = AttrText(list.Get(list.GetCursor(), 0)).text.ToString();
			for(const SnapArg& a : ModeArgs()) {
				if (a == match) continue;
				ArrayCtrl& list = *this->list[a];
				bool found = false;
				for(int j = 0; j < list.GetCount(); j++) {
					if (AttrText(list.Get(j, 0)).text.ToString() == key) {
						list.SetCursor(j);
						found = true;
						break;
					}
				}
				if (!found)
					list.KillCursor();
			}
		}
	}
	
	DataList(match);
}
