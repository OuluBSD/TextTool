#include "TextTool.h"


BEGIN_TEXTLIB_NAMESPACE


LeadsCtrl::LeadsCtrl(TextTool* app) : ToolEditorBase("leads", *app) {
	Add(hsplit.SizePos());
	
	hsplit.Horz() << menusplit << base;
	hsplit.SetPos(1000);
	
	menusplit.Vert() << page_group_list << page_list << profile;
	
	profile.WhenBar << THISBACK(ProfileMenu);
	
	profile.AddColumn(t_("Profile"));
	profile <<= THISBACK(DataProfile);
	
	
}

void LeadsCtrl::InitSimplified() {
	AddItem(t_("Tools"), t_("AI Image Generator"), image_gen);
	AddItem(t_("Database"), t_("Lead Websites"), lead_sites);
	
}

void LeadsCtrl::Init() {
	UpdatePageList();
	
	ToolEditorBase::Init();
	
	
}

void LeadsCtrl::Data() {
	DataPage();
}

void LeadsCtrl::DataProfile() {
	
}

void LeadsCtrl::ProfileMenu(Bar& bar) {
	
}

void LeadsCtrl::SetSubMenu(int i) {
	
}


END_TEXTLIB_NAMESPACE
	