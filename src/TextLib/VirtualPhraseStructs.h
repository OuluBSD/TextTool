#ifndef _TextLib_VirtualPhraseStructs_h_
#define _TextLib_VirtualPhraseStructs_h_


BEGIN_TEXTLIB_NAMESPACE


class VirtualPhraseStructsProcess : public SolverBase {
	int actual = 0;
	
public:
	enum {
		PHASE_GET,
		
		PHASE_COUNT
	};
	
	void Get();

public:
	typedef VirtualPhraseStructsProcess CLASSNAME;
	VirtualPhraseStructsProcess();
	
	int GetPhaseCount() const override;
	int GetBatchCount(int phase) const override;
	int GetSubBatchCount(int phase, int batch) const override;
	void DoPhase() override;
	
	static VirtualPhraseStructsProcess& Get(int appmode);
	
};


END_TEXTLIB_NAMESPACE


#endif
