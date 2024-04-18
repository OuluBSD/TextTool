#include "TextDatabase.h"


BEGIN_TEXTLIB_NAMESPACE


LeadData::LeadData() {
	
}

void LeadData::Load() {
	String& dir = MetaDatabase::Single().dir;
	LoadFromJsonFileStandard(*this, dir + DIR_SEPS "share" DIR_SEPS + "db.json");
}

void LeadData::Store() {
	String& dir = MetaDatabase::Single().dir;
	StoreAsJsonFileStandard(*this, dir + DIR_SEPS "share" DIR_SEPS + "db.json", true);
}

void LeadData::Jsonize(JsonIO& json) {
	json("opportunities", opportunities);
}

LeadOpportunity& LeadData::GetAddOpportunity(int leadsite, String id) {
	for (LeadOpportunity& o : opportunities) {
		if (o.leadsite == leadsite && o.id == id) {
			o.last_seen = GetSysTime();
			return o;
		}
	}
	LeadOpportunity& o = opportunities.Add();
	o.first_seen = GetSysTime();
	o.leadsite = leadsite;
	o.id = id;
	return o;
}



void LeadOpportunity::Jsonize(JsonIO& json) {
	#define ITEM(x) (#x, x)
	json
		ITEM(first_seen)
		ITEM(last_seen)
		ITEM(leadsite)
		ITEM(id)
		ITEM(name)
		ITEM(links)
		ITEM(genres)
		ITEM(promoter_group_genres)
		ITEM(band_opportunity_type)
		ITEM(obj_class)
		ITEM(request_entry_fee)
		ITEM(request_featured )
		ITEM(request_exclusive )
		ITEM(request_curated )
		ITEM(request_contest )
		ITEM(request_comments)
		ITEM(request_first_name)
		ITEM(request_last_name)
		ITEM(request_email)
		ITEM(request_phone)
		ITEM(request_description)
		ITEM(request_opportunity_description)
		ITEM(request_band_description)
		ITEM(request_selection_description)
		ITEM(vanity_url_id )
		ITEM(vanity_url_name)
		ITEM(status_text)
		ITEM(description)
		ITEM(band_opportunity_type_text)
		ITEM(local_event_end_datetime)
		ITEM(is_accepting_entries)
		ITEM(deleted )
		ITEM(address_str)
		ITEM(public_image_url)
		ITEM(logo_image_url)
		ITEM(promoter_group_name)
		ITEM(promoter_group_main_image_url)
		ITEM(promoter_group_facebook_url)
		ITEM(promoter_group_twitter_url)
		ITEM(promoter_group_youtube_url)
		ITEM(promoter_group_instagram_url)
		ITEM(promoter_group_talent_description)
		ITEM(promoter_group_short_description)
		ITEM(promoter_group_talent_roster)
		ITEM(promoter_group_opportunity_frequency_count )
		ITEM(promoter_group_opportunity_frequency)
		ITEM(compensated )
		ITEM(min_compensation )
		ITEM(max_compensation )
		ITEM(pay_to_apply )
		ITEM(free_to_apply )
		ITEM(entry_count )
		ITEM(entry_end_datetime)
		ITEM(date_created)
	;
	#undef ITEM
}

void LeadOpportunity::Genre::Jsonize(JsonIO& json) {
	json
		("id", id)
		("name", name)
		("primary", primary)
		;
}



LeadDataAnalysis::LeadDataAnalysis() {
	
}


END_TEXTLIB_NAMESPACE

