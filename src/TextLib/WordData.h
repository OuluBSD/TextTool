#ifndef _TextLib_WordData_h_
#define _TextLib_WordData_h_


BEGIN_TEXTLIB_NAMESPACE


class WordDataProcess : public SolverBase {
	
public:
	enum {
		PHASE_DETAILS,
		PHASE_SYLLABLES,
		
		PHASE_COUNT
	};
	

public:
	typedef WordDataProcess CLASSNAME;
	WordDataProcess();
	
	int GetPhaseCount() const override;
	int GetBatchCount(int phase) const override;
	int GetSubBatchCount(int phase, int batch) const override;
	void DoPhase() override;
	
	static WordDataProcess& Get(int appmode);
	
	void Details();
	void Syllables();
	
};


END_TEXTLIB_NAMESPACE

#endif
