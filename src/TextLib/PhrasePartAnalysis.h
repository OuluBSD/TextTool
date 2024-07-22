#ifndef _TextLib_PhrasePartAnalysis_h_
#define _TextLib_PhrasePartAnalysis_h_


BEGIN_TEXTLIB_NAMESPACE


class PhrasePartAnalysisProcess : public SolverBase {
	
public:
	enum {
		PHASE_COLOR,
		PHASE_ATTR,
		PHASE_ACTIONS,
		PHASE_SCORES,
		PHASE_TYPECLASS,
		PHASE_CONTENT,
		
		PHASE_COUNT
	};
	
	void GetColor();
	void GetAttr();
	void GetActions();
	void GetScores();
	void GetTypeclass();
	void GetContent();

public:
	typedef PhrasePartAnalysisProcess CLASSNAME;
	PhrasePartAnalysisProcess();
	
	int GetPhaseCount() const override;
	int GetBatchCount(int phase) const override;
	int GetSubBatchCount(int phase, int batch) const override;
	void DoPhase() override;
	
	static PhrasePartAnalysisProcess& Get(int appmode);
	
};


END_TEXTLIB_NAMESPACE


#endif
