#include "TextTool.h"


BEGIN_TEXTLIB_NAMESPACE


LeadsCtrl::LeadsCtrl(TextTool* app) : ToolEditorBase("leads", *app) {
	MetaPtrs::Single().leads = this;
	
	Add(hsplit.SizePos());
	
	hsplit.Horz() << menusplit << base;
	hsplit.SetPos(1000);
	
	menusplit.Vert() << page_group_list << page_list << owners << profiles << snapshots;
	
}

void LeadsCtrl::InitSimplified() {
	AddItem(t_("Tools"), t_("AI Image Generator"), image_gen);
	AddItem(t_("Tools"), t_("AI Image aspect ratio fixer"), aspect_fixer);
	AddItem(t_("Database"), t_("Lead Websites"), lead_sites);
	AddItem(t_("Database"), t_("Lead Templates"), lead_templates);
	AddItem(t_("Database"), t_("Lead Publishers"), lead_publisher);
	AddItem(t_("Profile"), t_("Owner Info"), owner_info);
	AddItem(t_("Profile"), t_("Profile Info"), profile_info);
	AddItem(t_("Social Media"), t_("Beliefs"), social_beliefs);
	AddItem(t_("Social Media"), t_("Biography Generator"), biography_gen);
	AddItem(t_("Social Media"), t_("Biography"), biography);
	AddItem(t_("Social Media"), t_("Biography Elements"), biography_elements);
	AddItem(t_("Social Media"), t_("Biography Concepts"), conceptual);
	AddItem(t_("Social Media"), t_("Biography Summary"), biography_summary);
	AddItem(t_("Social Media"), t_("Image Biography"), image_biography);
	AddItem(t_("Social Media"), t_("Image Biography Summary"), image_biography_summary);
	AddItem(t_("Social Media"), t_("Platform"), platform);
	AddItem(t_("Social Media"), t_("Platform Profile"), platform_profile);
	AddItem(t_("Social Media"), t_("Audience"), audience);
	AddItem(t_("Social Media"), t_("Header"), social_header);
	AddItem(t_("Social Media"), t_("Content"), some_content);
	AddItem(t_("Social Media"), t_("Needs"), social_needs);
	AddItem(t_("Social Media"), t_("Marketplace"), marketplace);
	
	//AddItem(t_("Social Media"), t_("Content Templates"), tmpl_content);
	
}

void LeadsCtrl::Init() {
	ToolEditorBase::Init(); // must be first
	UpdatePageList();
	
	
	
}

void LeadsCtrl::Data() {
	DataMeta();
}

void LeadsCtrl::OnDataSnapshot() {
	DataPage();
}

void LeadsCtrl::SetSubMenu(int i) {
	
}


END_TEXTLIB_NAMESPACE
	