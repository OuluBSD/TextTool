#ifndef _TextLib_SocialBeliefsProcess_h_
#define _TextLib_SocialBeliefsProcess_h_


BEGIN_TEXTLIB_NAMESPACE


class SocialBeliefsProcess : public SolverBase {
	
public:
	enum {
		PHASE_GET_POSITIVE_ATTRS,
		PHASE_GET_NEGATIVE_ATTRS,
		
		PHASE_COUNT
	};
	
	Owner* owner = 0;
	Profile* profile = 0;
	BiographySnapshot* snap = 0;
	
public:
	typedef SocialBeliefsProcess CLASSNAME;
	SocialBeliefsProcess();
	
	int GetPhaseCount() const override;
	void DoPhase() override;
	
	static SocialBeliefsProcess& Get(Profile& p, BiographySnapshot& snap);
	
	
};


END_TEXTLIB_NAMESPACE


#endif
