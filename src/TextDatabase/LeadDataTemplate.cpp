#include "TextDatabase.h"


BEGIN_TEXTLIB_NAMESPACE


LeadDataTemplate::LeadDataTemplate() {
	
}

void LeadDataTemplate::Load() {
	String& dir = MetaDatabase::Single().dir;
	ASSERT(dir.GetCount());
	LoadFromJsonFileStandard(*this, dir + DIR_SEPS + "share-common" + DIR_SEPS + "template_lead_data.json");
}

void LeadDataTemplate::Store() {
	String& dir = MetaDatabase::Single().dir;
	StoreAsJsonFileStandard(*this, dir + DIR_SEPS + "share-common" + DIR_SEPS + "template_lead_data.json", true);
}

void LeadDataTemplate::Jsonize(JsonIO& json) {
	json		("templates", templates)
				("author_classes", author_classes)
				("author_specialities", author_specialities)
				("profit_reasons", profit_reasons)
				("organizational_reasons", organizational_reasons)
				("publishers", publishers)
				;
}

/*LeadOpportunity& LeadDataTemplate::GetAddOpportunity(int leadsite, String id) {
	for (LeadOpportunity& o : opportunities) {
		if (o.leadsite == leadsite && o.id == id) {
			Time now = GetSysTime();
			if (o.last_seen < now-12*60*60)
				o.last_seen = GetSysTime();
			return o;
		}
	}
	LeadOpportunity& o = opportunities.Add();
	o.first_seen = GetSysTime();
	o.last_seen = o.first_seen;
	o.leadsite = leadsite;
	o.id = id;
	return o;
}*/


END_TEXTLIB_NAMESPACE

