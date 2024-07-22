#ifndef _TextLib_AmbiguousWordPairs_h_
#define _TextLib_AmbiguousWordPairs_h_


BEGIN_TEXTLIB_NAMESPACE


class AmbiguousWordPairsProcess : public SolverBase {
	
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
