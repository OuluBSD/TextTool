#ifndef _TextLib_VideoSolver_h_
#define _TextLib_VideoSolver_h_


BEGIN_TEXTLIB_NAMESPACE


class VideoSolver : public SolverBase {
	
public:
	enum {
		PHASE_MAKE_STORYBOARD,
		PHASE_STORYBOARD_TO_PARTS_AND_DALLE_PROMPTS,
		PHASE_TEXT_TO_PARTS,
		PHASE_FILL_STORY_PROMPTS,
		
		PHASE_COUNT
	};
	
	Component* comp = 0;
	Snapshot* snap = 0;
	Entity* entity = 0;
	
	Vector<String> tmp_lines;
	
	void OnProcessAnalyzeRoleScores(String res);
	
public:
	typedef VideoSolver CLASSNAME;
	VideoSolver();
	
	int GetPhaseCount() const override;
	void DoPhase() override;
	
	static VideoSolver& Get(Component& c);
	
};


END_TEXTLIB_NAMESPACE


#endif
