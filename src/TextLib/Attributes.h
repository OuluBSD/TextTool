#ifndef _TextLib_Attributes_h_
#define _TextLib_Attributes_h_


BEGIN_TEXTLIB_NAMESPACE


class AttributesProcess : public SolverBase {
	
public:
	enum {
		
		PHASE_COUNT
	};
	
	void GetLineActions();
	void GetUsingExisting();
	void Get();

public:
	typedef AttributesProcess CLASSNAME;
	AttributesProcess();
	
	int GetPhaseCount() const override;
	int GetBatchCount(int phase) const override;
	int GetSubBatchCount(int phase, int batch) const override;
	void DoPhase() override;
	
	static AttributesProcess& Get(int appmode);
	
};


END_TEXTLIB_NAMESPACE


#endif
