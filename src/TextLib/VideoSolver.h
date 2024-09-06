#ifndef _TextLib_VideoSolver_h_
#define _TextLib_VideoSolver_h_


BEGIN_TEXTLIB_NAMESPACE


class VideoSolver : public SolverBase {
	
public:
	enum {
		PHASE_MAKE_STORYBOARD,
		PHASE_STORYBOARD_TO_PARTS_AND_DALLE_PROMPTS,
		PHASE_TEXT_TO_PARTS,
		PHASE_IMAGE_SEARCH_PROMPT,
		
		PHASE_COUNT,
		
		PHASE_FILL_STORY_PROMPTS,
		PHASE_GET_IMAGES,
		PHASE_SAFE_PROMPTS,
		PHASE_GET_SAFE_IMAGES,
		PHASE_GET_RUNWAY_STORYBOARD,
		
	};
	
	Component* comp = 0;
	Snapshot* snap = 0;
	Entity* entity = 0;
	
	Vector<String> tmp_lines;
	
	int arg_image_count = 4;
	
	void OnProcessAnalyzeRoleScores(String res);
	
public:
	typedef VideoSolver CLASSNAME;
	VideoSolver();
	
	int GetPhaseCount() const override;
	void DoPhase() override;
	
	static VideoSolver& Get(Component& c, int appmode);
	
};


END_TEXTLIB_NAMESPACE


#endif
