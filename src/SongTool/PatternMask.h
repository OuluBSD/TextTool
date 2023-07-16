#ifndef _SongTool_PatternMask_h_
#define _SongTool_PatternMask_h_

struct PatternMask : Ptrs {
	Index<SnapAttrStr> mask;
	
	
	void Clear() {mask.Clear();}
	void Add(const SnapAttrStr& sa) {
		mask.FindAdd(sa);
	}
	void Jsonize(JsonIO& json) {
		json
			("mask", mask)
			;
	}
	void GetGroups(Index<int>& groups) {
		for (const SnapAttrStr& sa : mask.GetKeys()) {
			sa.RealizeId();
			groups.FindAdd(sa.group_i);
		}
	}
	void GetGroupItems(VectorMap<int, Vector<int>>& group_items) {
		for (const SnapAttrStr& sa : mask.GetKeys()) {
			sa.RealizeId();
			group_items.GetAdd(sa.group_i).Add(sa.item_i);
		}
	}
	void ResolveId() {
		for (const SnapAttrStr& sa : mask.GetKeys())
			sa.RealizeId();
	}
};

#endif
