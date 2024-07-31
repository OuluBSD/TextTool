#ifndef _TextLib_ScriptRange_h_
#define _TextLib_ScriptRange_h_


BEGIN_TEXTLIB_NAMESPACE


class ScriptRangeProcess : public SolverBase {
	
public:
	enum {
		PHASE_COLOR,
		PHASE_ATTR,
		PHASE_ACTION,
		LS_COLLECT,
		
		PHASE_COUNT,
	};
	
	void ProcessColor();
	void ProcessAttr();
	void ProcessAction();
	void ProcessCollect();
	void OnProcessColor(String result);
	void OnProcessAttr(String result);
	
	
	Entity* artist = 0;
	Script* script = 0;
	
	Vector<VectorMap<int,double>> phrase_parts;
	
	
public:
	typedef ScriptRangeProcess CLASSNAME;
	ScriptRangeProcess();
	
	int GetPhaseCount() const override;
	int GetBatchCount(int phase) const override;
	int GetSubBatchCount(int phase, int batch) const override;
	void DoPhase() override;
	
	static ScriptRangeProcess& Get(int appmode, Entity& a, Script& l);
	
};


END_TEXTLIB_NAMESPACE


#endif
