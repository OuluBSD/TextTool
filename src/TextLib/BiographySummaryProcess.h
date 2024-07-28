#ifndef _TextLib_BiographySummaryProcess_h_
#define _TextLib_BiographySummaryProcess_h_


BEGIN_TEXTLIB_NAMESPACE


class BiographySummaryProcess : public SolverBase {
	
public:
	enum {
		PHASE_SUMMARIZE,
		
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
	
	void Summarize();
	void OnProcessSummarize(String res);
	
};


END_TEXTLIB_NAMESPACE


#endif
