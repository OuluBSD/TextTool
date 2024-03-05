#include "SongDataCtrl.h"


BEGIN_SONGLIB_NAMESPACE


Attributes::Attributes() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << datasets << groups << values << vsplit;
	hsplit.SetPos(1000,0);
	
	vsplit.Vert() << pos_values << neg_values;
	
	datasets.AddColumn(t_("Dataset"));
	datasets.WhenCursor << THISBACK(DataDataset);
	
	groups.AddColumn(t_("Group"));
	groups.AddColumn(t_("Value count"));
	groups.AddColumn(t_("Link"));
	groups.AddIndex("IDX");
	groups.ColumnWidths("3 1 1");
	groups.WhenCursor << THISBACK(DataGroup);
	
	values.AddColumn(t_("Value"));
	
	pos_values.AddColumn(t_("Value"));
	pos_values.AddColumn(t_("Most popular"));
	
	neg_values.AddColumn(t_("Value"));
	neg_values.AddColumn(t_("Most popular"));
	
	
}

void Attributes::Data() {
	SongDatabase& db = SongDatabase::Single();
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
	SongDatabase& db = SongDatabase::Single();
	SongData& sd = db.song_data;
	SongDataAnalysis& sda = db.song_data.a;
	int ds_i = datasets.GetCursor();
	DatasetAnalysis& da = sda.datasets[ds_i];
	
	/*if (uniq_attrs.IsEmpty())*/ {
		for(int i = 0; i < da.attrs.GetCount(); i++) {
			const AttrHeader& ah = da.attrs.GetKey(i);
			uniq_attrs.GetAdd(ah.group).FindAdd(ah.value);
			uniq_attrs_i.GetAdd(ah.group).FindAdd(i);
		}
		
		SortByKey(uniq_attrs, StdLess<String>());
		for(int i = 0; i < uniq_attrs.GetCount(); i++) {
			SortIndex(uniq_attrs[i], StdLess<String>());
		}
	}
	
	
	for(int i = 0; i < uniq_attrs.GetCount(); i++) {
		String group = uniq_attrs.GetKey(i);
		const auto& v = uniq_attrs[i];
		groups.Set(i, 0, group);
		groups.Set(i, 1, v.GetCount());
		groups.Set(i, "IDX", i);
		
		if (v.GetCount() == 1) {
			AttrHeader ah;
			ah.group = group;
			ah.value = v[0];
			int j = da.attrs.Find(ah);
			if (j >= 0) {
				const ExportAttr& ea = da.attrs[j];
				if (ea.link >= 0) {
					const AttrHeader& link_ah = da.attrs.GetKey(ea.link);
					groups.Set(i, 2, link_ah.group + ": " + link_ah.value);
				}
			}
			else
				groups.Set(i, 2, Value());
		}
		else {
			groups.Set(i, 2, Value());
		}
	}
	INHIBIT_CURSOR(groups);
	groups.SetCount(uniq_attrs.GetCount());
	groups.SetSortColumn(1, true);
	if (!groups.IsCursor() && groups.GetCount())
		groups.SetCursor(0);
	
	DataGroup();
}

void Attributes::DataGroup() {
	SongDatabase& db = SongDatabase::Single();
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
	const String& group = uniq_attrs.GetKey(g_i);
	const Index<String>& gvalues = uniq_attrs[g_i];
	
	int attr_i0 = -1;
	int attr_i1 = -1;
	String attr_s0, attr_s1;
	int i = da.simple_attrs.Find(group);
	if (i >= 0) {
		const ExportSimpleAttr& t = da.simple_attrs[i];
		attr_i0 = t.attr_i0;
		attr_i1 = t.attr_i1;
		attr_s0 = da.attrs.GetKey(attr_i0).value;
		attr_s1 = da.attrs.GetKey(attr_i1).value;
	}
	for(int i = 0; i < gvalues.GetCount(); i++) {
		String attr_s = gvalues[i];
		if (attr_s == attr_s0)
			SetColoredListValue(values, i, 0, attr_s, Green());
		else if (attr_s == attr_s1)
			SetColoredListValue(values, i, 0, attr_s, Red());
		else
			values.Set(i, 0, attr_s);
	}
	INHIBIT_CURSOR(values);
	values.SetCount(gvalues.GetCount());
	if (!values.IsCursor() && values.GetCount())
		values.SetCursor(0);
	
	i = uniq_attrs_i.Find(group);
	if (i < 0) {
		pos_values.SetCount(0);
		neg_values.SetCount(0);
	}
	else {
		const auto& indices = uniq_attrs_i[i];
		int prow = 0, nrow = 0;
		for(int i = 0; i < indices.GetCount(); i++) {
			const AttrHeader& ah = da.attrs.GetKey(indices[i]);
			const ExportAttr& ea = da.attrs[indices[i]];
			if (ea.positive) {
				pos_values.Set(prow, 0, ah.value);
				prow++;
			}
			else {
				neg_values.Set(nrow, 0, ah.value);
				nrow++;
			}
			pos_values.SetCount(prow);
			neg_values.SetCount(nrow);
		}
	}
	
}

void Attributes::ToolMenu(Bar& bar) {
	bar.Add(t_("Update"), AppImg::BlueRing(), THISBACK(Data)).Key(K_CTRL_Q);
	bar.Separator();
	bar.Add(t_("Get main groups"), AppImg::RedRing(), THISBACK1(DoAttributes, 0)).Key(K_F5);
	bar.Add(t_("Simplify attrs"), AppImg::RedRing(), THISBACK1(DoAttributes, 1)).Key(K_F6);
	bar.Add(t_("Join orphaned groups"), AppImg::RedRing(), THISBACK1(DoAttributes, 2)).Key(K_F7);
	bar.Add(t_("Fix data"), AppImg::RedRing(), THISBACK1(DoAttributes, 3)).Key(K_F8);
}

void Attributes::DoAttributes(int fn) {
	SongLib::TaskManager& tm = SongLib::TaskManager::Single();
	tm.DoAttributes(0, fn);
}


END_SONGLIB_NAMESPACE
