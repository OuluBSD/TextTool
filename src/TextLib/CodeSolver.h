#ifndef _TextLib_CodeSolver_h_
#define _TextLib_CodeSolver_h_


BEGIN_TEXTLIB_NAMESPACE


class CodeSolver : public SolverBase {
	
public:
	enum {
		PHASE_GET_PLATFORMS,
		PHASE_GET_OPERATING_SYSTEMS,
		PHASE_GET_SW_LANGUAGES,
		PHASE_GET_SW_PARADIGMS,
		PHASE_GET_IDE,
		PHASE_GET_LIBRARIES,
		PHASE_GET_PROJECT_TYPES,
		
		PHASE_COUNT
	};
	
	
	VectorMap<int, Index<int>> tmp_map;
	
public:
	typedef CodeSolver CLASSNAME;
	CodeSolver();
	
	int GetPhaseCount() const override;
	void DoPhase() override;
	
	static CodeSolver& Get(int appmode);
	
};


END_TEXTLIB_NAMESPACE


#endif
