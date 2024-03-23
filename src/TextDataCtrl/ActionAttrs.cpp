#include "TextDataCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


ActionAttrsPage::ActionAttrsPage() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << vsplit << actions;
	hsplit.SetPos(2000);
	
	vsplit.Vert() << datasets << attrs << colors;
	vsplit.SetPos(1000,0);
	
	datasets.AddColumn(t_("Dataset"));
	datasets.WhenCursor << THISBACK(DataDataset);
	
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

void ActionAttrsPage::Data() {
	
}

void ActionAttrsPage::ToolMenu(Bar& bar) {
	bar.Add(t_("Update data"), AppImg::BlueRing(), THISBACK(DataMain)).Key(K_CTRL_Q);
	bar.Separator();
	//bar.Add(t_("Update from cache"), AppImg::RedRing(), THISBACK(UpdateFromCache)).Key(K_F5);
	bar.Add(t_("Update action colors"), AppImg::RedRing(), THISBACK(UpdateColors)).Key(K_F6);
	bar.Add(t_("Update action attributes"), AppImg::RedRing(), THISBACK(UpdateAttributes)).Key(K_F7);
	
}

void ActionAttrsPage::DataMain() {
	TextDatabase& db = GetDatabase();
	TextData& sd = db.src_data;
	TextDataAnalysis& sda = db.src_data.a;
	
	
	for(int i = 0; i < sda.datasets.GetCount(); i++) {
		datasets.Set(i, 0, sda.datasets.GetKey(i));
	}
	datasets.SetCount(sda.datasets.GetCount());
	if (!datasets.IsCursor() || datasets.GetCount())
		datasets.SetCursor(0);
	
	DataDataset();
}

void ActionAttrsPage::DataDataset() {
	if (!datasets.IsCursor())
		return;
	
	
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
	if (!datasets.IsCursor() || !colors.IsCursor() || !attrs.IsCursor())
		return;
	
	TextDatabase& db = GetDatabase();
	TextData& sd = db.src_data;
	TextDataAnalysis& sda = db.src_data.a;
	int ds_i = datasets.GetCursor();
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	int clr_i = colors.GetCursor();
	int attr_group_i = attrs.Get("GROUP");
	int attr_value_i = attrs.Get("VALUE");
	String group_str = attr_group_i >= 0 ? ToLower(Attr::AttrKeys[attr_group_i][1]) : String();
	String value_str = attr_group_i >= 0 ? ToLower(Attr::AttrKeys[attr_group_i][2 + attr_value_i]) : String();
	
	bool clr_filter = clr_i > 0;
	bool attr_filter = attr_group_i >= 0;
	clr_i--;
	
	int row = 0;
	for(int i = 0; i < da.actions.GetCount(); i++) {
		const ActionHeader& ah = da.actions.GetKey(i);
		ExportAction& aa = da.actions[i];
		
		// Filter by color group
		if (clr_filter && GetColorGroup(aa.clr) != clr_i)
			continue;
		
		// Filter by attribute
		if (aa.attr < 0)
			continue;
		const AttrHeader& ath = da.attrs.GetKey(aa.attr);
		if (attr_filter && (ath.group != group_str || ath.value != value_str))
			continue;
		
		actions.Set(row, "IDX", i);
		actions.Set(row, 0, ath.group);
		actions.Set(row, 1, ath.value);
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
	}
	actions.SetCount(row);
	actions.SetSortColumn(2);
	
}

void ActionAttrsPage::UpdateFromCache() {
	TextLib::TaskManager& tm = GetTaskManager();
	tm.DoActionlistCache(0);
}

void ActionAttrsPage::UpdateColors() {
	TextLib::TaskManager& tm = GetTaskManager();
	tm.DoActionlist(0, 0);
}

void ActionAttrsPage::UpdateAttributes() {
	TextLib::TaskManager& tm = GetTaskManager();
	tm.DoActionlist(0, 1);
}


END_TEXTLIB_NAMESPACE

