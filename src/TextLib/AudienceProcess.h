#ifndef _TextLib_AudienceProcess_h_
#define _TextLib_AudienceProcess_h_


BEGIN_TEXTLIB_NAMESPACE


class AudienceProcess : public SolverBase {
	
public:
	enum {
		PHASE_AUDIENCE_PROFILE_CATEGORIES,
		
		PHASE_COUNT,
	};
	
	Profile* p = 0;
	BiographySnapshot* snap = 0;
	
	
public:
	typedef AudienceProcess CLASSNAME;
	AudienceProcess();
	
	int GetPhaseCount() const override;
	int GetBatchCount(int phase) const override;
	int GetSubBatchCount(int phase, int batch) const override;
	void DoPhase() override;
	
	static AudienceProcess& Get(Profile& p, BiographySnapshot& snap);
	
private:
	
	void ProcessAudienceProfileCategories();
	void OnProcessAudienceProfileCategories(String res);
	
};


END_TEXTLIB_NAMESPACE


#endif
