#ifndef _SongTool_PatternMask_h_
#define _SongTool_PatternMask_h_

struct PatternMask {
	Index<SnapAttrStr> attrs;
	
	
	void Clear() {attrs.Clear();}
	void Add(const SnapAttrStr& sa) {
		attrs.FindAdd(sa);
	}
	void Jsonize(JsonIO& json) {
		json
			("attrs", attrs)
			;
	}
	void GetGroups(Index<int>& groups) {
		for (const SnapAttrStr& sa : attrs.GetKeys()) {
			sa.RealizeId();
			groups.FindAdd(sa.group_i);
		}
	}
	void GetGroupItems(VectorMap<int, Vector<int>>& group_items) {
		for (const SnapAttrStr& sa : attrs.GetKeys()) {
			sa.RealizeId();
			group_items.GetAdd(sa.group_i).Add(sa.item_i);
		}
	}
	void ResolveId() {
		for (const SnapAttrStr& sa : attrs.GetKeys())
			sa.RealizeId();
	}
};

#endif
