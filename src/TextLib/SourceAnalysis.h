#ifndef _TextLib_SourceAnalysis_h_
#define _TextLib_SourceAnalysis_h_


BEGIN_TEXTLIB_NAMESPACE


class SourceAnalysisProcess : public SolverBase {
	
public:
	enum {
		PHASE_ANALYZE_ARTISTS,
		PHASE_ANALYZE_ELEMENTS,
		PHASE_SUMMARIZE_CONTENT,
		
		PHASE_COUNT
	};
	
	bool filter_foreign = true;
	Atomic actual = 0, total = 0;
	TimeStop ts;
	SourceDataAnalysisArgs args;
	
public:
	typedef SourceAnalysisProcess CLASSNAME;
	SourceAnalysisProcess();
	
	int GetPhaseCount() const override;
	int GetBatchCount(int phase) const override;
	int GetSubBatchCount(int phase, int batch) const override;
	void DoPhase() override;
	
	static SourceAnalysisProcess& Get(int appmode);
	
private:
	void AnalyzeArtists();
	void AnalyzeElements();
	void SummarizeContent();
};


END_TEXTLIB_NAMESPACE


#endif
