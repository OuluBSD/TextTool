#ifndef _TextLib_ScriptRange_h_
#define _TextLib_ScriptRange_h_


BEGIN_TEXTLIB_NAMESPACE


class ScriptRangeProcess : public SolverBase {
	
public:
	enum {
		
		PHASE_COUNT
	};
	
public:
	typedef ScriptRangeProcess CLASSNAME;
	ScriptRangeProcess();
	
	int GetPhaseCount() const override;
	int GetBatchCount(int phase) const override;
	int GetSubBatchCount(int phase, int batch) const override;
	void DoPhase() override;
	
	static ScriptRangeProcess& Get(int appmode);
	
};


END_TEXTLIB_NAMESPACE


#endif
