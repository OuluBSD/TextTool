#ifndef _SongTool_Break_h_
#define _SongTool_Break_h_

struct Break :
	SnapContext
{
	
	void Jsonize(JsonIO& json) {
		
		SnapContext::Jsonize(json);
	}
	void FixPtrs() {
		SetBreakPtr(this);
	}
	static Array<Break>& GetSub() {static Array<Break> b; return b;} // for SnapContext::MergeOwner compat
	
	
	PATTERNMASK_MACROS
};

#endif
