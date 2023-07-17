#ifndef _SongTool_Part_h_
#define _SongTool_Part_h_

struct Part :
	PatternSnap
{
	String name;
	
	Array<Line>			lines;
	//PartScore			score;
	Composition			composition;
	Analysis			analysis;
	
	void Clear() {
		lines.Clear();
		PatternSnap::Clear();
	}
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("data", data)
			("lines", lines)
			//("snap", snap)
			//("rev_snap", rev_snap)
			//("score", score)
			//("mask", mask)
			("composition", composition)
			("analysis", analysis)
			;
		PatternSnap::Jsonize(json);
	}
	String ToString() const {return name + ", lines=" + IntStr(lines.GetCount());}
	void FixPtrs() {
		this->part = this;
		int id = 0;
		for (Line& l : lines) {
			static_cast<Ptrs&>(l) = *(Ptrs*)this;
			l.owner = this;
			l.SetId(id++);
			l.FixPtrs();
		}
	}
	Array<Line>& GetSub() {return lines;}
	const Array<Line>& GetSub() const {return lines;}
	
	
	PATTERNMASK_MACROS
};

#endif
