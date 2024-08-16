#ifndef _TextLib_MarketplaceProcess_h_
#define _TextLib_MarketplaceProcess_h_


BEGIN_TEXTLIB_NAMESPACE


class MarketplaceProcess : public SolverBase {
	
public:
	enum {
		PHASE_DESCRIPTION,
		
		PHASE_COUNT
	};
	
	Owner* owner = 0;
	
public:
	typedef MarketplaceProcess CLASSNAME;
	MarketplaceProcess();
	
	int GetPhaseCount() const override;
	int GetBatchCount(int phase) const override;
	int GetSubBatchCount(int phase, int batch) const override;
	void DoPhase() override;
	
	static MarketplaceProcess& Get(Owner& o);
	
	void ProcessDescription();
	void MakeArgs(MarketplaceArgs& args);
	
};


END_TEXTLIB_NAMESPACE


#endif
