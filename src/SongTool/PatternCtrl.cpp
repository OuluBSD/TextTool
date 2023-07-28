#include "SongTool.h"


PatternCtrl::PatternCtrl() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << tree << vsplit;
	hsplit.SetPos(800);
	
	vsplit.Vert();
	for(int mode = 0; mode < GENDER_COUNT; mode++) {
		AttrCtrl& attr = this->attr[mode];
		Label& txt = this->txt[mode];
		Ctrl& ctrl = this->ctrl[mode];
		
		attr.SetMode(mode);
		
		ctrl.Add(txt.HSizePos().TopPos(0,15));
		ctrl.Add(attr.HSizePos().VSizePos(115,0));
		vsplit << ctrl;
		
		//attr.WhenAction << THISBACK1(DataList, mode);
	}
	
	tree.WhenSel << THISBACK(OnTreeSel);
	//merge_owner <<= THISBACK(MergeOwner);
	
}

void PatternCtrl::Data() {
	for(int mode = 0; mode < GENDER_COUNT; mode++) {
		AttrCtrl& attr = this->attr[mode];
		int src_mode = use_rev_snap ? mode + GENDER_COUNT : mode;
		attr.SetMode(src_mode);
	}
	
	DataPatternTree();
	DataPatternSnapAll();
	DataListAll();
}

void PatternCtrl::DataPatternTree() {
	Database& db = Database::Single();
	Ptrs& p = db.ctx[MALE];
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
		Part& part = *tree_parts[j];
		SnapContext& ctx = *tree_snaps[i];
		for(int k = 0; k < PTR_COUNT; k++) {
			Ptrs& p = db.ctx[k];
			p.part = &part;
			p.snap = &ctx.snap[k];
		}
		FocusListAll();
		DataListAll();
		DataPatternSnapAll();
	}
}

void PatternCtrl::OnListSel(int mode) {
	Database& db = Database::Single();
	Ptrs& p = db.ctx[mode];
	int cursor = list[mode]->GetCursor();
	if (cursor >= 0 && cursor < level_snaps[mode].GetCount()) {
		PatternSnap& snap = *level_snaps[mode][cursor];
		p.snap = &snap;
		FocusTree(mode);
		DataPatternSnap(mode);
	}
}

void PatternCtrl::DataPatternSnap(int mode) {
	//NO! attr[mode].SetMode(mode);
	attr[mode].Load();
	attr[mode].Refresh();
}

void PatternCtrl::DataList(int mode) {
	Database& db = Database::Single();
	Ptrs& p = db.ctx[mode];
	
	if (!p.part || !p.snap)
		return;
	
	int src_mode = use_rev_snap ? mode + GENDER_COUNT : mode;
	
	Label& txt = this->txt[mode];
	One<ArrayCtrl>& list0 = this->list[mode];
	Ctrl& ctrl = this->ctrl[mode];
	
	Database& d = Database::Single();
	Attributes& g = d.attrs;
	if (g.groups.IsEmpty()) return;
	
	Part& part = *p.part;
	PatternSnap& s = *p.snap;
	int level = s.GetLevel();
	
	Vector<PatternSnap*>& level_snaps = this->level_snaps[mode];
	Index<int>& group_types = this->group_types[mode];
	VectorMap<int, Vector<int>>& group_items = this->group_items[mode];
	
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
	part.GetSnapsLevel(src_mode, level, level_snaps);
	
	{
		group_items.Clear();
		group_types.Clear();
		part.snap[src_mode].GetGroupItems(group_items);
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
		
		attr[mode].Layout();
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
	list.WhenSel << THISBACK1(OnListSel, mode);
}

void PatternCtrl::MergeOwner() {
	Database& db = Database::Single();
	Ptrs& p = db.ctx[MALE];
	if (!p.part || !p.snap)
		return;
	p.part->MergeOwner();
	Data();
}

void PatternCtrl::FocusTree(int mode) {
	int src_mode = use_rev_snap ? mode + GENDER_COUNT : mode;
	Database& db = Database::Single();
	Ptrs& p = db.ctx[mode];
	for(int i = 0; i < tree_snaps.GetCount(); i++) {
		if (&tree_snaps[i]->snap[src_mode] == p.snap) {
			tree.SetCursor(tree_snaps.GetKey(i));
			break;
		}
	}
}

void PatternCtrl::FocusList(int mode) {
	Database& db = Database::Single();
	Ptrs& p = db.ctx[mode];
	for(int i = 0; i < level_snaps[mode].GetCount(); i++) {
		if (level_snaps[mode][i] == p.snap) {
			list[mode]->SetCursor(i);
			break;
		}
	}
}

void PatternCtrl::SelectLine(int match) {
	
	// Select same kay in other lists too
	if (match >= 0 && match < GENDER_COUNT) {
		if (!this->list[match])
			return;
		ArrayCtrl& list = *this->list[match];
		if (list.IsCursor()) {
			String key = AttrText(list.Get(list.GetCursor(), 0)).text.ToString();
			for(int i = 0; i < GENDER_COUNT; i++) {
				if (i == match) continue;
				ArrayCtrl& list = *this->list[i];
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
