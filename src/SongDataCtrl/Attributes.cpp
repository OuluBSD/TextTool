#include "SongDataCtrl.h"



Attributes::Attributes() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << datasets << groups << values << vsplit;
	hsplit.SetPos(1000,0);
	
	vsplit.Vert() << pos_values << neg_values;
	
	groups.AddColumn(t_("Group"));
	groups.AddColumn(t_("Value count"));
	groups.AddIndex("IDX");
	groups.ColumnWidths("3 1");
	groups.WhenCursor << THISBACK(DataGroup);
	
	values.AddColumn(t_("Value"));
	values.AddColumn(t_("Polarity"));
	values.ColumnWidths("3 1");
	
	pos_values.AddColumn(t_("Value"));
	pos_values.AddColumn(t_("Most popular"));
	
	neg_values.AddColumn(t_("Value"));
	neg_values.AddColumn(t_("Most popular"));
	
	
}

void Attributes::Data() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	
	
	for(int i = 0; i < sda.datasets.GetCount(); i++) {
		datasets.Set(i, 0, sda.datasets.GetKey(i));
	}
	datasets.SetCount(sda.datasets.GetCount());
	if (!datasets.IsCursor() || datasets.GetCount())
		datasets.SetCursor(0);
	
	DataDataset();
}

void Attributes::DataDataset() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	int ds_i = datasets.GetCursor();
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	if (uniq_attrs.IsEmpty()) {
		for(int i = 0; i < da.attrs.GetCount(); i++) {
			const AttrHeader& ah = da.attrs.GetKey(i);
			uniq_attrs.GetAdd(ah.group).FindAdd(ah.value);
		}
		
		SortByKey(uniq_attrs, StdLess<String>());
		for(int i = 0; i < uniq_attrs.GetCount(); i++) {
			SortIndex(uniq_attrs[i], StdLess<String>());
		}
	}
	
	
	for(int i = 0; i < uniq_attrs.GetCount(); i++) {
		groups.Set(i, 0, uniq_attrs.GetKey(i));
		groups.Set(i, 1, uniq_attrs[i].GetCount());
		groups.Set(i, "IDX", i);
	}
	INHIBIT_CURSOR(groups);
	groups.SetCount(uniq_attrs.GetCount());
	groups.SetSortColumn(1, true);
	if (!groups.IsCursor() && groups.GetCount())
		groups.SetCursor(0);
	
	DataGroup();
}

void Attributes::DataGroup() {
	Database& db = Database::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	int ds_i = datasets.GetCursor();
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	if (!groups.IsCursor()) {
		pos_values.Clear();
		neg_values.Clear();
		return;
	}
	
	int g_i = groups.Get("IDX");
	const Index<String>& gvalues = uniq_attrs[g_i];
	
	int attr_i0 = -1;
	int attr_i1 = -1;
	if (g_i < da.attr_group_main.GetCount()) {
		const auto& t = da.attr_group_main[g_i];
		attr_i0 = t.a;
		attr_i1 = t.b;
	}
	for(int i = 0; i < gvalues.GetCount(); i++) {
		int attr_i = gvalues[i];
		values.Set(i, 0, attr_i);
		if (attr_i == attr_i0)
			values.Set(i, 1, "+");
		else if (attr_i == attr_i1)
			values.Set(i, 1, "-");
		else
			values.Set(i, 1, Value());
	}
	INHIBIT_CURSOR(values);
	values.SetCount(gvalues.GetCount());
	if (!values.IsCursor() && values.GetCount())
		values.SetCursor(0);
	
}

void Attributes::ToolMenu(Bar& bar) {
	bar.Add(t_("Update"), AppImg::BlueRing(), THISBACK(Data)).Key(K_CTRL_Q);
	bar.Separator();
	bar.Add(t_("Get main groups"), AppImg::RedRing(), THISBACK1(DoAttributes, 0)).Key(K_F5);
	bar.Add(t_("Simplify attrs"), AppImg::RedRing(), THISBACK1(DoAttributes, 1)).Key(K_F6);
	bar.Add(t_("Join orphaned groups"), AppImg::RedRing(), THISBACK1(DoAttributes, 2)).Key(K_F6);
}

void Attributes::DoAttributes(int fn) {
	SongLib::TaskManager& tm = SongLib::TaskManager::Single();
	tm.DoAttributes(0, fn);
}
