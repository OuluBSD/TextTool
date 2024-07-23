#ifndef _TextLib_ScriptText_h_
#define _TextLib_ScriptText_h_


BEGIN_TEXTLIB_NAMESPACE


class ScriptTextProcess : public SolverBase {
	
public:
	enum {
		
		PHASE_COUNT
	};
	
public:
	typedef ScriptTextProcess CLASSNAME;
	ScriptTextProcess();
	
	int GetPhaseCount() const override;
	int GetBatchCount(int phase) const override;
	int GetSubBatchCount(int phase, int batch) const override;
	void DoPhase() override;
	
	static ScriptTextProcess& Get(int appmode);
	
};


END_TEXTLIB_NAMESPACE


#endif
