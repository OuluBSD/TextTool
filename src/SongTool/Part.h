#ifndef _SongTool_Part_h_
#define _SongTool_Part_h_

struct Part :
	SnapContext
{
	String name;
	
	Array<Line>			lines;
	Composition			composition;
	Analysis			analysis;
	
	void Clear() {
		lines.Clear();
		SnapContext::Clear();
	}
	void Jsonize(JsonIO& json) {
		json
			("name", name)
			("lines", lines)
			("composition", composition)
			("analysis", analysis)
			;
		SnapContext::Jsonize(json);
	}
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
	
	
	PATTERNMASK_MACROS
};

#endif
