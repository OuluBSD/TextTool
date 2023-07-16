#ifndef _SongTool_Break_h_
#define _SongTool_Break_h_

struct Break : PatternSnap {
	String						txt;
	
	
	void Jsonize(JsonIO& json) {
		json
			("txt", txt)
			;
		PatternSnap::Jsonize(json);
	}
	void FixPtrs() {
		this->brk = this;
	}
	static Array<Break>& GetSub() {static Array<Break> b; return b;} // for PatternSnap::MergeOwner compat
	
	
	PATTERNMASK_MACROS
};

#endif
