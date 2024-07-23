#ifndef _TextLib_PhraseParts_h_
#define _TextLib_PhraseParts_h_


BEGIN_TEXTLIB_NAMESPACE


class ActionAttrsProcess : public SolverBase {
	VectorMap<String, VectorMap<String, int>> uniq_acts;
	int per_action_clrs = 60;
	int per_action_attrs = 40;
	int actual = 0, total = 0;
	ActionAnalysisArgs args;
	
public:
	enum {
		PHASE_COLORS_USING_EXISTING,
		PHASE_COLORS,
		PHASE_ATTRS_USING_EXISTING,
		PHASE_ATTRS,
		
		PHASE_COUNT
	};
	
	void PrepareUsingExisting();
	void Prepare(int fn);
	void ColorsUsingExisting();
	void Colors();
	void AttrsUsingExisting();
	void Attrs();

	int BatchCount(int phase) const;
	
public:
	typedef ActionAttrsProcess CLASSNAME;
	ActionAttrsProcess();
	
	int GetPhaseCount() const override;
	int GetBatchCount(int phase) const override;
	int GetSubBatchCount(int phase, int batch) const override;
	void DoPhase() override;
	
	static ActionAttrsProcess& Get(int appmode);
	
};


END_TEXTLIB_NAMESPACE


#endif
