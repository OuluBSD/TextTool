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
};

#endif
