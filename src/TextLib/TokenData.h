#ifndef _TextLib_TokenData_h_
#define _TextLib_TokenData_h_


BEGIN_TEXTLIB_NAMESPACE


class TokenDataProcess : public SolverBase {
	
public:
	enum {
		PHASE_GET_USING_EXISTING,
		PHASE_GET,
		
		PHASE_COUNT
	};
	
	int actual = 0, total = 0;
	TokenArgs token_args;
	
	void GetUsingExisting();
	void Get();

public:
	typedef TokenDataProcess CLASSNAME;
	TokenDataProcess();
	
	int GetPhaseCount() const override;
	int GetBatchCount(int phase) const override;
	int GetSubBatchCount(int phase, int batch) const override;
	void DoPhase() override;
	
	static TokenDataProcess& Get(int appmode);
	
};


END_TEXTLIB_NAMESPACE


#endif
