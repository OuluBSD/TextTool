#ifndef _TextLib_AmbiguousWordPairs_h_
#define _TextLib_AmbiguousWordPairs_h_


BEGIN_TEXTLIB_NAMESPACE


class AmbiguousWordPairsProcess : public SolverBase {
	TokenArgs token_args;
	Vector<void*> tmp_ptrs;
	
	// Params
	int per_action_task = 100;
	
public:
	enum {
		PHASE_GET_USING_EXISTING,
		PHASE_GET,
		
		PHASE_COUNT
	};
	
	void GetUsingExisting();
	void Get();

public:
	typedef AmbiguousWordPairsProcess CLASSNAME;
	AmbiguousWordPairsProcess();
	
	int GetPhaseCount() const override;
	int GetBatchCount(int phase) const override;
	int GetSubBatchCount(int phase, int batch) const override;
	void DoPhase() override;
	
	static AmbiguousWordPairsProcess& Get(int appmode);
	
};


END_TEXTLIB_NAMESPACE


#endif
