#ifndef _TextLib_SourceDataImporter_h_
#define _TextLib_SourceDataImporter_h_


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


class BasicScriptStructureSolver {
	
	
public:
	struct Section : Moveable<Section> {
		Vector<hash_t> hashes;
		int orig_count = 0;
		int count = 0;
		int orig_weight = 0;
		bool operator()(const Section& a, const Section& b) const {return a.hashes.GetCount() > b.hashes.GetCount();}
		//bool operator()(const Section& a, const Section& b) const {return a.orig_weight > b.orig_weight;}
	};
	/*struct Transfer : Moveable<Transfer> {
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
	};*/
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
	
	Vector<Line> lines;
	Vector<Section> sections;
	VectorMap<hash_t,UniqueLine> uniq_lines;
	//Vector<Transfer> transfers;
	
	// Params
	int section_cmp_header_len = 6;
	
	
	/*Transfer& GetAddTransfer(hash_t h0, hash_t h1);
	Transfer* FindTransfer(hash_t h);
	Transfer* FindTransfer(hash_t from, hash_t to);*/
	String FindLine(hash_t h) const;
	
public:
	typedef BasicScriptStructureSolver CLASSNAME;
	BasicScriptStructureSolver();
	
	void Process(String s);
	String GetResult() const;
	
};

class SourceDataImporter : public SolverBase {
	
public:
	enum {
		PHASE_,
		
		PHASE_COUNT
	};
	
	int appmode = -1;
	
public:
	typedef SourceDataImporter CLASSNAME;
	SourceDataImporter();
	
	int GetPhaseCount() const override;
	void DoPhase() override;
	
	static SourceDataImporter& Get(int appmode);
	
};


END_TEXTLIB_NAMESPACE


#endif
