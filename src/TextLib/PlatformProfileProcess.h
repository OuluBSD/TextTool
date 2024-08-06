#ifndef _TextLib_PlatformProfileProcess_h_
#define _TextLib_PlatformProfileProcess_h_


BEGIN_TEXTLIB_NAMESPACE


class PlatformProfileProcess : public SolverBase {
	
public:
	enum {
		PHASE_ANALYZE_PROFILE_EPK_PHOTO_AI_PROMPTS,
		PHASE_ANALYZE_PROFILE_EPK_SUMMARIZE_PHOTO_AI_PROMPTS,
		PHASE_ANALYZE_PROFILE_EPK_PHOTO_DALLE2_EXAMPLES,
		
		PHASE_COUNT,
	};
	
	Profile* p = 0;
	BiographySnapshot* snap = 0;
	
	
	struct ProfileEPKTask : Moveable<ProfileEPKTask> {
		PlatformBiographyAnalysis* pba = 0;
		const Platform* plat = 0;
		const PlatformAnalysis* pa = 0;
		const PlatformAnalysisPhoto* pap = 0;
		PlatformAnalysisPhoto* prof_pap = 0;
		PhotoPrompt* pp = 0;
	};
	Vector<ProfileEPKTask> prof_epk_tasks;
	void TraverseProfileEPKTasks();
	
	
public:
	typedef PlatformProfileProcess CLASSNAME;
	PlatformProfileProcess();
	
	int GetPhaseCount() const override;
	int GetBatchCount(int phase) const override;
	int GetSubBatchCount(int phase, int batch) const override;
	void DoPhase() override;
	
	static PlatformProfileProcess& Get(Profile& p, BiographySnapshot& snap);
	
private:
	
	void ProcessAnalyzeProfileEpkPhotoAiPrompts();
	void ProcessAnalyzeProfileEpkSummarizePhotoAiPrompts();
	void ProcessAnalyzeProfileEpkPhotoDalle2Examples();
	void OnProcessAnalyzeProfileEpkPhotoAiPrompts(String res);
	void OnProcessAnalyzeProfileEpkSummarizePhotoAiPrompts(String res);
	void OnProcessAnalyzeProfileEpkPhotoDalle2Examples(Array<Image>& images, int batch);
	
};


END_TEXTLIB_NAMESPACE


#endif