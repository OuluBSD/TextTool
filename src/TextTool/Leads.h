#ifndef _TextTool_Leads_h_
#define _TextTool_Leads_h_


BEGIN_TEXTLIB_NAMESPACE


class TextTool;

class LeadsCtrl : public ToolEditorBase {
	
protected:
	friend class OwnerInfoCtrl;
	friend class ProfileInfoCtrl;
	Splitter			hsplit, menusplit;
	ArrayCtrl			componentlist;
	
	ImageGenTool				image_gen;
	OwnerInfoCtrl				owner_info;
	ProfileInfoCtrl				profile_info;
	LeadWebsites				lead_sites;
	LeadTemplateCtrl			lead_templates;
	SocialContent				some_content;
	LeadPublishers				lead_publisher;
	//SocialContentTemplateCtrl	tmpl_content;
	SocialHeaderCtrl			social_header;
	BiographyCtrl				biography;
	ImageBiographyCtrl			image_biography;
	AudienceCtrl				audience;
	BiographySummaryCtrl		biography_summary;
	ImageBiographySummaryCtrl	image_biography_summary;
	PlatformCtrl				platform;
	PlatformProfileCtrl			platform_profile;
	SocialNeedsCtrl				social_needs;
	SocialBeliefsCtrl			social_beliefs;
	
	
public:
	typedef LeadsCtrl CLASSNAME;
	LeadsCtrl(TextTool* app);
	
	void Init();
	void Data() override;
	void DataComponent();
	void InitSimplified();
	void SetSubMenu(int i) override;
	void OnDataProfile() override;
	
};



END_TEXTLIB_NAMESPACE


#endif
