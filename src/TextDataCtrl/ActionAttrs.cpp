#include "TextDataCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


ActionAttrsPage::ActionAttrsPage() {
	Add(hsplit.VSizePos(0,30).HSizePos());
	Add(prog.BottomPos(0,30).HSizePos(300));
	Add(remaining.BottomPos(0,30).LeftPos(0,300));
	
	hsplit.Horz() << vsplit << actions;
	hsplit.SetPos(2000);
	
	vsplit.Vert() << attrs << colors;
	
	attrs.AddColumn(t_("Group"));
	attrs.AddColumn(t_("Value"));
	attrs.AddIndex("GROUP");
	attrs.AddIndex("VALUE");
	attrs.ColumnWidths("1 1");
	attrs.WhenCursor << THISBACK(DataAttribute);
	
	colors.AddColumn(t_("Colors"));
	colors.WhenCursor << THISBACK(DataColor);
	
	actions.AddColumn(t_("Group"));
	actions.AddColumn(t_("Value"));
	actions.AddColumn(t_("Action"));
	actions.AddColumn(t_("Action arg"));
	actions.AddIndex("IDX");
	actions.ColumnWidths("2 2 4 4");
}

void ActionAttrsPage::ToolMenu(Bar& bar) {
	bar.Add(t_("Update Data"), AppImg::BlueRing(), THISBACK(Data)).Key(K_CTRL_Q);
	bar.Separator();
	bar.Add(t_("Start"), AppImg::RedRing(), THISBACK1(Do, 0)).Key(K_F5);
	bar.Add(t_("Stop"), AppImg::RedRing(), THISBACK1(Do, 1)).Key(K_F6);
	#if 0
	bar.Add(t_("Update data"), AppImg::BlueRing(), THISBACK(Data)).Key(K_CTRL_Q);
	bar.Separator();
	//bar.Add(t_("Update from cache"), AppImg::RedRing(), THISBACK(UpdateFromCache)).Key(K_F5);
	bar.Add(t_("Update action colors using existing"), AppImg::VioletRing(), THISBACK1(DoActionlistUsingExisting, 0)).Key(K_F5);
	bar.Add(t_("Update action colors"), AppImg::RedRing(), THISBACK1(DoActionlist, 0)).Key(K_F6);
	bar.Separator();
	bar.Add(t_("Update action attributes using existing"), AppImg::VioletRing(), THISBACK1(DoActionlistUsingExisting, 1)).Key(K_F7);
	bar.Add(t_("Update action attributes"), AppImg::RedRing(), THISBACK1(DoActionlist, 1)).Key(K_F8);
	#endif
}

void ActionAttrsPage::Data() {
	int gi = 0;
	int i = 0;
	
	attrs.Set(i, 0, "All");
	attrs.Set(i, 1, "All");
	attrs.Set(i, "GROUP", -1);
	attrs.Set(i, "VALUE", -1);
	i++;
	
	#define ATTR_ITEM(e, g, i0, i1) \
		attrs.Set(i, 0, g); \
		attrs.Set(i, 1, i0); \
		attrs.Set(i, "GROUP", gi); \
		attrs.Set(i, "VALUE", 0); \
		i++; \
		attrs.Set(i, 0, g); \
		attrs.Set(i, 1, i1); \
		attrs.Set(i, "GROUP", gi); \
		attrs.Set(i, "VALUE", 1); \
		i++, gi++;
	ATTR_LIST
	#undef ATTR_ITEM

	if (!attrs.IsCursor() && attrs.GetCount())
		attrs.SetCursor(0);
	
	
	DataAttribute();
}

void ActionAttrsPage::DataAttribute() {
	if (!attrs.IsCursor())
		return;
	
	
	
	colors.SetCount(1+GetColorGroupCount());
	colors.Set(0, 0, t_("All words"));
	for(int i = 0; i < GetColorGroupCount(); i++) {
		colors.Set(1+i, 0,
			AttrText("#" + IntStr(i))
				.NormalPaper(GetGroupColor(i)).NormalInk(Black())
				.Paper(Blend(GrayColor(), GetGroupColor(i))).Ink(White()));
	}
	if (colors.GetCount() && !colors.IsCursor())
		colors.SetCursor(0);
	
	
	DataColor();
}

void ActionAttrsPage::DataColor() {
	if (!colors.IsCursor() || !attrs.IsCursor())
		return;
	
	TextDatabase& db = GetDatabase();
	SourceData& sd = db.src_data;
	SourceDataAnalysis& sda = db.src_data.a;
	DatasetAnalysis& da = sda.dataset;
	
	int clr_i = colors.GetCursor();
	int attr_group_i = attrs.Get("GROUP");
	int attr_value_i = attrs.Get("VALUE");
	String group_str = attr_group_i >= 0 ? ToLower(Attr::AttrKeys[attr_group_i][1]) : String();
	String value_str = attr_group_i >= 0 ? ToLower(Attr::AttrKeys[attr_group_i][2 + attr_value_i]) : String();
	
	bool clr_filter = clr_i > 0;
	bool attr_filter = attr_group_i >= 0;
	clr_i--;
	
	int row = 0, max_rows = 10000;
	for(int i = 0; i < da.actions.GetCount(); i++) {
		const ActionHeader& ah = da.actions.GetKey(i);
		ExportAction& aa = da.actions[i];
		
		// Filter by color group
		if (clr_filter && GetColorGroup(aa.clr) != clr_i)
			continue;
		
		// Filter by attribute
		String g, v;
		if (attr_filter) {
			if (aa.attr < 0)
				continue;
			const AttrHeader& ath = da.attrs.GetKey(aa.attr);
			if (ath.group != group_str || ath.value != value_str)
				continue;
		}
		
		
		actions.Set(row, "IDX", i);
		if (aa.attr >= 0) {
			const AttrHeader& ath = da.attrs.GetKey(aa.attr);
			actions.Set(row, 0, ath.group);
			actions.Set(row, 1, ath.value);
		}
		else {
			actions.Set(row, 0, Value());
			actions.Set(row, 1, Value());
		}
		actions.Set(row, 2,
			AttrText(ah.action)
				.NormalPaper(Blend(aa.clr, White(), 128+64)).NormalInk(Black())
				.Paper(Blend(aa.clr, GrayColor())).Ink(White())
			);
		actions.Set(row, 3,
			AttrText(ah.arg)
				.NormalPaper(Blend(aa.clr, White(), 128+64)).NormalInk(Black())
				.Paper(Blend(aa.clr, GrayColor())).Ink(White())
			);
		row++;
		if (row >= max_rows)
			break;
	}
	actions.SetCount(row);
	actions.SetSortColumn(2);
	
}

void ActionAttrsPage::Do(int fn) {
	DoT<ActionAttrsProcess>(fn);
}

/*void ActionAttrsPage::UpdateFromCache() {
	TextLib::TaskManager& tm = GetTaskManager();
	tm.DoActionlistCache();
}

void ActionAttrsPage::DoActionlist(int fn) {
	TextLib::TaskManager& tm = GetTaskManager();
	tm.DoActionlist(fn);
}

void ActionAttrsPage::DoActionlistUsingExisting(int fn) {
	TextLib::TaskManager& tm = GetTaskManager();
	tm.DoActionlistUsingExisting(fn);
}*/


END_TEXTLIB_NAMESPACE

