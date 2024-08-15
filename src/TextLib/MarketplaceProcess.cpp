#include "TextLib.h"


BEGIN_TEXTLIB_NAMESPACE


MarketplaceProcess::MarketplaceProcess() {
	
}

int MarketplaceProcess::GetPhaseCount() const {
	return PHASE_COUNT;
}

int MarketplaceProcess::GetBatchCount(int phase) const {
	const auto& s = GetMarketplaceSections();
	switch (phase) {
		case PHASE_DESCRIPTION:	return owner->marketplace.items.GetCount();
		case PHASE_TITLE:		return owner->marketplace.items.GetCount();
		default: return 1;
	}
}

int MarketplaceProcess::GetSubBatchCount(int phase, int batch) const {
	switch (phase) {
		case PHASE_DESCRIPTION:	return 1;
		case PHASE_TITLE:		return 1;
		default: return 1;
	}
}

void MarketplaceProcess::DoPhase() {
	switch (phase) {
		case PHASE_DESCRIPTION:	ProcessDescription(); return;
		case PHASE_TITLE:		ProcessTitle(); return;
		default: NextPhase(); break;
	}
}

MarketplaceProcess& MarketplaceProcess::Get(Owner& o) {
	String t = o.name;
	hash_t h = t.GetHashValue();
	static ArrayMap<hash_t, MarketplaceProcess> map;
	int i = map.Find(h);
	if (i >= 0)
		return map[i];
	
	MarketplaceProcess& ls = map.Add(h);
	ls.owner = &o;
	return ls;
}

void MarketplaceProcess::MakeArgs(MarketplaceArgs& args) {
	MarketplaceItem& it = owner->marketplace.items[batch];
	args.map.Add("generic", it.generic);
	args.map.Add("brand", it.brand);
	args.map.Add("model", it.model);
	args.map.Add("price", DblStr(it.price) + "€");
	args.map.Add("faults", it.faults);
	args.map.Add("works", it.works);
	args.map.Add("condition", it.broken ? "broken" : (it.good ? "good" : "fair"));
	args.map.Add("title", it.title);
	args.map.Add("description", it.description);
	args.map.Add("other", it.other);
	if (it.year_of_manufacturing > 0)
		args.map.Add("year of manufacturing", IntStr(it.year_of_manufacturing));
	
	const auto& sects = GetMarketplaceSections();
	args.map.Add("category", sects.GetKey(it.category));
	args.map.Add("subcategory", sects[it.category][it.subcategory]);
}

void MarketplaceProcess::ProcessDescription() {
	if (batch >= owner->marketplace.items.GetCount()) {
		NextPhase();
		return;
	}
	
	MarketplaceArgs args;
	args.fn = 0;
	MakeArgs(args);
	args.map.RemoveKey("title");
	args.map.RemoveKey("description");
	MarketplaceItem& it = owner->marketplace.items[batch];
	int64 hash = args.map.GetHashValue();
	/*if (skip_ready && hash == it.input_hash && it.description.GetCount()) {
		NextBatch();
		return;
	}*/
	
	SetWaiting(1);
	TaskMgr& m = TaskMgr::Single();
	m.GetMarketplace(args, [this,hash](String res) {
		MarketplaceItem& it = owner->marketplace.items[batch];
		res = TrimBoth(res);
		RemoveQuotes(res);
		it.description = res;
		it.input_hash = hash;
		it.title.Clear();
		
		SetWaiting(0);
		NextBatch();
	});
}

void MarketplaceProcess::ProcessTitle() {
	if (batch >= owner->marketplace.items.GetCount()) {
		NextPhase();
		return;
	}
	
	MarketplaceArgs args;
	args.fn = 1;
	MakeArgs(args);
	args.map.RemoveKey("title");
	
	MarketplaceItem& it = owner->marketplace.items[batch];
	int64 hash = args.map.GetHashValue();
	if (it.title.GetCount()) {
		NextBatch();
		return;
	}
	
	SetWaiting(1);
	TaskMgr& m = TaskMgr::Single();
	m.GetMarketplace(args, [this,hash](String res) {
		MarketplaceItem& it = owner->marketplace.items[batch];
		res = TrimBoth(res);
		RemoveQuotes(res);
		it.title = res;
		
		SetWaiting(0);
		NextBatch();
	});
}


END_TEXTLIB_NAMESPACE

