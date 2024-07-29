#ifndef _TextLib_PlatformProcess_h_
#define _TextLib_PlatformProcess_h_


BEGIN_TEXTLIB_NAMESPACE


class PlatformProcess : public SolverBase {
	
public:
	enum {
		PHASE_ANALYZE_ROLE_SCORES,
		PHASE_ANALYZE_PLATFORM_ROLES,
		PHASE_ANALYZE_PLATFORM_EPK_TEXT_FIELDS,
		PHASE_ANALYZE_PLATFORM_EPK_PHOTO_TYPES,
		PHASE_ANALYZE_PLATFORM_EPK_PHOTO_AI_PROMPTS,
		
		PHASE_COUNT,
	};
	
	Profile* p = 0;
	BiographySnapshot* snap = 0;
	
	
	struct VisionTask : Moveable<VisionTask> {
		BioImage* bimg = 0;
		String jpeg;
	};
	Vector<VisionTask> vision_tasks;
	void TraverseVisionTasks();
	
	
	struct ImageSummaryTask : Moveable<ImageSummaryTask> {
		BiographyCategory* bcat = 0;
		BioYear* by = 0;
		BioImage* summary = 0;
		BioRange range;
		int bcat_i = -1;
	};
	Vector<ImageSummaryTask> imgsum_tasks;
	void TraverseImageSummaryTasks();
	
	
public:
	typedef PlatformProcess CLASSNAME;
	PlatformProcess();
	
	int GetPhaseCount() const override;
	int GetBatchCount(int phase) const override;
	int GetSubBatchCount(int phase, int batch) const override;
	void DoPhase() override;
	
	static PlatformProcess& Get(Profile& p, BiographySnapshot& snap);
	
private:
	
	void ProcessAnalyzeRoleScores();
	void ProcessAnalyzePlatformRoles();
	void ProcessAnalyzePlatformEpkTextFields();
	void ProcessAnalyzePlatformEpkPhotoTypes();
	void ProcessAnalyzePlatformEpkPhotoAiPrompts();
	void OnProcessAnalyzeRoleScores(String res);
	void OnProcessAnalyzePlatformRoles(String res);
	void OnProcessAnalyzePlatformEpkTextFields(String res);
	void OnProcessAnalyzePlatformEpkPhotoTypes(String res);
	void OnProcessAnalyzePlatformEpkPhotoAiPrompts(String res);
	
};


END_TEXTLIB_NAMESPACE


#endif
