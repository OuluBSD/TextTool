#ifndef _TextTool_Leads_h_
#define _TextTool_Leads_h_


BEGIN_TEXTLIB_NAMESPACE


class TextTool;

class LeadsCtrl : public ToolEditorBase {
	
private:
	Splitter			hsplit, menusplit;
	ArrayCtrl			componentlist, profile;
	
	
	ImageGenTool					image_gen;
	LeadWebsites					lead_sites;
	
	
public:
	typedef LeadsCtrl CLASSNAME;
	LeadsCtrl(TextTool* app);
	
	void ProfileMenu(Bar& bar);
	void Init();
	void Data() override;
	void DataComponent();
	void DataProfile();
	void InitSimplified();
	void SetSubMenu(int i) override;
	
};



END_TEXTLIB_NAMESPACE


#endif
