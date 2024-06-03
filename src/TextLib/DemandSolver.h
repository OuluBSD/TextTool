#ifndef _TextLib_DemandSolver_h_
#define _TextLib_DemandSolver_h_


BEGIN_TEXTLIB_NAMESPACE

/*
Notes:
	- single page text/manifest for startup
	- growing phase / maintaining phase / legacy phase
	- fast (everyday) / regular (weekly or bi-weekly) / slow (from monthly to once in a half year)
	- list of products that can be demanded:
		songs, albums, tweets, blogs, reels, music videos, podcasts
	- list of "ingredients of minimum viable product" e.g. product of products
		- what static albums, messages, reels, etc. must be out at minimum
	- list of promotion that can be demanded for the product
		- attention from fast to regular / from regular to slow
	- list of demands for images
		- profile, target group, etc.
*/
class DemandSolver : public SolverBase {
	
public:
	enum {
		PHASE_NEEDS_PER_ROLE,
		PHASE_CAUSES_PER_NEED,
		PHASE_MESSAGES_PER_NEED,
		PHASE_PLATFORMS_PER_CAUSE,
		PHASE_ACTION_CAUSES,
		PHASE_EVENT_ENTRIES,
		
		PHASE_COUNT
	};
	
	Owner* owner = 0;
	
	struct Task : Moveable<Task> {
		Role* r = 0;
		Need* n = 0;
		RoleAction* ra = 0;
		RoleEvent* re = 0;
		Vector<bool> enabled;
	};
	Vector<Task> tasks;
	Vector<bool> enabled_tmp;
	
	void OnProcessAnalyzeRoleScores(String res);
	
public:
	typedef DemandSolver CLASSNAME;
	DemandSolver();
	
	int GetPhaseCount() const override;
	void DoPhase() override;
	
	static DemandSolver& Get(Owner& c);
	
};


END_TEXTLIB_NAMESPACE


#endif
