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
	ArrayCtrl			owners;
	ArrayCtrl			profiles;
	
	ImageGenTool		image_gen;
	OwnerInfoCtrl		owner_info;
	ProfileInfoCtrl		profile_info;
	LeadWebsites		lead_sites;
	SocialContent		some_content;
	
	
public:
	typedef LeadsCtrl CLASSNAME;
	LeadsCtrl(TextTool* app);
	
	void OwnerMenu(Bar& bar);
	void ProfileMenu(Bar& bar);
	void Init();
	void Data() override;
	void DataComponent();
	void DataProfile();
	void DataOwner();
	void InitSimplified();
	void SetSubMenu(int i) override;
	void AddOwner();
	void RemoveOwner();
	void AddProfile();
	void RemoveProfile();
	
};



END_TEXTLIB_NAMESPACE


#endif
