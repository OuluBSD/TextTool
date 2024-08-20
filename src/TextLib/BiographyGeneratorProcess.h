#ifndef _TextLib_BiographyGeneratorProcess_h_
#define _TextLib_BiographyGeneratorProcess_h_


BEGIN_TEXTLIB_NAMESPACE


class BiographyGeneratorProcess : public SolverBase {
	
public:
	enum {
		PHASE_GENERATE,
		PHASE_GENERATE_DETAILS,
		
		PHASE_COUNT,
	};
	
	Profile* p = 0;
	BiographySnapshot* snap = 0;
	
	struct Task : Moveable<Task> {
		int year, category, year_i;
	};
	Vector<Task> tasks;
	
public:
	typedef BiographyGeneratorProcess CLASSNAME;
	BiographyGeneratorProcess();
	
	int GetPhaseCount() const override;
	int GetBatchCount(int phase) const override;
	int GetSubBatchCount(int phase, int batch) const override;
	void DoPhase() override;
	
	static BiographyGeneratorProcess& Get(Profile& p, BiographySnapshot& snap);
	
private:
	void Generate();
	void GenerateDetails();
	
};


END_TEXTLIB_NAMESPACE


#endif
