#ifndef _TextLib_ImageBiographySummaryProcess_h_
#define _TextLib_ImageBiographySummaryProcess_h_


BEGIN_TEXTLIB_NAMESPACE


class ImageBiographySummaryProcess : public SolverBase {
	
public:
	enum {
		PHASE_SUMMARIZE_IMAGE_CATEGORY_YEAR,
		PHASE_SUMMARIZE_IMAGE_BIOGRAPHY,
		
		PHASE_COUNT,
	};
	
	Profile* p = 0;
	BiographySnapshot* snap = 0;
	
	
	
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
	typedef ImageBiographySummaryProcess CLASSNAME;
	ImageBiographySummaryProcess();
	
	int GetPhaseCount() const override;
	int GetBatchCount(int phase) const override;
	int GetSubBatchCount(int phase, int batch) const override;
	void DoPhase() override;
	
	static ImageBiographySummaryProcess& Get(Profile& p, BiographySnapshot& snap);
	
private:
	
	void ProcessSummarizeImageCategoryYear();
	void ProcessSummarizeImageBiography();
	void OnProcessSummarizeImageCategoryYear(String res);
	void OnProcessSummarizeImageBiography(String res);
	
};


END_TEXTLIB_NAMESPACE


#endif
