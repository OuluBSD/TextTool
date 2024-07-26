#ifndef _TextLib_ScriptText_h_
#define _TextLib_ScriptText_h_


BEGIN_TEXTLIB_NAMESPACE


class ScriptTextProcess : public SolverBase {
	
public:
	enum {
		// Copied from ScriptGenerator
		PHASE_MAKE_SOURCE_POOL,
		PHASE_TRANSLATE,
		PHASE_MAKE_PHRASE_PAIRS,
		PHASE_MAKE_RHYMES,
		PHASE_GET_AI_SCORES,
		
		// Copied ScriptSolver
		PHASE_COLLECT,
		PHASE_FILL_LINES,
		PHASE_COMPARISON,
		
		PHASE_COUNT
	};
	
	struct ConvTask : Moveable<ConvTask> {
		Vector<String> from, ref;
		String part;
	};
	
	// params
	int per_sub_batch =  50;
	int pair_limit = 300;
	int phrase_limit = 150;
	double dist_limit = 0.005;
	int primary_count = 50;
	int rhyming_list_count = 5;
	int sugg_limit = 6;
	bool start_post_solver = false;
	
	// temp
	Entity* artist = 0;
	Script* script = 0;
	Vector<VectorMap<int,double>> phrase_ints;
	Vector<int> tmp;
	int iter = 0;
	VectorMap<String, int> pp_is;
	Vector<VectorMap<int,double>> phrase_parts;
	Vector<String> phrase_strs;
	Vector<Tuple2<int,int>> matches;
	Index<int> remaining;
	VectorMap<String,int> part_sizes;
	ComponentAnalysis* sa = 0;
	Vector<int> phrase_src;
	String active_part;
	Index<hash_t> visited;
	Vector<ConvTask> conv_tasks;
	Index<String> added_phrases;
	
	
	
	void ProcessSourcePool();
	void ProcessTranslate();
	void ProcessPairPhrases();
	void ProcessRhymes();
	void ProcessScores();
	void ProcessCollect();
	void ProcessFillLines();
	void ProcessComparison();
	void OnProcessPairPhrases(String result);
	void OnProcessRhymes(String result);
	void OnProcessScores(String result);
	void OnProcessTranslate(String result);
	void OnProcessFillLines(String res);
	void OnProcessComparison(String res);
	void OnProcessComparisonFail(int loser);
	
public:
	typedef ScriptTextProcess CLASSNAME;
	ScriptTextProcess();
	
	int GetPhaseCount() const override;
	int GetBatchCount(int phase) const override;
	int GetSubBatchCount(int phase, int batch) const override;
	void DoPhase() override;
	
	static ScriptTextProcess& Get(int appmode, Entity& a, Script& l);
	
};


END_TEXTLIB_NAMESPACE


#endif
