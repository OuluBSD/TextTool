#ifndef _TextLib_PhrasePartAnalysis_h_
#define _TextLib_PhrasePartAnalysis_h_


BEGIN_TEXTLIB_NAMESPACE


class PhrasePartAnalysisProcess : public SolverBase {
	PhraseArgs phrase_args;
	TokenArgs token_args;
	Vector<void*> tmp_ptrs;
	Vector<int> tmp, tmp_iters;
	
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
	
	void Do(int fn);
	void OnPhraseColors(String result);
	void OnPhraseAttrs(String result);
	void OnPhraseActions(String result);
	void OnPhraseScores(String result);
	void OnPhraseTypeclasses(String result);
	void OnPhraseContrast(String result);

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
