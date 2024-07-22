#ifndef _TextLib_TokenPhrases_h_
#define _TextLib_TokenPhrases_h_


BEGIN_TEXTLIB_NAMESPACE


class TokenPhrasesProcess : public SolverBase {
	
public:
	enum {
		PHASE_UNKNOWN_PAIRS,
		
		PHASE_COUNT
	};
	
	void UnknownPairs();

public:
	typedef TokenPhrasesProcess CLASSNAME;
	TokenPhrasesProcess();
	
	int GetPhaseCount() const override;
	int GetBatchCount(int phase) const override;
	int GetSubBatchCount(int phase, int batch) const override;
	void DoPhase() override;
	
	static TokenPhrasesProcess& Get(int appmode);
	
};


END_TEXTLIB_NAMESPACE


#endif
