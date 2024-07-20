#ifndef _TextLib_ScriptStructureSolver_h_
#define _TextLib_ScriptStructureSolver_h_


BEGIN_TEXTLIB_NAMESPACE


/* TextDescriptor's ideas is based on the ORB image feature descriptor:
- 'the weighted center' is 'the most common transform from character to character'
- 'orientation match' is the getting of the common subset of 'character to character transforms'
*/
struct TextDescriptor : Moveable<TextDescriptor> {
	struct Item : Moveable<Item> {
		uint16 from, to;
		uint16 count;
		
		bool operator()(const Item& a, const Item& b) const {
			if (a.count != b.count) return a.count > b.count;
			if (a.from != b.from) return a.from < b.from;
			return a.to < b.to;
		}
		void Zero() {from = 0; to = 0; count = 0;}
	};
	
	
	static constexpr int ITEM_COUNT = 50;
	
	
	Item items[ITEM_COUNT];
	
	void Zero() {memset(items, 0, sizeof(items));}
	hash_t GetHash(int c) const;
	
};


class ScriptStructureSolverBase {
	
	
public:
	struct MetaSection : Moveable<MetaSection> {
		Vector<int> sections;
		int type = -1;
		int num = -1;
		int count = 0;
	};
	struct UniqueLine : Moveable<UniqueLine> {
		String txt;
		Vector<int> lines;
		Vector<int> possible_sections;
		int count = 0;
		bool operator()(const UniqueLine& a, const UniqueLine& b) const {return a.count > b.count;}
		String ToString() const {return IntStr(count) + ": " + txt;}
	};
	
	String input;
	Vector<MetaSection> meta_sections;
	String debug_out;
	
	// Params
	int section_cmp_header_len = 3;
	double cr_limit = 0.3;
	bool force_limit = false;
	double forced_limit_value = 0;
	double repeatability_range = 0.2;
	
	// Temp
	double iter_r_limit = 0;
	
	
public:
	typedef ScriptStructureSolverBase CLASSNAME;
	ScriptStructureSolverBase();
	
	void Process(String s);
	void SetForcedLimit(double d) {forced_limit_value = d; force_limit = true;}
	
	String GetDebugOut() const {return debug_out;}
	
protected:
	virtual void DefaultProcess() = 0;
	void SingleIteration();
	
	virtual void MakeLines() = 0;
	virtual void MakeUniqueLines() = 0;
	virtual void MakeSections() = 0;
	virtual void MakeRepeatingSectionLines() = 0;
	virtual void MakeSingleLineSections() = 0;
	virtual void MakeMetaSections() = 0;
	
public:
	virtual String GetDebugLines() const = 0;
	virtual String GetDebugSections() const = 0;
	virtual String GetResult() const = 0;
	virtual String GetDebugHashes() const = 0;
	virtual String FindLine(hash_t h) const = 0;
	
};

class TryHashSectionSolverBase : public ScriptStructureSolverBase {
	
public:
	struct Line : Moveable<Line> {
		TextDescriptor descriptor;
		hash_t hash = 0;
		String txt;
		int section = -1;
		double repeatability = 0;
		double circumstacial_repeatability = 0;
		double GetRepeatabilitySum() const {return repeatability + circumstacial_repeatability;}
	};
	struct Section : Moveable<Section> {
		Vector<hash_t> hashes;
		int orig_count = 0;
		int count = 0;
		int orig_weight = 0;
		double repeat = 0;
		int first_line = -1;
		bool flag_repeating = false;
		bool operator()(const Section& a, const Section& b) const {return a.hashes.GetCount() > b.hashes.GetCount();}
	};
	
	Vector<Line> lines;
	Vector<Section> sections;
	VectorMap<hash_t,UniqueLine> uniq_lines;
	
	
	void DefaultProcess() override;
	void MakeLines() override;
	void MakeUniqueLines() override;
	void MakeSingleLineSections() override;
	void MakeMetaSections() override;
	String GetDebugLines() const override;
	String GetDebugSections() const override;
	String GetResult() const override;
	String GetDebugHashes() const override;
	String FindLine(hash_t h) const override;
	
};

class TryStrDistSectionSolverBase : public ScriptStructureSolverBase {
	
public:
	struct Line : Moveable<Line> {
		TextDescriptor descriptor;
		String txt;
		int section = -1;
		double repeatability = 0;
		double circumstacial_repeatability = 0;
		double GetRepeatabilitySum() const {return repeatability + circumstacial_repeatability;}
	};
	struct Section : Moveable<Section> {
		Vector<String> lines;
		int meta_section = -1;
		int orig_count = 0;
		int count = 0;
		int orig_weight = 0;
		double repeat = 0;
		int first_line = -1;
		bool flag_repeating = false;
		bool operator()(const Section& a, const Section& b) const {return a.lines.GetCount() > b.lines.GetCount();}
	};
	struct HashRangeLink : Moveable<HashRangeLink> {
		SoftMatchString k;
		Vector<int> v;
	};
	
	Vector<Line> lines;
	Vector<Section> sections;
	Vector<UniqueLine> uniq_lines;
	double unique_dist_limit = 0.95;
	Vector<HashRangeLink> hash_ranges;
	
	UniqueLine& GetAddUniqueLine(const String& s);
	Vector<int>& GetAddHashRange(const SoftMatchString& s);
	
	void DefaultProcess() override;
	void MakeLines() override;
	void MakeUniqueLines() override;
	void MakeSingleLineSections() override;
	void MakeMetaSections() override;
	String GetDebugLines() const override;
	String GetDebugSections() const override;
	String GetResult() const override;
	String GetDebugHashes() const override;
	String FindLine(hash_t h) const override;
	int GetMetaSectionLen(int ms_i) const;
	
};

class TryWithTransferStructureSolver : public TryHashSectionSolverBase {
	
public:
	struct Transfer : Moveable<Transfer> {
		struct Vec : Moveable<Vec> {
			int line = -1;
			int weight = 0;
			int len = 0;
			bool operator()(const Vec& a, const Vec& b) const {return a.weight > b.weight;}
		};
		hash_t from, to;
		int count;
		Vector<Vec> vectors;
		
		bool operator()(const Transfer& a, const Transfer& b) const {
			if (a.count != b.count) return a.count > b.count;
			if (a.from != b.from) return a.from < b.from;
			return a.to < b.to;
		}
	};
	
	Vector<Transfer> transfers;
	
	Transfer& GetAddTransfer(hash_t h0, hash_t h1);
	Transfer* FindTransfer(hash_t h);
	Transfer* FindTransfer(hash_t from, hash_t to);
	
};

class TryNo1tStructureSolver : public TryWithTransferStructureSolver {
	
public:
	void MakeSections() override;
	void MakeRepeatingSectionLines() override;
	
};

class TryNo2tStructureSolver : public TryWithTransferStructureSolver {
	
public:
	void MakeSections() override;
	void MakeRepeatingSectionLines() override;
	
};

class TryNo3tStructureSolver : public TryHashSectionSolverBase {
	
	struct TempSection {
		Vector<int> lines;
		int length = 0;
		
		void Chk() {
			for(int i = 0; i < lines.GetCount(); i++) {
				for(int j = i+1; j < lines.GetCount(); j++) {
					ASSERT(lines[i] != lines[j]);
				}
			}
		}
		String ToString() const {
			String s = "weight=" + IntStr(length*lines.GetCount()) + ",length=" + IntStr(length) + ": ";
			for(int i = 0; i < lines.GetCount(); i++) {if (i) s << ", "; s << lines[i];}
			return s;
		}
		bool operator()(const TempSection& a, const TempSection& b) const {
			return (a.length*a.lines.GetCount()) > (b.length*b.lines.GetCount());
		}
	};
	
	Array<TempSection> v;
	
public:
	void MakeSections() override;
	void MakeRepeatingSectionLines() override;
	
};

class TryNo4tStructureSolver : public TryHashSectionSolverBase {
	
public:
	void MakeSections() override;
	void MakeRepeatingSectionLines() override;
	
};

class TryNo5tStructureSolver : public TryStrDistSectionSolverBase {
	
public:
	void MakeSections() override;
	void MakeRepeatingSectionLines() override;
	
};

class MultiScriptStructureSolver {
	int c = -1;
	TryNo1tStructureSolver t1;
	TryNo2tStructureSolver t2;
	TryNo3tStructureSolver t3;
	TryNo4tStructureSolver t4;
	TryNo5tStructureSolver t5;
	
public:
	typedef MultiScriptStructureSolver CLASSNAME;
	MultiScriptStructureSolver();
	
	ScriptStructureSolverBase& Get();
	
};

typedef TryNo5tStructureSolver BestStructureSolver:

END_TEXTLIB_NAMESPACE


#endif
