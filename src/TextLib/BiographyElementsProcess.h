#ifndef _TextLib_BiographyElementsProcess_h_
#define _TextLib_BiographyElementsProcess_h_


BEGIN_TEXTLIB_NAMESPACE


class BiographyElementsProcess : public SolverBase {
	
public:
	enum {
		PHASE_SEARCH_TOPIC_BUNDLES,
		PHASE_GET_TOPIC_BUNDLE_ELEMENTS,
		
		PHASE_COUNT
	};
	
	Profile* p = 0;
	BiographySnapshot* snap = 0;
	
	
public:
	typedef BiographyElementsProcess CLASSNAME;
	BiographyElementsProcess();
	
	int GetPhaseCount() const override;
	int GetBatchCount(int phase) const override;
	int GetSubBatchCount(int phase, int batch) const override;
	void DoPhase() override;
	
	static BiographyElementsProcess& Get(Profile& p, BiographySnapshot& snap);
	
private:
	
	void SearchTopicBundles();
	void GetTopicBundleElements();
	
};


END_TEXTLIB_NAMESPACE


#endif
