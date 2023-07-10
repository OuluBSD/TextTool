#ifndef _SongTool_Part_h_
#define _SongTool_Part_h_

struct Part {
	String name;
	
	Vector<String> lines;
	PatternSnap snap;
	
	// Components
	PatternMask		mask;
	PatternScore	patternscore;
	Composition		composition;
	Analysis		analysis;
	AttrScoreGroup	scoregroup;
	
	void Clear() {
		lines.Clear();
		snap.Clear();
	}
	void Jsonize(JsonIO& json) {
		json
			("lines", lines)
			("snap", snap)
			
			("mask", mask)
			("patternscore", patternscore)
			("composition", composition)
			("analysis", analysis)
			("scoregroup", scoregroup)
			;
	}
	String ToString() const {return "line_len=" + IntStr(lines.GetCount());}
	void FixPtrs() {
		snap.part = this;
		snap.FixPtrs();
	}
};

#endif
