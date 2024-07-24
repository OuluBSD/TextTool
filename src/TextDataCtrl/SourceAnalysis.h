#ifndef _TextDataCtrl_SourceAnalysis_h_
#define _TextDataCtrl_SourceAnalysis_h_


BEGIN_TEXTLIB_NAMESPACE


class SourceAnalysisCtrl : public ToolAppCtrl {
	Splitter vsplit, hsplit;
	ArrayCtrl entities, components;//, active_components;
	DocEdit scripts, analysis;
	
public:
	typedef SourceAnalysisCtrl CLASSNAME;
	SourceAnalysisCtrl();
	
	void Data() override;
	void DataEntity();
	void DataComponent();
	void ToolMenu(Bar& bar) override;
	void Do(int fn);
	
};


END_TEXTLIB_NAMESPACE


#endif
