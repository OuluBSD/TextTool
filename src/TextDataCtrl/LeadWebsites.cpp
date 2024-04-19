#include "TextDataCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


LeadWebsites::LeadWebsites() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << vsplit << mainsplit;
	hsplit.SetPos(2000);
	
	vsplit.Vert() << websites << payouts << prices;
	
	mainsplit.Vert() << list << attrs;
	
	
	websites.AddColumn(t_("Website"));
	websites.AddColumn(t_("Count"));
	websites.AddIndex("IDX");
	websites.ColumnWidths("3 1");
	websites.WhenCursor << THISBACK(DataWebsite);
	
	payouts.AddColumn(t_("Payout"));
	payouts.AddColumn(t_("Count"));
	payouts.AddIndex("IDX");
	payouts.AddIndex("MIN");
	payouts.AddIndex("MAX");
	payouts.ColumnWidths("3 1");
	payouts.WhenCursor << THISBACK(DataPayout);
	
	prices.AddColumn(t_("Submission price"));
	prices.AddColumn(t_("Count"));
	prices.AddIndex("IDX");
	prices.AddIndex("MIN");
	prices.AddIndex("MAX");
	prices.ColumnWidths("3 1");
	prices.WhenCursor << THISBACK(DataPrice);
	
	
	list.AddColumn(t_("Website"));
	list.AddColumn(t_("Name"));
	list.AddColumn(t_("Price"));
	list.AddColumn(t_("Payout"));
	list.AddColumn(t_("Description"));
	list.AddColumn(t_("Score"));
	list.AddIndex("IDX");
	list.ColumnWidths("2 4 1 1 10 1");
	list.WhenCursor << THISBACK(DataOpportunity);
	
	attrs.AddColumn(t_("Key"));
	attrs.AddColumn(t_("Value"));
	attrs.ColumnWidths("1 8");
	
}

void LeadWebsites::Data() {
	MetaDatabase& db = MetaDatabase::Single();
	LeadData& ld = db.lead_data;
	LeadDataAnalysis& sda = db.lead_data.a;
	
	
	int counts[LEADSITE_COUNT];
	for(int i = 0; i < LEADSITE_COUNT; i++) counts[i] = 0;
	for (LeadOpportunity& o : ld.opportunities) {
		counts[o.leadsite]++;
	}
	int total = 0;
	for(int i = 0; i < LEADSITE_COUNT; i++) total += counts[i];
	
	
	websites.Set(0, 0, t_("All"));
	websites.Set(0, 1, total);
	websites.Set(0, "IDX", -1);
	
	for(int i = 0; i < LEADSITE_COUNT; i++) {
		websites.Set(1+i, 0, GetLeadWebsiteKey(i));
		websites.Set(1+i, 1, counts[i]);
		websites.Set(1+i, "IDX", i);
	}
	INHIBIT_CURSOR(websites);
	websites.SetCount(1+LEADSITE_COUNT);
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
	
	int leadsite_i = websites.Get("IDX");
	bool filter_leadsite = leadsite_i >= 0;
	
	VectorMap<Tuple2<double,double>,int> counts;
	counts.Add(Tuple2<double,double>(0,1), 0);
	counts.Add(Tuple2<double,double>(1,10), 0);
	counts.Add(Tuple2<double,double>(10,100), 0);
	counts.Add(Tuple2<double,double>(100,1000), 0);
	counts.Add(Tuple2<double,double>(10000,10000), 0);
	counts.Add(Tuple2<double,double>(10000,100000000), 0);
	
	
	for(int i = 0; i < LEADSITE_COUNT; i++) counts[i] = 0;
	for (LeadOpportunity& o : ld.opportunities) {
		if (filter_leadsite && o.leadsite != leadsite_i)
			continue;
		int range_i = 0;
		for(int j = 0; j < counts.GetCount(); j++) {
			const auto& range = counts.GetKey(j);
			if (o.min_compensation >= range.a && o.min_compensation < range.b) {
				range_i = j;
				break;
			}
		}
		counts[range_i]++;
	}
	int total = 0;
	for(int i = 0; i < LEADSITE_COUNT; i++) total += counts[i];
	
	
	
	payouts.Set(0,0,t_("All"));
	payouts.Set(0,1,total);
	payouts.Set(0,"IDX",-1);
	payouts.Set(0,"MIN",0);
	payouts.Set(0,"MAX",INT_MAX);
	for(int i = 0; i < counts.GetCount(); i++) {
		const auto& range = counts.GetKey(i);
		String str = IntStr(range.a) + " - " + IntStr(range.b);
		payouts.Set(1+i, 0, str);
		payouts.Set(1+i, 1, counts[i]);
		payouts.Set(1+i, "IDX", i);
		payouts.Set(1+i, "MIN", range.a);
		payouts.Set(1+i, "MAX", range.b);
	}
	
	INHIBIT_CURSOR(payouts);
	payouts.SetCount(1+counts.GetCount());
	//payouts.SetSortColumn(1, true);
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
	
	int leadsite_i = websites.Get("IDX");
	bool filter_leadsite = leadsite_i >= 0;
	int payout_min = payouts.Get("MIN");
	int payout_max = payouts.Get("MAX");
	
	VectorMap<Tuple2<double,double>,int> counts;
	counts.Add(Tuple2<double,double>(0,1), 0);
	counts.Add(Tuple2<double,double>(1,10), 0);
	counts.Add(Tuple2<double,double>(10,100), 0);
	counts.Add(Tuple2<double,double>(100,1000), 0);
	counts.Add(Tuple2<double,double>(10000,10000), 0);
	counts.Add(Tuple2<double,double>(10000,100000000), 0);
	
	
	for(int i = 0; i < LEADSITE_COUNT; i++) counts[i] = 0;
	for (LeadOpportunity& o : ld.opportunities) {
		if (filter_leadsite && o.leadsite != leadsite_i)
			continue;
		if (o.min_compensation < payout_min || o.min_compensation >= payout_max)
			continue;
		double price = 0.01 * o.min_entry_price_cents;
		int range_i = 0;
		for(int j = 0; j < counts.GetCount(); j++) {
			const auto& range = counts.GetKey(j);
			if (price >= range.a && price < range.b) {
				range_i = j;
				break;
			}
		}
		counts[range_i]++;
	}
	int total = 0;
	for(int i = 0; i < LEADSITE_COUNT; i++) total += counts[i];
	
	
	
	prices.Set(0,0,t_("All"));
	prices.Set(0,1,total);
	prices.Set(0,"IDX",-1);
	prices.Set(0,"MIN",0);
	prices.Set(0,"MAX",INT_MAX);
	for(int i = 0; i < counts.GetCount(); i++) {
		const auto& range = counts.GetKey(i);
		String str = IntStr(range.a) + " - " + IntStr(range.b);
		prices.Set(1+i, 0, str);
		prices.Set(1+i, 1, counts[i]);
		prices.Set(1+i, "IDX", i);
		prices.Set(1+i, "MIN", range.a);
		prices.Set(1+i, "MAX", range.b);
	}
	
	
	INHIBIT_CURSOR(prices);
	prices.SetCount(1+counts.GetCount());
	//prices.SetSortColumn(1, true);
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
	
	int leadsite_i = websites.Get("IDX");
	bool filter_leadsite = leadsite_i >= 0;
	int payout_min = payouts.Get("MIN");
	int payout_max = payouts.Get("MAX");
	int price_min = prices.Get("MIN");
	int price_max = prices.Get("MAX");
	
	
	int row = 0;
	int i = -1;
	for (LeadOpportunity& o : ld.opportunities) {
		i++;
		if (filter_leadsite && o.leadsite != leadsite_i)
			continue;
		if (o.min_compensation < payout_min || o.min_compensation >= payout_max)
			continue;
		double price = 0.01 * o.min_entry_price_cents;
		if (price < price_min || price >= price_max)
			continue;
		
		double score = 0;
		if (price > 0 && o.min_compensation) {
			score = o.min_compensation / price * 1000;
		}
		else if (o.min_compensation > 0) {
			score = o.min_compensation;
		}
			
		
		list.Set(row, 0, GetLeadWebsiteKey(o.leadsite));
		list.Set(row, 1, o.name);
		list.Set(row, 2, price);
		list.Set(row, 3, o.min_compensation);
		if (o.request_opportunity_description.GetCount()) {
			String s;
			s	<< o.request_opportunity_description << "\n"
				<< o.request_band_description << "\n"
				<< o.request_selection_description;
			list.Set(row, 4, s);
		}
		else {
			list.Set(row, 4, o.request_description);
		}
		list.Set(row, 5, score);
		list.Set(row, "IDX", i);
		
		row++;
	}
	
	INHIBIT_CURSOR(list);
	list.SetCount(row);
	list.SetSortColumn(5, true);
	if (!list.IsCursor() && list.GetCount())
		list.SetCursor(0);
	
	DataOpportunity();
}

void LeadWebsites::DataOpportunity() {
	MetaDatabase& db = MetaDatabase::Single();
	LeadData& ld = db.lead_data;
	LeadDataAnalysis& sda = db.lead_data.a;
	
	if (!list.IsCursor())
		return;
	
	int idx = list.Get("IDX");
	LeadOpportunity& o = ld.opportunities[idx];
	
	for(int i = 0; i < o.GetCount(); i++) {
		const char* key = o.GetKey(i);
		Value val = o[i];
		attrs.Set(i, 0, key);
		attrs.Set(i, 1, val);
	}
	
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
