#ifndef _TextLib_ScriptSolver_h_
#define _TextLib_ScriptSolver_h_


BEGIN_TEXTLIB_NAMESPACE


class ScriptSolver : public SolverBase {
	enum {
		LS_FILL_LINES,
		LS_COMPARISON,
		
		LS_COUNT,
		
		LS_MATCH_REFERENCE,
		LS_SCORE_MATCH,
		LS_FILL_REFERENCE_MATCH,
		LS_SMOOTH_REFERENCE_MATCH,
		LS_TITLE,
		
	};
	Entity* artist = 0;
	Script* script = 0;
	
	// params
	double dist_limit = 0.005;
	int primary_count = 50;
	int rhyming_list_count = 5;
	int sugg_limit = 6;
	bool start_post_solver = false;
	
	// temp
	Vector<VectorMap<int,double>> phrase_parts;
	Vector<String> phrases;
	Vector<Tuple2<int,int>> matches;
	Index<int> remaining;
	VectorMap<String,int> part_sizes;
	ComponentAnalysis* sa = 0;
	Vector<int> phrase_src;
	String active_part;
	Index<hash_t> visited;
	
	struct ConvTask : Moveable<ConvTask> {
		Vector<String> from, ref;
		String part;
	};
	Vector<ConvTask> conv_tasks;
	Index<String> added_phrases;
	
	void Process();
	void ClearScript();
	void ProcessFillLines();
	void ProcessPrimary();
	void ProcessMakeHoles();
	void ProcessComparison();
	void ProcessReference();
	void ProcessScoreMatch();
	void ProcessTitle();
	void ProcessFillReferenceMatch();
	void ProcessSmoothReferenceMatch();
	void OnProcessPrimary(String res);
	void OnProcessFillLines(String res);
	void OnProcessMakeHoles(String res);
	void OnProcessComparison(String res);
	void OnProcessComparisonFail(int loser);
	void OnProcessReference(String res);
	void OnProcessScoreMatch(String res);
	void OnProcessFillReferenceMatch(String res);
	void OnProcessSmoothReferenceMatch(String res);
	
	TextDatabase& GetDatabase() {return GetAppModeDatabase(appmode);}
	int GetTypeclassCount() {return TextLib::GetTypeclassCount(appmode);}
	int GetContentCount() {return TextLib::GetContentCount(appmode);}
	
public:
	typedef ScriptSolver CLASSNAME;
	ScriptSolver();
	
	int GetPhaseCount() const override;
	void DoPhase() override;
	static ScriptSolver& Get(int appmode, Entity& a, Script& l);
	
	void StartPostSolver(bool b=true) {start_post_solver = b;}
	
	Callback2<int,int> WhenProgress;
	
};


END_TEXTLIB_NAMESPACE


#endif
