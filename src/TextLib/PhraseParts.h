#ifndef _TextLib_PhraseParts_h_
#define _TextLib_PhraseParts_h_


BEGIN_TEXTLIB_NAMESPACE


class PhrasePartsProcess : public SolverBase {
	
public:
	enum {
		PHASE_GET_LINE_ACTIONS,
		PHASE_GET_USING_EXISTING,
		PHASE_GET,
		
		PHASE_COUNT
	};
	
	void GetLineActions();
	void GetUsingExisting();
	void Get();

public:
	typedef PhrasePartsProcess CLASSNAME;
	PhrasePartsProcess();
	
	int GetPhaseCount() const override;
	int GetBatchCount(int phase) const override;
	int GetSubBatchCount(int phase, int batch) const override;
	void DoPhase() override;
	
	static PhrasePartsProcess& Get(int appmode);
	
};


END_TEXTLIB_NAMESPACE


#endif
