#ifndef _SongTool_Line_h_
#define _SongTool_Line_h_


struct Line :
	SnapContext
{
	// Local
	Array<Break>				breaks;
	VectorMap<String,String>	data;
	
	void Serialize(Stream& s) {
		s	% breaks
			% data;
		SnapContext::Serialize(s);
	}
	/*void Jsonize(JsonIO& json) {
		json
			("breaks", breaks)
			("data", data)
			;
		SnapContext::Jsonize(json);
	}*/
	void FixPtrs() {
		this->SetLinePtr(this);
		int id = 0;
		for (Break& b : breaks) {
			b.CopyPtrs(*this);
			b.SetOwner(*this);
			b.SetId(id++);
			b.FixPtrs();
		}
	}
	Array<Break>& GetSub() {return breaks;}
	const Array<Break>& GetSub() const {return breaks;}
	void ParseLine(const SnapArg& a, const String& txt);
	int GetLength(const SnapArg& a) const;
	int GetBreakIdx(const Break& b0) const;
	
	PATTERNMASK_MACROS
};


#endif
