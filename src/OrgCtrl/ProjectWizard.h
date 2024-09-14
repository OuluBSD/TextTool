#ifndef _OrgCtrl_ProjectWizard_h_
#define _OrgCtrl_ProjectWizard_h_


BEGIN_TEXTLIB_NAMESPACE


class ProjectWizardView : public NodeViewBase {
	
public:
	typedef ProjectWizardView CLASSNAME;
	ProjectWizardView();
	
	void Data() override;
	
};

class ProjectWizardCtrl : public NodeCtrlBase {
	Splitter hsplit;
	ArrayCtrl topics, main;
	
	enum {
		VIEW_REQUIREMENTS,
		VIEW_DELIVERABLES,
		VIEW_TECHNOLOGIES,
		VIEW_DEVELOPMENT,
		VIEW_LANGUAGE,
		VIEW_STRUCTURE,
		VIEW_DEPENDENCIES,
		VIEW_DOCUMENTATION,
		VIEW_ARCHITECTURE,
		VIEW_HEADERS,
		VIEW_PERFORMANCE,
		VIEW_USER_INTERFACE,
		VIEW_DATA_SECURITY,
		VIEW_INTEGRATION,
		VIEW_MAINTENANCE,
		VIEW_ERROR_HANDLING,
		VIEW_SOURCE,
		
		VIEW_COUNT
	};
	
	static String GetViewName(int i);
	
public:
	typedef ProjectWizardCtrl CLASSNAME;
	ProjectWizardCtrl();
	
	void Data() override;
	void DataTopic();
	void ToolMenu(Bar& bar) override;
	void Do(int fn);
	
	void SetView(int i);
	
};


END_TEXTLIB_NAMESPACE


#endif
