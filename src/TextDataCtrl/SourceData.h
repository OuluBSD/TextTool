#ifndef _TextDataCtrl_SourceData_h_
#define _TextDataCtrl_SourceData_h_


BEGIN_TEXTLIB_NAMESPACE


class SourceDataCtrl : public ToolAppCtrl {
	Splitter vsplit, hsplit;
	ArrayCtrl entities, components;//, active_components;
	DocEdit scripts, analysis;
	
public:
	typedef SourceDataCtrl CLASSNAME;
	SourceDataCtrl();
	
	void Data() override;
	void DataEntity();
	void DataComponent();
	void ToolMenu(Bar& bar) override;
	void Do(int fn);
	
};


END_TEXTLIB_NAMESPACE


#endif
