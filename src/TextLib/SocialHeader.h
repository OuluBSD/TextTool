#ifndef _TextLib_SocialHeader_h_
#define _TextLib_SocialHeader_h_


BEGIN_TEXTLIB_NAMESPACE


class SocialHeaderProcess : public SolverBase {
	
public:
	enum {
		PHASE_AUDIENCE_REACTS_SUMMARY,
		
		PHASE_COUNT,
	};
	
	Profile* p = 0;
	BiographySnapshot* snap = 0;
	
	Vector<BiographyProfileAnalysis*> ptrs;
	Vector<const RoleProfile*> prof_ptrs;
	Vector<String> role_descs;
	
public:
	typedef SocialHeaderProcess CLASSNAME;
	SocialHeaderProcess();
	
	int GetPhaseCount() const override;
	int GetBatchCount(int phase) const override;
	int GetSubBatchCount(int phase, int batch) const override;
	void DoPhase() override;
	
	static SocialHeaderProcess& Get(Profile& p, BiographySnapshot& snap);
	
private:
	
	void ProcessAudienceReactsSummary();
	void OnProcessAudienceReactsSummary(String res);
	
};


END_TEXTLIB_NAMESPACE


#endif
