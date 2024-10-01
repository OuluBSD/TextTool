#ifndef _TextLib_SnapSolver_h_
#define _TextLib_SnapSolver_h_

BEGIN_TEXTLIB_NAMESPACE


class SnapSolver : public SolverBase {
	
public:
	enum {
		SS_LYRICS_SUMMARIES,
		SS_LYRICS_SUMMARY,
		SS_LYRICS_PSYCHOANALYSIS,
		SS_LYRICS_SOCIAL_PSYCHOLOGY_ANALYSIS,
		SS_MARKET_VALUE_ANALYSIS,
		SS_MARKETING_SUGGESTION,
		SS_ART_SUGGESTION,
		SS_COVER_SUGGESTION,
		SS_COVER_SUGGESTION_DALLE2,
		
		SS_COUNT
	};
	
	Snapshot* snap = 0;
	Entity* entity = 0;
	
	void OnProcessAnalyzeRoleScores(String res);
	
public:
	typedef SnapSolver CLASSNAME;
	SnapSolver();
	
	int GetPhaseCount() const override;
	void DoPhase() override;
	
	static SnapSolver& Get(Snapshot& s, int appmode);
	
};


END_TEXTLIB_NAMESPACE

#endif
