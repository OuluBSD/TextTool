#ifndef _TextLib_ScriptStructure_h_
#define _TextLib_ScriptStructure_h_


BEGIN_TEXTLIB_NAMESPACE


class ScriptStructureProcess : public SolverBase {
	
public:
	enum {
		
		PHASE_COUNT
	};
	
public:
	typedef ScriptStructureProcess CLASSNAME;
	ScriptStructureProcess();
	
	int GetPhaseCount() const override;
	int GetBatchCount(int phase) const override;
	int GetSubBatchCount(int phase, int batch) const override;
	void DoPhase() override;
	
	static ScriptStructureProcess& Get(int appmode);
	
};


END_TEXTLIB_NAMESPACE


#endif
