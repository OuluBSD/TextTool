#ifndef _TextDataCtrl_LeadTemplates_h_
#define _TextDataCtrl_LeadTemplates_h_


BEGIN_TEXTLIB_NAMESPACE



class LeadTemplateCtrl : public ToolAppCtrl {
	ArrayCtrl templates;
	
public:
	typedef LeadTemplateCtrl CLASSNAME;
	LeadTemplateCtrl();
	
	void Data() override;
	void ToolMenu(Bar& bar) override;
	void Do(int fn);
	void ExportJson();
	
};


END_TEXTLIB_NAMESPACE


#endif
