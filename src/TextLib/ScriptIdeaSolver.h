#ifndef _TextLib_ScriptIdeaSolver_h_
#define _TextLib_ScriptIdeaSolver_h_


BEGIN_TEXTLIB_NAMESPACE


class ScriptIdeaSolver : public SolverBase {
	
public:
	enum {
		PHASE_GET_IDEAS,
		PHASE_GET_SCORES_OF_IDEAS,
		
		PHASE_COUNT
	};
	
	Owner* owner = 0;
	Profile* profile = 0;
	Entity* entity = 0;
	int appmode = 0;
	bool only_single = false;
	int only_tc = -1;
	int only_con = -1;
	
public:
	typedef ScriptIdeaSolver CLASSNAME;
	ScriptIdeaSolver();
	
	int GetPhaseCount() const override;
	void DoPhase() override;
	
	static ScriptIdeaSolver& Get(Entity& e, int appmode);
	
	
};


END_TEXTLIB_NAMESPACE


#endif
