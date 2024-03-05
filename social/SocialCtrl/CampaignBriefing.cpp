#include "SocialCtrl.h"


BEGIN_SOCIALLIB_NAMESPACE


CampaignBriefing::CampaignBriefing() {
	Add(vsplit.SizePos());
	
	vsplit.Vert() << list << values;
	
	CtrlLayout(values);
	
	list.AddIndex();
	list.AddColumn("Key");
	list.AddColumn("Value");
	list.ColumnWidths("1 4");
	list.WhenCursor << THISBACK(OnListCursor);
	
	values.value.WhenAction << THISBACK(OnValueChange);
	
}

void CampaignBriefing::Data() {
	SocialDatabase& db = SocialDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	if (!p.campaign) return;
	Campaign& campaign = *p.campaign;
	for(int i = 0; i < ITEM_COUNT; i++) {
		list.Set(i, 0, i);
		switch(i) {
			#define ITEM(k,s,d) case k: list.Set(i, 1, s); list.Set(i, 2, campaign.data.Get(#k, "")); break;
			ALBUM_BRIEFING_LIST
			#undef ITEM
		
			default: break;
		}
	}
	list.SetCount(ITEM_COUNT);
	
	if (!list.IsCursor() && list.GetCount()) list.SetCursor(0);
	
	OnListCursor();
}

void CampaignBriefing::OnListCursor() {
	values.key.Clear();
	values.description.Clear();
	values.value.Clear();
	
	SocialDatabase& db = SocialDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	if (!p.campaign) return;
	Campaign& campaign = *p.campaign;
	
	String value_str;
	switch (list.GetCursor()) {
		#define ITEM(k,s,d) case k: values.key.SetData(s); values.description.SetData(d); value_str = campaign.data.Get(#k, ""); break;
		ALBUM_BRIEFING_LIST
		#undef ITEM
		
		default: break;
	}
	
	values.value.SetData(value_str);
}

void CampaignBriefing::OnValueChange() {
	SocialDatabase& db = SocialDatabase::Single();
	EditorPtrs& p = EditorPtrs::Single();
	if (!p.campaign) return;
	Campaign& campaign = *p.campaign;
	
	if (!list.IsCursor()) return;
	
	String key_str;
	switch (list.GetCursor()) {
		#define ITEM(k,s,d) case k: key_str = #k; break;
		ALBUM_BRIEFING_LIST
		#undef ITEM
		default: return;
	}
	
	String value_str = values.value.GetData();
	campaign.data.GetAdd(key_str) = value_str;
	
	list.Set(2, value_str);
}


END_SOCIALLIB_NAMESPACE
