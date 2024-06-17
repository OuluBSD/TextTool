#ifndef _TextLib_BeliefSolver_h_
#define _TextLib_BeliefSolver_h_


BEGIN_TEXTLIB_NAMESPACE


class BeliefSolver : public SolverBase {
	
public:
	enum {
		PHASE_GET_POSITIVE_ATTRS,
		PHASE_GET_NEGATIVE_ATTRS,
		
		PHASE_COUNT
	};
	
	
public:
	typedef BeliefSolver CLASSNAME;
	BeliefSolver();
	
	int GetPhaseCount() const override;
	void DoPhase() override;
	
	static BeliefSolver& Get();
	
	Event<> WhenReady;
	
};


END_TEXTLIB_NAMESPACE


#endif
