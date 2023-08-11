#ifndef _SongTool_Part_h_
#define _SongTool_Part_h_

struct Part :
	SnapContext
{
	// Local
	String name;
	Array<Line>						lines;
	Composition						composition;
	MArr<VectorMap<String,String>>	analysis;
	MArr<Index<String>>				unique_lines;
	
	void Clear() {
		lines.Clear();
		SnapContext::Clear();
	}
	void Serialize(Stream& s) {
		s	% name
			% lines
			% composition
			% analysis;
		SnapContext::Serialize(s);
	}
	/*void Jsonize(JsonIO& json) {
		json
			("name", name)
			("lines", lines)
			("composition", composition)
			("analysis", analysis)
			;
		SnapContext::Jsonize(json);
	}*/
	String ToString() const {return name + ", lines=" + IntStr(lines.GetCount());}
	void FixPtrs() {
		SetPartPtr(this);
		int id = 0;
		for (Line& l : lines) {
			l.CopyPtrs(*this);
			l.SetOwner(*this);
			l.SetId(id++);
			l.FixPtrs();
		}
	}
	Array<Line>& GetSub() {return lines;}
	const Array<Line>& GetSub() const {return lines;}
	int GetLength(const SnapArg& a) const;
	int GetLineIdx(const Line& l) const;
	String CreateLyricsFromBreaks(const SnapArg& a, bool add_comma) const;
	
	PATTERNMASK_MACROS
};

#endif
