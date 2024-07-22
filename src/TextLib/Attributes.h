#ifndef _TextLib_Attributes_h_
#define _TextLib_Attributes_h_


BEGIN_TEXTLIB_NAMESPACE


class AttributesProcess : public SolverBase {
	
	struct AttrExtremesBatch : Moveable<AttrExtremesBatch> {
		String group;
	};
	Vector<AttrExtremesBatch> attr_extremes_batches;
	
	struct Batch : Moveable<Batch> {
		EntityDataset* artist;
		ScriptDataset* scripts;
		String txt;
		int lng_i;
		bool song_begins;
	};
	Vector<Batch> batches;
	
	struct AttrPolarBatch : Moveable<AttrPolarBatch> {
		String group, attr0, attr1;
		Vector<String> attrs;
	};
	Vector<AttrPolarBatch> attr_polar_batches;
	
	struct AttrJoinBatch : Moveable<AttrJoinBatch> {
		Vector<String> groups, values;
	};
	Vector<AttrJoinBatch> attr_join_batches;
	
	
	String tmp_str;
	VectorMap<String,Index<String>> uniq_attrs;
	Index<String> tmp_words, tmp_words2;
	
public:
	enum {
		PHASE_MAIN_GROUPS,
		PHASE_SIMPLIFY_ATTRS,
		PHASE_JOIN_ORPHANED,
		PHASE_FIX_DATA,
		
		PHASE_COUNT
	};
	
	void MainGroups();
	void SimplifyAttrs();
	void JoinOrphaned();
	void FixData();
	void RealizeBatch_AttrExtremesBatch();
	
public:
	typedef AttributesProcess CLASSNAME;
	AttributesProcess();
	
	int GetPhaseCount() const override;
	int GetBatchCount(int phase) const override;
	int GetSubBatchCount(int phase, int batch) const override;
	void DoPhase() override;
	
	static AttributesProcess& Get(int appmode);
	
};


END_TEXTLIB_NAMESPACE


#endif
