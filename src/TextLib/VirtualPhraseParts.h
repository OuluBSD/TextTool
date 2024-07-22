#ifndef _TextLib_VirtualPhraseParts_h_
#define _TextLib_VirtualPhraseParts_h_


BEGIN_TEXTLIB_NAMESPACE


class VirtualPhrasePartsProcess : public SolverBase {
	TokenArgs token_args;
	Vector<void*> tmp_ptrs;
	
public:
	enum {
		PHASE_GET_USING_EXISTING,
		PHASE_GET,
		
		PHASE_COUNT
	};
	
	void GetUsingExisting();
	void Get();

public:
	typedef VirtualPhrasePartsProcess CLASSNAME;
	VirtualPhrasePartsProcess();
	
	int GetPhaseCount() const override;
	int GetBatchCount(int phase) const override;
	int GetSubBatchCount(int phase, int batch) const override;
	void DoPhase() override;
	
	static VirtualPhrasePartsProcess& Get(int appmode);
	
};


END_TEXTLIB_NAMESPACE


#endif
