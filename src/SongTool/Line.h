#ifndef _SongTool_Line_h_
#define _SongTool_Line_h_


struct Line :
	SnapContext
{
	Array<Break>				breaks;
	VectorMap<String,String>	data;
	
	void Jsonize(JsonIO& json) {
		json
			("breaks", breaks)
			("data", data)
			;
		SnapContext::Jsonize(json);
	}
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
	void ParseLine(Song& song, int mode, const String& txt);
	int GetLength(int mode) const;
	
	PATTERNMASK_MACROS
};


#endif
