#ifndef _SongTool_Pipeline_h_
#define _SongTool_Pipeline_h_


struct Pipe :
	SnapContext,
	TaskMgr,
	Attributes,
	AttrScore
{
	// Local
	VectorMap<String,String>	data;
	Array<Part>					parts;
	Array<Track>				tracks;
	String						structure_str;
	Vector<String>				structure;
	
	MArr<String>					content;
	MArr<VectorMap<String,String>>	analysis;
	
	int							rev_impact = -1;
	Vector<int>					rev_common_mask_tasks;
	Vector<int>					rev_separate_mask_tasks;
	Vector<int>					rev_pattern_tasks;
	Array<ReverseTask>			rev_tasks;
	
	// Local (from artist)
	String						vocalist_visual;
	//Attributes				attrs;
	//AttrScore					attrscores;
	
	// Temp
	Song*						song = 0;
	PipePtrs					p;
	RWMutex						lock;
	
	
	
	Pipe();
	Array<Part>& GetSub() {return parts;}
	const Array<Part>& GetSub() const {return parts;}
	
	/*void Jsonize(JsonIO& json) {
		json
			("data", data)
			("analysis", analysis)
			("parts", parts)
			("tracks", tracks)
			("structure_str", structure_str)
			("structure", structure)
			("content", content)
			;
		SnapContext::Jsonize(json);
		TaskMgr::Jsonize(json);
	}*/
	void RealizeProduction();
	int GetPartIdx(const Part& p) const;
	void RealizeTaskSnaps(bool force=false);
	PatternSnap* FindSnapByImpact(String impact_txt);
	void FixPtrs() {
		SetPipePtr(this);
		int id = 0;
		for (Part& p : parts) {
			p.CopyPtrs(*this);
			p.SetOwner(*this);
			p.SetId(id++);
			p.FixPtrs();
		}
	}
	int FindPartIdx(const String& name) const {
		int i = 0;
		for (const Part& p : parts) {
			if (p.name == name)
				return i;
			i++;
		}
		return -1;
	}
	const Part* FindPart(const String& name) const {
		for (const Part& p : parts)
			if (p.name == name)
				return &p;
		return 0;
	}
	Part* FindPart(const String& name) {
		for (Part& p : parts)
			if (p.name == name)
				return &p;
		return 0;
	}
	Part& GetAddPart(String name) {
		Part* ptr = FindPart(name);
		if (ptr) return *ptr;
		Part& p = parts.Add();
		p.name = name;
		FixPtrs();
		return p;
	}
	int GetLength(const SnapArg& a) const;
	
	String CreateLyricsFromBreaks(const SnapArg& a) const;
	
	PATTERNMASK_MACROS
};


#endif
