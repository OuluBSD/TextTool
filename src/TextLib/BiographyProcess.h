#ifndef _TextLib_BiographyProcess_h_
#define _TextLib_BiographyProcess_h_


BEGIN_TEXTLIB_NAMESPACE


class BiographyProcess : public SolverBase {
	
public:
	enum {
		PHASE_ELEMENTS_USING_EXISTING,
		PHASE_ELEMENTS_SINGLE_YEAR,
		PHASE_ELEMENT_SCORES,
		
		PHASE_COUNT,
	};
	
	Profile* p = 0;
	BiographySnapshot* snap = 0;
	
	
public:
	typedef BiographyProcess CLASSNAME;
	BiographyProcess();
	
	int GetPhaseCount() const override;
	int GetBatchCount(int phase) const override;
	int GetSubBatchCount(int phase, int batch) const override;
	void DoPhase() override;
	
	static BiographyProcess& Get(Profile& p, BiographySnapshot& snap);
	
private:
	
	void GetElementsUsingExisting();
	void ElementsForSingleYears();
	void GetElementScores();
	
};


END_TEXTLIB_NAMESPACE


#endif
