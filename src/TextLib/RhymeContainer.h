#ifndef _TextLib_RhymeContainer_h_
#define _TextLib_RhymeContainer_h_


BEGIN_TEXTLIB_NAMESPACE


class RhymeContainerProcess : public SolverBase {
	
public:
	enum {
		PHASE_GET,
		PHASE_GET_NANA,
		
		PHASE_COUNT
	};
	
	void Get();
	void GetNana();

public:
	typedef RhymeContainerProcess CLASSNAME;
	RhymeContainerProcess();
	
	int GetPhaseCount() const override;
	int GetBatchCount(int phase) const override;
	int GetSubBatchCount(int phase, int batch) const override;
	void DoPhase() override;
	
	static RhymeContainerProcess& Get(int appmode);
	
};


END_TEXTLIB_NAMESPACE


#endif
