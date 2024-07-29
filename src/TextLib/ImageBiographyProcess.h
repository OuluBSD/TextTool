#ifndef _TextLib_ImageBiographyProcess_h_
#define _TextLib_ImageBiographyProcess_h_


BEGIN_TEXTLIB_NAMESPACE


class ImageBiographyProcess : public SolverBase {
	
public:
	enum {
		PHASE_ANALYZE_IMAGE_BIOGRAPHY,
		
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
	
	
	
	
public:
	typedef ImageBiographyProcess CLASSNAME;
	ImageBiographyProcess();
	
	int GetPhaseCount() const override;
	int GetBatchCount(int phase) const override;
	int GetSubBatchCount(int phase, int batch) const override;
	void DoPhase() override;
	
	static ImageBiographyProcess& Get(Profile& p, BiographySnapshot& snap);
	
private:
	
	void ProcessAnalyzeImageBiography();
	void OnProcessAnalyzeImageBiography(String res);
	
};


END_TEXTLIB_NAMESPACE


#endif
