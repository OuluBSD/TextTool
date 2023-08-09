#include "SongTool.h"


PatternCtrl::PatternCtrl() {
	Add(ctrl.SizePos());
	
	ctrl.Add(txt.HSizePos().TopPos(0,15));
	ctrl.Add(attr.HSizePos().VSizePos(115,0));
	
	//attr.WhenAction << THISBACK1(DataList, mode);
	
	
	//merge_owner <<= THISBACK(MergeOwner);
	
}

void PatternCtrl::SetMode(const SnapArg& a) {
	this->a = a;
	attr.SetMode(a);
}

void PatternCtrl::Data() {
	DataPatternSnap();
	DataList();
}

void PatternCtrl::OnListSel() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	int cursor = list->GetCursor();
	if (cursor >= 0 && cursor < level_snaps.GetCount()) {
		PatternSnap& snap = *level_snaps[cursor];
		db.ctx.snap[a] = &snap;
		WhenFocusTree();
		DataPatternSnap();
		list->SetFocus();
	}
}

void PatternCtrl::DataPatternSnap() {
	//NO! attr[mode].SetMode(mode);
	attr.Load();
	attr.Refresh();
}

void PatternCtrl::DataList() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	Song& song = *p.song;
	if (!song.pipe) return;
	Pipe& pipe = *song.pipe;
	PipePtrs& pp = pipe.p;
	
	if (!pp.part || !db.ctx.snap[a])
		return;
	
	SnapArg src = a;
	//src.InverseDir();
	int src_mode = src.Get();
	
	One<ArrayCtrl>& list0 = this->list;
	
	Database& d = Database::Single();
	Attributes& g = pipe;
	if (g.attr_groups.IsEmpty()) return;
	
	Part& part = *pp.part;
	PatternSnap& s = *db.ctx.snap[a];
	int level = s.GetLevel();
	
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
		pipe.Attributes::FindGroupTypes(group_items.GetKeys(), group_types);
		
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
					Attr::Group& gg = g.attr_groups[group_i];
					String key = db.Translate(gg.description);
					list.AddColumn(key);
					col_width_str << " 1";
				}
				else
					group_items.Remove(i--);
			}
		}
		list.ColumnWidths(col_width_str);
		
		attr.Layout();
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
	list.WhenSel << THISBACK(OnListSel);
}

void PatternCtrl::MergeOwner() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	Song& song = *p.song;
	if (!song.pipe) return;
	Pipe& pipe = *song.pipe;
	PipePtrs& pp = pipe.p;
	if (!pp.part)
		return;
	pp.part->MergeOwner();
	Data();
}

void PatternCtrl::FocusList() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	for(int i = 0; i < level_snaps.GetCount(); i++) {
		if (level_snaps[i] == db.ctx.snap[a]) {
			list->SetCursor(i);
			break;
		}
	}
}

void PatternCtrl::SelectLine(const SnapArg& match) {
	
	// NOTE: this might not make sense at all
	
	// Select same kay in other lists too
	if (match >= 0 && match < MODE_COUNT) {
		if (!this->list)
			return;
		ArrayCtrl& list = *this->list;
		if (list.IsCursor()) {
			String key = AttrText(list.Get(list.GetCursor(), 0)).text.ToString();
			
			if (a == match) return;
			ArrayCtrl& list = *this->list;
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
	
	DataList();
}














PatternList::PatternList() {
	Add(hsplit.SizePos());
	
	hsplit.Horz();
	for (const SnapArg& a : ModeArgs()) {
		Splitter& vsplit = this->vsplit[a];
		ArrayCtrl& data = this->data[a];
		DocEdit& lyrics = this->lyrics[a];
		
		hsplit << vsplit;
		vsplit.Vert() << lyrics << data;
		vsplit.SetPos(2000);
		
		data.AddColumn(t_("Key"));
		data.AddColumn(t_("Value"));
		data.ColumnWidths("1 4");
		data.WhenAction << THISBACK1(SelectLine, a);
	}
	
}

void PatternList::Data() {
	for(const SnapArg& a : ModeArgs()) {
		Splitter& vsplit = this->vsplit[a];
		ArrayCtrl& data = this->data[a];
		DocEdit& lyrics = this->lyrics[a];
		String txt;
		
		SnapContext* ctx = 0;
		if (p.brk) {
			ctx = p.brk;
		}
		else if (p.line) {
			ctx = p.line;
		}
		else if (p.part) {
			ctx = p.part;
		}
		else if (p.pipe) {
			ctx = p.pipe;
			txt = p.pipe->content[a];
		}
		
		if (ctx) {
			PatternSnap& snap = ctx->snap[a];
			
			if (txt.IsEmpty()) txt = snap.txt;
			
			lyrics.SetData(txt);
			
			for(int j = 0; j < snap.attributes.GetCount(); j++) {
				const SnapAttrStr& sa = snap.attributes[j];
				
				bool in_all = true;
				for (const SnapArg& a0 : ModeArgs()) {
					if (&a == &a0) continue;
					PatternSnap& snap0 = ctx->Get(a0);
					if (snap0.attributes.Find(sa) < 0) {
						in_all = false;
						break;
					}
				}
				
				Color gclr = GetGenderColor(a.mode);
				Color clr = Blend(!in_all ? gclr : White(), White(), 256-32);
				
				data.Set(j, 0, AttrText(sa.group).NormalPaper(clr));
				data.Set(j, 1, AttrText(sa.item).NormalPaper(clr));
			}
			data.SetCount(snap.attributes.GetCount());
		}
		else {
			data.Clear();
		}
	}
}

void PatternList::SelectLine(const SnapArg& match) {
	
	// Select same kay in other lists too
	ArrayCtrl& list = this->data[match];
	if (list.IsCursor()) {
		String group = AttrText(list.Get(list.GetCursor(), 0)).text.ToString();
		String value = AttrText(list.Get(list.GetCursor(), 1)).text.ToString();
		for (const SnapArg& a : ModeArgs()) {
			if (a == match) continue;
			ArrayCtrl& list = this->data[a];
			bool found = false;
			for(int j = 0; j < list.GetCount(); j++) {
				String group0 = AttrText(list.Get(j, 0)).text.ToString();
				String value0 = AttrText(list.Get(j, 1)).text.ToString();
				if (group0 == group && value0 == value) {
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







PatternView::PatternView() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << tree << tabs;
	hsplit.SetPos(800);
	
	tree.WhenSel << THISBACK(OnTreeSel);
	
	this->a[0] = ZeroArg();
	
	tabs.Add(list.SizePos(), "List");
	for(int i = 0; i < MODE_COUNT; i++) {
		PatternCtrl& pc = pattern[i];
		
		tabs.Add(pc.SizePos(), GetModeString((SnapMode)i));
		pc.WhenFocusTree << THISBACK(FocusTree);
		
		SnapArg a;
		a.ctx = CTX_TEXT;
		a.mode = (SnapMode)i;
		a.dir = FORWARD;
		pc.SetMode(a);
		
		this->a[i+1] = a;
	}
	
	tabs.WhenSet << THISBACK(Data);
	
}

void PatternView::Data() {
	DataPatternTree();
	
	int tab = tabs.Get();
	switch(tab) {
		case 0: list.Data(); break;
		case 1: pattern[0].Data(); break;
		case 2: pattern[1].Data(); break;
		case 3: pattern[2].Data(); break;
		case 4: pattern[3].Data(); break;
		default: break;
	}
}

void PatternView::UseRev(){
	for(int i = 0; i < MODE_COUNT; i++) {
		SnapArg a;
		a.ctx = CTX_TEXT;
		a.mode = (SnapMode)i;
		a.dir = FORWARD;
		pattern[i].SetMode(a);
	}
}

void PatternView::DataPatternTree() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song || !p.song->pipe)
		return;
	Pipe& pipe = *p.song->pipe;
	int cursor = tree.IsCursor() ? tree.GetCursor() : 0;
	
	tree_snaps.Clear();
	tree_parts.Clear();
	tree.Clear();
	tree.SetRoot(AppImg::Root(), "Root");
	
	for(int i = 0; i < pipe.parts.GetCount(); i++) {
		Part& part = pipe.parts[i];
		String s = part.name + " :" + IntStr(part.lines.GetCount());
		int id = tree.Add(0, AppImg::Part(), s);
		DataPatternTreeNode(part, part, id);
	}
	tree.OpenDeep(0);
	tree.SetCursor(cursor);
}

void PatternView::OnTreeSel() {
	Database& db = Database::Single();
	EditorPtrs& p = db.ctx.ed;
	if (!p.song || !p.song->pipe)
		return;
	Pipe& pipe = *p.song->pipe;
	PipePtrs& pp = pipe.p;
	int cursor = tree.GetCursor();
	int i = tree_snaps.Find(cursor);
	int j = tree_parts.Find(cursor);
	if (!cursor || (i >= 0 && j >= 0)) {
		SnapContext& ctx = cursor == 0 ? pipe : *tree_snaps[i];
		pp.part = cursor == 0 ? 0 : tree_parts[j];
		
		for (int t = 0; t < MODE_COUNT+1; t++) {
			if (t >= 1 && t < MODE_COUNT+1) {
				const SnapArg& a = this->a[t];
				db.ctx.snap[a] = &ctx.Get(a);
				
				PatternCtrl& pc = pattern[t-1];
				pc.FocusList();
				pc.DataList();
				pc.DataPatternSnap();
			}
			else {
				list.p.CopyPtrs(ctx.Get(ZeroArg()));
				list.Data();
			}
		}
		tree.SetFocus();
	}
}

void PatternView::FocusTree() {
	int t = tabs.Get();
	if (t >= 1 && t < MODE_COUNT+1) {
		const SnapArg& a = this->a[t];
		
		Database& db = Database::Single();
		EditorPtrs& p = db.ctx.ed;
		for(int i = 0; i < tree_snaps.GetCount(); i++) {
			if (&tree_snaps[i]->Get(a) == db.ctx.snap[a]) {
				tree.SetCursor(tree_snaps.GetKey(i));
				tree.SetFocus();
				break;
			}
		}
	}
	else {
		
		//TODO
		
	}
}
