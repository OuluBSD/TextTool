#ifndef _TextLib_VirtualPhrases_h_
#define _TextLib_VirtualPhrases_h_


BEGIN_TEXTLIB_NAMESPACE


class VirtualPhrasesProcess : public SolverBase {
	
public:
	enum {
		PHASE_IMPORT_TOKEN_TEXTS,
		PHASE_GET_PARTS_USING_EXISTING,
		PHASE_GET_PARTS,
		PHASE_GET_USING_EXISTING,
		PHASE_GET,
		
		
		PHASE_COUNT
	};
	
	void ImportTokenTexts();
	void GetPartsUsingExisting();
	void GetParts();
	void GetUsingExisting();
	void Get();

public:
	typedef VirtualPhrasesProcess CLASSNAME;
	VirtualPhrasesProcess();
	
	int GetPhaseCount() const override;
	int GetBatchCount(int phase) const override;
	int GetSubBatchCount(int phase, int batch) const override;
	void DoPhase() override;
	
	static VirtualPhrasesProcess& Get(int appmode);
	
};


END_TEXTLIB_NAMESPACE


#endif
