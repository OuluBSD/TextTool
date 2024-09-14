#include "OrgCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


ProjectWizardView::ProjectWizardView() {
	
}

void ProjectWizardView::Data() {
	
}



ProjectWizardCtrl::ProjectWizardCtrl() {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << topics << main;
	hsplit.SetPos(1500);
	
	topics.AddColumn("Topic");
	for(int i = 0; i < VIEW_COUNT; i++) {
		topics.Set(i, 0, GetViewName(i));
	}
	topics.SetCursor(0);
	topics.WhenCursor << THISBACK(DataTopic);
	
	
}

String ProjectWizardCtrl::GetViewName(int i) {
	switch (i) {
		case VIEW_REQUIREMENTS:		return "Define project requirements and scope";
		case VIEW_DELIVERABLES:		return "Create a project plan and timeline";
		case VIEW_TECHNOLOGIES:		return "Identify necessary tools and technologies";
		case VIEW_DEVELOPMENT:		return "Set up a development environment";
		case VIEW_LANGUAGE:			return "Choose a programming language";
		case VIEW_STRUCTURE:		return "Determine project structure and organization";
		case VIEW_DEPENDENCIES:		return "Install and configure necessary dependencies and libraries";
		case VIEW_DOCUMENTATION:	return "Write design documentation";
		case VIEW_ARCHITECTURE:		return "Consider program architecture and design patterns";
		case VIEW_HEADERS:			return "Consider data structures, classes, and functions needed";
		case VIEW_PERFORMANCE:		return "Consider performance and efficiency";
		case VIEW_USER_INTERFACE:	return "Consider user interface design and user experience";
		case VIEW_DATA_SECURITY:	return "Consider data security and privacy";
		case VIEW_INTEGRATION:		return "Consider integration with other systems or platforms";
		case VIEW_MAINTENANCE:		return "Consider maintenance and future updates";
		case VIEW_ERROR_HANDLING:	return "Consider error handling and debugging strategies";
		case VIEW_SOURCE:			return "Create source code files";
		default: return "Error";
	}
}

void ProjectWizardCtrl::Data() {
	
	// WizardPlan& wp;
	
	DataTopic();
}

void ProjectWizardCtrl::DataTopic() {
	if (!topics.IsCursor())
		return;
	
	
}

void ProjectWizardCtrl::ToolMenu(Bar& bar) {
	
}

void ProjectWizardCtrl::Do(int fn) {
	
}


END_TEXTLIB_NAMESPACE
