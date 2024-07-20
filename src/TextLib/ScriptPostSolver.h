#ifndef _TextLib_ScriptPostSolver_h_
#define _TextLib_ScriptPostSolver_h_


BEGIN_TEXTLIB_NAMESPACE


class ScriptPostSolver : public SolverBase {
	
public:
	// This solver uses generations: it loops phases multiple times
	enum {
		PHASE_ANALYSE_WEAKNESSES,
		PHASE_IMPROVE_WEAKNESSES,
		PHASE_SCORE_VARIATIONS,
		
		PHASE_COUNT
	};
	
	Owner* owner = 0;
	Profile* profile = 0;
	Script* script = 0;
	Entity* entity = 0;
	
	// Genetic Solver params
	int variation_count = 10;
	double mutation_factor = 0.5; // 50%
	
	struct Variation {
		Vector<String> lines;
		Vector<int> scores;
		int ScoreSum() const {int s = 0; for (int i : scores) s += i; return s;}
		double ScoreAv() const {int s = 0; for (int i : scores) s += i; return (double)s/scores.GetCount();}
		#if 0
		int rank = -1;
		bool operator()(const Variation& a, const Variation& b) const {return a.rank < b.rank;}
		#else
		bool operator()(const Variation& a, const Variation& b) const {return a.ScoreSum() >= b.ScoreSum();}
		#endif
	};
	Array<Variation> variations;
	Index<int> remaining;
	Vector<Tuple2<int,int>> matches;
	
	void TraverseMessageTasks(int prof_i, int plat_i);
	void TraverseMessageTasks(Vector<PlatformComment*>& before, PlatformComment& plc);
	
	
	void ProcessMergeMessages();
	void OnProcessMergeMessages(String res);
	
public:
	typedef ScriptPostSolver CLASSNAME;
	ScriptPostSolver();
	
	int GetPhaseCount() const override;
	void DoPhase() override;
	
	static ScriptPostSolver& Get(Profile& p, Entity& e, Script& s, int appmode);
	
};


END_TEXTLIB_NAMESPACE


#endif
