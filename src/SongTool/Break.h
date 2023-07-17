#ifndef _SongTool_Break_h_
#define _SongTool_Break_h_

struct Break : PatternSnap {
	
	void Jsonize(JsonIO& json) {
		
		PatternSnap::Jsonize(json);
	}
	void FixPtrs() {
		this->brk = this;
	}
	static Array<Break>& GetSub() {static Array<Break> b; return b;} // for PatternSnap::MergeOwner compat
	
	
	PATTERNMASK_MACROS
};

#endif
