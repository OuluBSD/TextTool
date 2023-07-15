#ifndef _SongTool_Part_h_
#define _SongTool_Part_h_

struct Part {
	String name;
	
	VectorMap<String,String> data;
	Vector<String>		lines;
	PatternSnap			snap, rev_snap;
	PartScore			score;
	PatternMask			mask;
	Composition			composition;
	Analysis			analysis;
	
	void Clear() {
		lines.Clear();
		snap.Clear();
		rev_snap.Clear();
	}
	void Jsonize(JsonIO& json) {
		json
			("data", data)
			("lines", lines)
			("snap", snap)
			("rev_snap", rev_snap)
			("score", score)
			("mask", mask)
			("composition", composition)
			("analysis", analysis)
			;
	}
	String ToString() const {return "line_len=" + IntStr(lines.GetCount());}
	void FixPtrs() {
		snap.part = this;
		snap.FixPtrs();
		rev_snap.part = this;
		rev_snap.FixPtrs();
	}
};

#endif
