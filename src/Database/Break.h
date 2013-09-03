#ifndef _SongTool_Break_h_
#define _SongTool_Break_h_


struct Break :
	SnapContext
{
	
	
	void Serialize(Stream& s) {
		SnapContext::Serialize(s);
	}
	void FixPtrs() {
		SetBreakPtr(this);
	}
	static Array<Break>& GetSub() {static Array<Break> b; return b;} // for SnapContext::MergeOwner compat
	
	int GetLength(const SnapArg& a) const;
	
	PATTERNMASK_MACROS
};


#endif
