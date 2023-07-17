#ifndef _SongTool_Line_h_
#define _SongTool_Line_h_


struct Line : PatternSnap {
	Array<Break>				breaks;
	VectorMap<String,String>	data;
	
	void Jsonize(JsonIO& json) {
		json
			("breaks", breaks)
			("data", data)
			;
		PatternSnap::Jsonize(json);
	}
	void FixPtrs() {
		this->line = this;
		int id = 0;
		for (Break& b : breaks) {
			static_cast<Ptrs&>(b) = *(Ptrs*)this;
			b.owner = this;
			b.SetId(id++);
			b.FixPtrs();
		}
	}
	Array<Break>& GetSub() {return breaks;}
	const Array<Break>& GetSub() const {return breaks;}
	void ParseLine(Song& song, const String& txt);
	
	PATTERNMASK_MACROS
};


#endif
