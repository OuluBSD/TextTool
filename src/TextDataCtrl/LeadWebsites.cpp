#include "TextDataCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


LeadWebsites::LeadWebsites() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << vsplit << list;
	hsplit.SetPos(2000);
	
	vsplit.Vert() << websites << payouts << prices;
	
	websites.AddColumn(t_("Website"));
	websites.AddColumn(t_("Count"));
	websites.AddIndex("IDX");
	websites.ColumnWidths("3 1");
	websites.WhenCursor << THISBACK(DataWebsite);
	
	payouts.AddColumn(t_("Payout"));
	payouts.AddColumn(t_("Count"));
	payouts.AddIndex("IDX");
	payouts.ColumnWidths("3 1");
	payouts.WhenCursor << THISBACK(DataPayout);
	
	prices.AddColumn(t_("Submission price"));
	prices.AddColumn(t_("Count"));
	prices.AddIndex("IDX");
	prices.ColumnWidths("3 1");
	prices.WhenCursor << THISBACK(DataPrice);
	
	
}

void LeadWebsites::Data() {
	MetaDatabase& db = MetaDatabase::Single();
	LeadData& ld = db.lead_data;
	LeadDataAnalysis& sda = db.lead_data.a;
	
	
	for(int i = 0; i < LEADSITE_COUNT; i++) {
		websites.Set(i, 0, GetLeadWebsiteKey(i));
		websites.Set(i, 1, 0);
		websites.Set(i, "IDX", i);
		
		/*if (v.GetCount() == 1) {
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
		}*/
	}
	INHIBIT_CURSOR(websites);
	websites.SetCount(LEADSITE_COUNT);
	websites.SetSortColumn(1, true);
	if (!websites.IsCursor() && websites.GetCount())
		websites.SetCursor(0);
	
	DataWebsite();
}

void LeadWebsites::DataWebsite() {
	MetaDatabase& db = MetaDatabase::Single();
	LeadData& ld = db.lead_data;
	LeadDataAnalysis& sda = db.lead_data.a;
	
	if (!websites.IsCursor())
		return;
	
	payouts.Set(0,0,t_("All"));
	payouts.Set(0,"IDX",-1);
	
	INHIBIT_CURSOR(payouts);
	payouts.SetCount(1);
	payouts.SetSortColumn(1, true);
	if (!payouts.IsCursor() && payouts.GetCount())
		payouts.SetCursor(0);
	
	DataPayout();
}

void LeadWebsites::DataPayout() {
	MetaDatabase& db = MetaDatabase::Single();
	LeadData& ld = db.lead_data;
	LeadDataAnalysis& sda = db.lead_data.a;
	
	if (!websites.IsCursor() || !payouts.IsCursor())
		return;
	
	
	prices.Set(0,0,t_("All"));
	prices.Set(0,"IDX",-1);
	
	INHIBIT_CURSOR(prices);
	prices.SetCount(1);
	prices.SetSortColumn(1, true);
	if (!prices.IsCursor() && prices.GetCount())
		prices.SetCursor(0);
	
	
	DataPrice();
}

void LeadWebsites::DataPrice() {
	MetaDatabase& db = MetaDatabase::Single();
	LeadData& ld = db.lead_data;
	LeadDataAnalysis& sda = db.lead_data.a;
	
	if (!websites.IsCursor() || !payouts.IsCursor() || !prices.IsCursor())
		return;
	
	
	
	
	
	
}

void LeadWebsites::ToolMenu(Bar& bar) {
	bar.Add(t_("Refresh"), AppImg::BlueRing(), THISBACK(Data)).Key(K_CTRL_Q);
	bar.Separator();
	bar.Add(t_("Update website leads"), AppImg::RedRing(), THISBACK1(Do, 0)).Key(K_F5);
	
}

void LeadWebsites::Do(int fn) {
	LeadSolver& tm = LeadSolver::Get(MetaEntity::DatabaseUpdate());
	tm.Start();
}


END_TEXTLIB_NAMESPACE
