#ifndef _TextLib_SocialSolver_h_
#define _TextLib_SocialSolver_h_


BEGIN_TEXTLIB_NAMESPACE


class SocialSolver {
	enum {
		SS_BEGIN,
		
		SS_ANALYZE_ROLE_SCORES,
		SS_ANALYZE_PLATFORM_ROLES,
		SS_ANALYZE_PLATFORM_EPK_TEXT_FIELDS,
		SS_ANALYZE_PLATFORM_EPK_PHOTO_TYPES,
		SS_ANALYZE_PLATFORM_EPK_PHOTO_AI_PROMPTS,
		SS_ANALYZE_PROFILE_EPK_PHOTO_AI_PROMPTS,
		SS_ANALYZE_PROFILE_EPK_SUMMARIZE_PHOTO_AI_PROMPTS,
		SS_ANALYZE_PROFILE_EPK_PHOTO_DALLE2_EXAMPLES,
		/*
		SS_ANALYZE_PLATFORM_EPK_VIDEO_TYPES,
		SS_ANALYZE_PLATFORM_EPK_VIDEO_STORYBOARD_AI_PROMPTS,
		SS_ANALYZE_PLATFORM_EPK_VIDEO_STORYBOARD_DALLE2,
		SS_ANALYZE_PLATFORM_EPK_HOW_TO_PHOTO,
		*/
		SS_AUDIENCE_PROFILE_CATEGORIES,
		
		SS_ANALYZE_IMAGE_BIOGRAPHY,
		SS_SUMMARIZE_IMAGE_CATEGORY_YEAR,
		SS_SUMMARIZE_IMAGE_BIOGRAPHY,
		
		SS_SUMMARIZE,
		SS_AUDIENCE_REACTS_SUMMARY,
		SS_PACK_ROLE_REACTIONS,
		SS_PACK_PLATFORM_REACTIONS,
		SS_PLATFORM_DESCRIPTIONS,
		SS_PLATFORM_DESCRIPTION_REFINEMENTS,
		SS_PLATFORM_DESCRIPTION_TRANSLATED,
		SS_MERGE_MESSAGES,
		
		SS_COUNT
	};
	Time time_started, time_stopped;
	int phase = SS_BEGIN;
	int batch = 0, sub_batch = 0, batch_count = 0, per_batch = 0;
	Owner* owner = 0;
	Profile* profile = 0;
	
	bool waiting = false;
	bool running = false, stopped = true;
	bool skip_ready = true;
	
	Vector<BiographyProfileAnalysis*> ptrs;
	Vector<const RoleProfile*> prof_ptrs;
	Vector<String> role_descs;
	
	struct Range : Moveable<Range> {
		int off = 0, len = 0;
		int input[2] = {0,0};
		String ToString() const {return Format("%d, %d: %d, %d", off, len, input[0], input[1]);}
	};
	Vector<Range> ranges;
	
	
	struct MessageTask : Moveable<MessageTask> {
		const Platform* plat = 0;
		PlatformData* plat_data = 0;
		PlatformEntry* entry = 0;
		PlatformThread* thrd = 0;
		Vector<PlatformComment*> comments;
	};
	Vector<MessageTask> msg_tasks;
	MessageTask tmp_task;
	void TraverseMessageTasks(int prof_i, int plat_i);
	void TraverseMessageTasks(Vector<PlatformComment*>& before, PlatformComment& plc);
	
	
	struct VisionTask : Moveable<VisionTask> {
		BioImage* bimg = 0;
		String jpeg;
	};
	Vector<VisionTask> vision_tasks;
	MessageTask tmp_vision_task;
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
	
	
	int CreateRange(int off, int len);
	void OnBatchError();
	void Process();
	void ProcessAnalyzeRoleScores();
	void ProcessAnalyzePlatformRoles();
	void ProcessAnalyzePlatformEpkTextFields();
	void ProcessAnalyzePlatformEpkPhotoTypes();
	void ProcessAnalyzePlatformEpkPhotoAiPrompts();
	void ProcessAnalyzeProfileEpkPhotoAiPrompts();
	void ProcessAnalyzeProfileEpkSummarizePhotoAiPrompts();
	void ProcessAnalyzeProfileEpkPhotoDalle2Examples();
	void ProcessAudienceProfileCategories();
	void ProcessSummarize();
	void ProcessAudienceReactsSummary();
	void ProcessRoleReactions();
	void ProcessPlatformReactions();
	void ProcessPlatformDescriptions();
	void ProcessPlatformDescriptionRefinements();
	void ProcessPlatformDescriptionTranslated();
	void ProcessMergeMessages();
	void ProcessAnalyzeImageBiography();
	void ProcessSummarizeImageCategoryYear();
	void ProcessSummarizeImageBiography();
	void OnProcessAnalyzeRoleScores(String res);
	void OnProcessAnalyzePlatformRoles(String res);
	void OnProcessAnalyzePlatformEpkTextFields(String res);
	void OnProcessAnalyzePlatformEpkPhotoTypes(String res);
	void OnProcessAnalyzePlatformEpkPhotoAiPrompts(String res);
	void OnProcessAnalyzeProfileEpkPhotoAiPrompts(String res);
	void OnProcessAnalyzeProfileEpkSummarizePhotoAiPrompts(String res);
	void OnProcessAnalyzeProfileEpkPhotoDalle2Examples(Array<Image>& images, int batch);
	void OnProcessAudienceProfileCategories(String res);
	void OnProcessSummarize(String res);
	void OnProcessAudienceReactsSummary(String res);
	void OnProcessRoleReactions(String res);
	void OnProcessPlatformReactions(String res);
	void OnProcessPlatformDescriptions(String res);
	void OnProcessPlatformDescriptionRefinements(String res);
	void OnProcessPlatformDescriptionTranslated(String res);
	void OnProcessMergeMessages(String res);
	void OnProcessAnalyzeImageBiography(String res);
	void OnProcessSummarizeImageCategoryYear(String res);
	void OnProcessSummarizeImageBiography(String res);
	
	void PostProgress() {WhenProgress(phase, SS_COUNT);}
	void SetNotRunning() {running = false;}
	void SetWaiting(bool b) {waiting = b;}
	void MovePhase(int p) {phase = p; batch = 0; sub_batch = 0;}
	void NextPhase() {phase++; batch = 0; sub_batch = 0;}
	void NextBatch() {batch++; sub_batch = 0;}
	void NextSubBatch() {sub_batch++;}
	
public:
	typedef SocialSolver CLASSNAME;
	SocialSolver();
	
	void Start() {if (!running) {running = true; stopped = false; Thread::Start(THISBACK(Process));}}
	void Stop() {running = false; while (!stopped) Sleep(1);}
	
	static SocialSolver& Get(Profile& p);
	static void ClearTasks();
	static void RestartTasks();
	
	Callback2<int,int> WhenProgress;
	bool only_categories = true;
	
};


END_TEXTLIB_NAMESPACE


#endif
