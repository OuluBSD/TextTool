#ifndef _TextLib_BiographySummaryProcess_h_
#define _TextLib_BiographySummaryProcess_h_


BEGIN_TEXTLIB_NAMESPACE


class BiographySummaryProcess : public SolverBase {
	
public:
	enum {
		PHASE_FIX_SUMMARY_HASHES,
		PHASE_SUMMARIZE_USING_EXISTING,
		PHASE_SUMMARIZE,
		PHASE_SUMMARIZE_ELEMENTS_USING_EXISTING,
		PHASE_SUMMARIZE_ELEMENTS,
		
		PHASE_COUNT,
	};
	
	Profile* p = 0;
	BiographySnapshot* snap = 0;
	
	
public:
	typedef BiographySummaryProcess CLASSNAME;
	BiographySummaryProcess();
	
	int GetPhaseCount() const override;
	int GetBatchCount(int phase) const override;
	int GetSubBatchCount(int phase, int batch) const override;
	void DoPhase() override;
	
	static BiographySummaryProcess& Get(Profile& p, BiographySnapshot& snap);
	
private:
	
	void FixSummaryHashes();
	void SummarizeUsingExisting();
	bool SummarizeBase(int fn, BiographySummaryProcessArgs& args);
	void Summarize();
	void SummarizeElementsUsingExisting();
	void SummarizeElements();
	void OnProcessSummarize(String res);
	void OnProcessSummarizeElements(String res);
	
};


END_TEXTLIB_NAMESPACE


#endif
