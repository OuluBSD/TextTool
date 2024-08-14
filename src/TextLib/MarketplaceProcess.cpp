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
		case PHASE_CATEGORY:	return owner->marketplace.items.GetCount();
		case PHASE_SUBCATEGORY:	return owner->marketplace.items.GetCount();
		case PHASE_DESCRIPTION:	return owner->marketplace.items.GetCount();
		case PHASE_TITLE:		return owner->marketplace.items.GetCount();
		default: return 1;
	}
}

int MarketplaceProcess::GetSubBatchCount(int phase, int batch) const {
	switch (phase) {
		case PHASE_CATEGORY:	return 1;
		case PHASE_SUBCATEGORY:	return 1;
		case PHASE_DESCRIPTION:	return 1;
		case PHASE_TITLE:		return 1;
		default: return 1;
	}
}

void MarketplaceProcess::DoPhase() {
	switch (phase) {
		case PHASE_CATEGORY:	ProcessCategory(); return;
		case PHASE_SUBCATEGORY:	ProcessSubCategory(); return;
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
	args.map.Add("category", it.category);
	args.map.Add("subcategory", it.subcategory);
	args.map.Add("description", it.description);
}

void MarketplaceProcess::ProcessCategory() {
	if (batch >= owner->marketplace.items.GetCount()) {
		NextPhase();
		return;
	}
	
	MarketplaceArgs args;
	args.fn = 0;
	MakeArgs(args);
	
	SetWaiting(1);
	TaskMgr& m = TaskMgr::Single();
	m.GetMarketplace(args, [this](String res) {
		res = TrimBoth(res);
		
		if (res.GetCount() && IsDigit(res[0])) {
			int cat_i = ScanInt(res);
		}
		else {
			
		}
		
		SetWaiting(0);
		NextBatch();
	});
}

void MarketplaceProcess::ProcessSubCategory() {
	NextPhase();
}

void MarketplaceProcess::ProcessDescription() {
	
}

void MarketplaceProcess::ProcessTitle() {
	
}


END_TEXTLIB_NAMESPACE

