#ifndef _TextLib_ScriptRange_h_
#define _TextLib_ScriptRange_h_


BEGIN_TEXTLIB_NAMESPACE


class ScriptRangeProcess : public SolverBase {
	
public:
	enum {
		PHASE_COLOR,
		PHASE_ATTR,
		PHASE_ACTION,
		PHASE_MAKE_SOURCE_POOL,
		PHASE_TRANSLATE,
		PHASE_MAKE_PHRASE_PAIRS,
		PHASE_MAKE_RHYMES,
		PHASE_GET_AI_SCORES,
		
		
		PHASE_COUNT,
	};
	
	void ProcessColor();
	void ProcessAttr();
	void ProcessAction();
	void ProcessTranslate();
	void ProcessPairPhrases();
	void ProcessScores();
	void OnProcessColor(String result);
	void OnProcessAttr(String result);
	void OnProcessPairPhrases(String result);
	void OnProcessRhymes(String result);
	void OnProcessScores(String result);
	void OnProcessTranslate(String result);
	
	Entity* artist = 0;
	Script* script = 0;
	
	
	
public:
	typedef ScriptRangeProcess CLASSNAME;
	ScriptRangeProcess();
	
	int GetPhaseCount() const override;
	void DoPhase() override;
	
	static ScriptRangeProcess& Get(int appmode, Entity& a, Script& l);
	
};


END_TEXTLIB_NAMESPACE


#endif
