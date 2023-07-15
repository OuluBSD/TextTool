#include "SongTool.h"

AttrCtrl::AttrCtrl() {
	sel.b = -1;
	sel.c = -1;
	pressed.b = -1;
	pressed.c = -1;
	
	RealizeTemp();
}

void AttrCtrl::RealizeTemp() {
	Attributes& g = Database::Single().attrs;
	active.SetCount(g.GetCount() * g.group_limit, false);
	inherited_active.SetCount(g.GetCount() * g.group_limit, false);
}

void AttrCtrl::Load() {
	Database& db = Database::Single();
	Attributes& g = db.attrs;
	if (!db.active_snap)
		return;
	
	for (bool& b : active) b = false;
	for (const SnapAttrStr& a : db.active_snap->attributes) {
		int id = a.group_i * g.group_limit + a.item_i;
		ASSERT(id >= 0 && id < active.GetCount());
		if (id >= 0 && id < active.GetCount())
			active[id] = true;
	}
	
	for (bool& b : inherited_active) b = false;
	PatternSnap* owner = db.active_snap->owner;
	while (owner) {
		for (const SnapAttrStr& a : owner->attributes) {
			int id = a.group_i * g.group_limit + a.item_i;
			ASSERT(id >= 0 && id < inherited_active.GetCount());
			if (id >= 0 && id < inherited_active.GetCount())
				inherited_active[id] = true;
		}
		owner = owner->owner;
	}
}

void AttrCtrl::Store() {
	Database& db = Database::Single();
	Attributes& g = db.attrs;
	if (!db.active_snap)
		return;
	
	db.active_snap->attributes.Clear();
	
	int id = 0;
	for (bool& b : active) {
		if (b) {
			SnapAttrStr attr;
			attr.item_i = id % g.group_limit;
			attr.group_i = id / g.group_limit;
			attr.RealizeId();
			db.active_snap->attributes.Add(attr);
		}
		id++;
	}
}

void AttrCtrl::Paint(Draw& d) {
	Database& db = Database::Single();
	Attributes& g = db.attrs;
	Color bg = GrayColor(32);
	Size sz = GetSize();
	
	d.DrawRect(sz, bg);
	
	if (!db.active_part)
		return;
	Part& p = *db.active_part;
	
	int tgt_lineh = 18;
	Font fnt = SansSerif(15);
	
	
	
	
	/*int item_count = g.GetItemCount();
	if (!item_count)
		return;
	int item_total_h = item_count * tgt_lineh;
	double colsf = (double)item_total_h / (double)sz.cy;
	int cols = ceil(colsf) + 2;
	if (0) {
		DUMP(item_count);
		DUMP(item_total_h);
		DUMP(sz.cy);
		DUMP(colsf);
		DUMP(cols);
		LOG("");
	}*/
	
	// Make grid of groups
	if (grid.IsEmpty() || invalidate_group_grid) {
		invalidate_group_grid = false;
		
		{
			group_items.Clear();
			group_types.Clear();
			p.mask.GetGroupItems(group_items);
			db.attrs.FindGroupTypes(group_items.GetKeys(), group_types);
		}
		
		// Sort by group type first
		if (1) {
			int h_limit = sz.cy / 40;
			
			grid.Clear();
			VectorMap<int, Vector<int>>* cur_col = &grid.Add();
			int col_items = 0;
			for (int group_type : group_types.GetKeys()) {
				for(int i = 0; i < group_items.GetCount(); i++) {
					int group_i = group_items.GetKey(i);
					const Attributes::Group& gg = g.groups[group_i];
					ASSERT(gg.type_i >= 0);
					if (gg.type_i != group_type)
						continue;
					
					const Vector<int>& items = group_items[i];
					int new_col_items = col_items + items.GetCount();
					if (new_col_items >= h_limit && !cur_col->IsEmpty()) {
						cur_col = &grid.Add();
						new_col_items = items.GetCount();
					}
					col_items = new_col_items;
					Vector<int>& vals = cur_col->Add(group_i);
					vals <<= items;
				}
			}
		}
		// Sort by adding small first
		/*else {
			group_grid.SetCount(cols);
			for (auto& v : group_grid) v.SetCount(0);
			
			VectorMap<int,int> group_sizes;
			for(int i = 0; i < groups.GetCount(); i++) {
				int group_i = groups[i];
				group_sizes.Add(group_i, g.groups[group_i].values.GetCount());
			}
			SortByValue(group_sizes, StdLess<int>());
			
			int col = 0;
			bool add = true;
			for(int i = 0; i < group_sizes.GetCount(); i++) {
				int group = group_sizes.GetKey(i);
				group_grid[col].Add(group);
				if (add) {
					col++;
					if (col >= cols) {
						col = cols-1;
						add = false;
					}
				}
				else {
					col--;
					if (col < 0) {
						col = 0;
						add = true;
					}
				}
			}
		}*/
	}
	
	group_title_rects.SetCount(0);
	entry_rects.SetCount(0);
	
	int div = grid.GetCount();
	double cx = min(200.0, (double)sz.cx / div);
	for(int col = 0; col < grid.GetCount(); col++) {
		int x = col * cx;
		int y = 0;
		const VectorMap<int,Vector<int>>& col_groups = grid[col];
		int groups = col_groups.GetCount();
		int items = col_groups.GetCount();
		for (const Vector<int>& group : col_groups.GetValues())
			items += group.GetCount();
		if (!items)
			continue;
		int lineh = min<int>(tgt_lineh * 1.33, sz.cy / items);
		for(int j = 0; j < col_groups.GetCount(); j++) {
			int group_i = col_groups.GetKey(j);
			PaintKeys(d, group_i, col_groups[j], x, cx, y, lineh, fnt);
		}
	}
	
	
}

void AttrCtrl::PaintKeys(Draw& d, int group, const Vector<int>& items, int x, int cx, int& y, float lineh, Font fnt) {
	Database& db = Database::Single();
	Attributes& g = db.attrs;
	Attributes::Group& gg = g.groups[group];
	int x0 = x;
	int y0 = y;
	int x1 = x + cx;
	int key_count = items.GetCount() + 1;
	int ht = key_count * lineh;
	int y1 = y + ht;
	const int off = 1;
	Color clr = gg.clr;
	
	if (gg.type_i >= 0)
		clr = db.attrs.group_types[gg.type_i].clr;
	
	// realize snapshots
	int snap_size = g.groups.GetCount() * g.group_limit;
	active.SetCount(snap_size, false);
	inherited_active.SetCount(snap_size, false);
	
	for(int i = 0; i < key_count; i++) {
		int y1 = y0 + i * lineh;
		
		Rect r = RectC(x0, y1, cx, lineh);
		d.Clip(r);
		
		Color bg_clr, stripe_clr;
		bool stripes = false;
		String txt;
		Font txt_fnt = fnt;
		int xoff = 4;
		if (!i) {
			bg_clr = Black(); //clr;
			txt = g.Translate(gg.description);
			txt_fnt.Bold();
			Size sz = GetTextSize(txt, txt_fnt);
			xoff = (cx - sz.cx) / 2;
			
			RectId& rid = group_title_rects.Add();
			rid.a = r;
			rid.b = group;
			rid.c = -1;
		}
		else {
			int j = items[i-1];
			txt = g.Translate(gg.values[j]);
			int id = group * g.group_limit + j;
			bool active = this->active[id];
			bool inherited_active = this->inherited_active[id];
			
			// Mouse hovers
			if (group == pressed.b && j == pressed.c)
				bg_clr = Blend(clr, White(), 0.8*255);
			else if (group == sel.b && j == sel.c)
				bg_clr = clr;
			else if (active)
				bg_clr = Blend(clr, GrayColor(64), 0.3*255);
			else if (inherited_active) {
				bg_clr = Blend(clr, GrayColor(64), 0.97*255);
				stripe_clr = Blend(clr, GrayColor(64), 0.3*255);
				stripes = true;
			}
			else
				bg_clr = Blend(clr, GrayColor(64), 0.97*255);
			
			RectId& rid = entry_rects.Add();
			rid.a = r;
			rid.b = group;
			rid.c = j;
		}
		
		d.DrawRect(r, bg_clr);
		if (stripes) {
			int sw = 2;
			for (int x = x0; x <= x1; x += sw * 2) {
				Rect r = RectC(x, y1, sw, lineh+1);
				d.DrawRect(r, stripe_clr);
			}
		}
		
		// Capitalize first letter
		txt = ToUpper(txt.Left(1)) + txt.Mid(1);
		
		d.DrawText(xoff+ x0 + off + 1, y1 + off + 1, txt, txt_fnt, Black());
		d.DrawText(xoff+ x0 + off, y1 + off, txt, txt_fnt, White());
		d.End();
	}
	
	
	Color line_clr = GrayColor();
	d.DrawLine(x0, y0, x0, y1, 1, line_clr);
	d.DrawLine(x1, y0, x1, y1, 1, line_clr);
	for(int i = 0; i <= key_count; i++) {
		int y2 = y0 + i * lineh;
		d.DrawLine(x0, y2, x1, y2, 1, line_clr);
	}
	
	y += key_count * lineh;
}

void AttrCtrl::MouseMove(Point p, dword keyflags) {
	bool found = false;
	for(RectId& rid : entry_rects) {
		if (rid.a.Contains(p)) {
			bool r = sel.b != rid.b || sel.c != rid.c;
			sel = rid;
			if (r)
				Update();
			found = true;
			break;
		}
	}
	if (!found) {
		bool r = sel.b != -1;
		sel.b = -1;
		sel.c = -1;
		if (r)
			Update();
	}
}

void AttrCtrl::MouseLeave() {
	if (sel.b >= 0) {
		sel.b = -1;
		sel.c = -1;
		Update();
	}
}

void AttrCtrl::LeftDown(Point p, dword keyflags) {
	Database& db = Database::Single();
	Attributes& g = db.attrs;
	for(RectId& rid : entry_rects) {
		if (rid.a.Contains(p)) {
			SnapAttrStr a;
			a.group_i = rid.b;
			a.item_i = rid.c;
			a.RealizeId();
			pressed = rid;
			int id = a.group_i * g.group_limit + a.item_i;
			if (id >= 0 && id < active.GetCount()) {
				bool& b = active[id];
				if (db.active_snap) {
					b = !b;
					if (!b) {
						int i = db.active_snap->attributes.Find(a);
						ASSERT(i >= 0);
						db.active_snap->attributes.Remove(i);
					}
					else {
						db.active_snap->attributes.Add(a);
					}
					Update();
				}
			}
			break;
		}
	}
}

void AttrCtrl::LeftUp(Point, dword keyflags) {
	pressed.b = -1;
	pressed.c = -1;
	Update();
}

void AttrCtrl::RightDown(Point p, dword keyflags) {
	MenuBar::Execute(THISBACK1(ContextMenu, p));
}

void AttrCtrl::Update() {
	Refresh();
	WhenUpdate();
}

void AttrCtrl::ContextMenu(Bar& bar, Point pt) {
	bar.Add(t_("Add group"), THISBACK(AddGroup));
	
	for (RectId& rid : group_title_rects) {
		if (rid.a.Contains(pt)) {
			Attributes& g = Database::Single().attrs;
			String group = g.Translate(g.groups[rid.b].description);
			bar.Add(Format(t_("Add entry to group '%s'"), group), THISBACK1(AddEntry, rid.b));
		}
	}
}

void AttrCtrl::AddGroup() {
	Attributes& g = Database::Single().attrs;
	
	// Name
	String name;
	bool b = EditTextNotNull(
		name,
		"Add group",
		"Group's codename",
		0
	);
	if (!b) return;
	name = ToLower(name);
	
	// Description
	String desc;
	b = EditTextNotNull(
		desc,
		"Explain group",
		"Group's description",
		0
	);
	if (!b) return;
	
	// Translation
	String trans;
	if (g.trans_i >= 0) {
		b = EditTextNotNull(
			trans,
			t_("Translation of the group"),
			t_("Group's translation"),
			0
		);
		if (!b) return;
	}
	
	int i = g.FindGroup(name);
	if (i >= 0) {
		PromptOK(DeQtf(Format(t_("Group '%s' exists already"), name)));
		return;
	}
	
	String type;
	b = EditTextNotNull(
		type,
		t_("Type of the group (e.g. language)"),
		t_("Type of the group"),
		0
	);
	if (!b) return;
	
	Attributes::Group& gg = g.AddGroup(type, name);
	gg.clr = Color(Random(256), Random(256), Random(256));
	gg.description = desc;
	
	if (g.trans_i >= 0 && trans.GetCount()) {
		Attributes::Translation& t = g.translation[g.trans_i];
		t.data.Add(desc, trans);
	}
	
	invalidate_group_grid = true;
	Refresh();
	WhenUpdate();
}

void AttrCtrl::AddEntry(int group) {
	Attributes& g = Database::Single().attrs;
	if (group < 0 || group >= g.groups.GetCount())
		return;
	
	Attributes::Group& gg = g.groups[group];
	
	// Description
	String desc;
	bool b = EditTextNotNull(
		desc,
		"Explain entry",
		"Entry's description",
		0
	);
	if (!b) return;
	
	desc = ToLower(desc);
	
	// Translation
	String trans;
	if (g.trans_i >= 0) {
		b = EditTextNotNull(
			trans,
			t_("Translation of the entry"),
			t_("Entry's translation"),
			0
		);
		if (!b) return;
	}
	
	if (gg.HasValue(desc)) {
		PromptOK(DeQtf(Format(t_("Entry '%s' exists already"), desc)));
		return;
	}
	
	gg.values.Add(desc);
	
	if (g.trans_i >= 0 && trans.GetCount()) {
		Attributes::Translation& t = g.translation[g.trans_i];
		t.data.Add(desc, trans);
	}
	
	invalidate_group_grid = true;
	Refresh();
	WhenUpdate();
}