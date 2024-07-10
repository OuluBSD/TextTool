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
	struct Section : Moveable<Section> {
		Vector<hash_t> hashes;
		int orig_count = 0;
		int count = 0;
		int orig_weight = 0;
		bool operator()(const Section& a, const Section& b) const {return a.hashes.GetCount() > b.hashes.GetCount();}
		//bool operator()(const Section& a, const Section& b) const {return a.orig_weight > b.orig_weight;}
	};
	struct UniqueLine : Moveable<UniqueLine> {
		String txt;
		Vector<int> lines;
		Vector<int> possible_sections;
		int count = 0;
		bool operator()(const UniqueLine& a, const UniqueLine& b) const {return a.count > b.count;}
		String ToString() const {return IntStr(count) + ": " + txt;}
	};
	struct Line : Moveable<Line> {
		TextDescriptor descriptor;
		hash_t hash = 0;
		String txt;
		int section = -1;
	};
	
	String input;
	Vector<Line> lines;
	Vector<Section> sections;
	VectorMap<hash_t,UniqueLine> uniq_lines;
	
	// Params
	int section_cmp_header_len = 6;
	
	
	
public:
	typedef ScriptStructureSolverBase CLASSNAME;
	ScriptStructureSolverBase();
	
	void Process(String s);
	String GetResult() const;
	String GetDebugHashes() const;
	String GetDebugSections() const;
	String FindLine(hash_t h) const;
	
protected:
	void DefaultProcess();
	
	virtual void MakeLines();
	virtual void MakeUniqueLines();
	virtual void MakeSections() = 0;
	virtual void MakeRepeatingSectionLines() = 0;
	virtual void MakeSingleLineSections();
	
};

class TryWithTransferStructureSolver : public ScriptStructureSolverBase {
	
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

class TryNo3tStructureSolver : public ScriptStructureSolverBase {
	
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

class TryNo4tStructureSolver : public TryWithTransferStructureSolver {
	
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
	
public:
	typedef MultiScriptStructureSolver CLASSNAME;
	MultiScriptStructureSolver();
	
	void Process(String s);
	String GetResult();
	
};


END_TEXTLIB_NAMESPACE


#endif
