#ifndef _TextLib_ScriptIdeaSolver_h_
#define _TextLib_ScriptIdeaSolver_h_


BEGIN_TEXTLIB_NAMESPACE


class ScriptIdeaSolver : public SolverBase {
	
public:
	enum {
		PHASE_GET_IDEAS,
		PHASE_GET_LEAD_IDEAS,
		PHASE_GET_SCORES_OF_IDEAS,
		
		PHASE_COUNT
	};
	
	Owner* owner = 0;
	Profile* profile = 0;
	Entity* entity = 0;
	ContentVisionOwner* cvo = 0;
	Script* script = 0;
	
	bool only_single = false;
	int only_tc = -1;
	int only_con = -1;
	
	void ParseSuggestions(String res, bool types, int tc_i, int con_i);
	
public:
	typedef ScriptIdeaSolver CLASSNAME;
	ScriptIdeaSolver();
	
	int GetPhaseCount() const override;
	void DoPhase() override;
	
	static ScriptIdeaSolver& Get(Entity& e, Script* s, int appmode);
	
	
};


END_TEXTLIB_NAMESPACE


#endif
