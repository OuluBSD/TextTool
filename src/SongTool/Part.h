#ifndef _SongTool_Part_h_
#define _SongTool_Part_h_

struct Part {
	String name;
	
	Vector<String>		lines;
	PatternSnap			snap;
	PartScore			score;
	PatternMask			mask;
	Composition			composition;
	Analysis			analysis;
	
	void Clear() {
		lines.Clear();
		snap.Clear();
	}
	void Jsonize(JsonIO& json) {
		json
			("lines", lines)
			("snap", snap)
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
	}
};

#endif
