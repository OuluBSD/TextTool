#ifndef _TextLib_VirtualPhrases_h_
#define _TextLib_VirtualPhrases_h_


BEGIN_TEXTLIB_NAMESPACE


class VirtualPhrasesProcess : public SolverBase {
	int actual = 0, total = 0;
	Vector<void*> tmp_ptrs;
	TokenArgs token_args;
	
	// Params
	int per_action_task = 40;
	
public:
	enum {
		PHASE_IMPORT_TOKEN_TEXTS,
		PHASE_GET_PARTS_USING_EXISTING,
		PHASE_GET_PARTS,
		
		
		PHASE_COUNT
	};
	
	void ImportTokenTexts();
	void GetPartsUsingExisting();
	void GetParts();

public:
	typedef VirtualPhrasesProcess CLASSNAME;
	VirtualPhrasesProcess();
	
	int GetPhaseCount() const override;
	int GetBatchCount(int phase) const override;
	int GetSubBatchCount(int phase, int batch) const override;
	void DoPhase() override;
	
	static VirtualPhrasesProcess& Get(int appmode);
	
};


bool GetTypePhrase(Vector<int>& types, const DatasetAnalysis& da, int next_w_i, int w_i, int prev_w_i);


END_TEXTLIB_NAMESPACE


#endif
