#ifndef _SocialCtrl_Editor_h_
#define _SocialCtrl_Editor_h_

class ToolAppCtrl;


BEGIN_SOCIALLIB_NAMESPACE


class SocialTool;


class SocialEditor : public Ctrl {
	
private:
	struct ListItem : Moveable<ListItem> {
		String item;
		ToolAppCtrl* ctrl = 0;
	};
	VectorMap<String, Vector<ListItem>> items;
	
	void AddItem(String g, String i, ToolAppCtrl& c);
	void InitListItems();
	
protected:
	friend class SocialTool;
	friend class ProgramInfoCtrl;
	friend class CompanyInfoCtrl;
	friend class CampaignInfoCtrl;
	friend class StoryInfoCtrl;
	
	Ctrl				subsplit;
	Splitter			hsplit, menusplit, programsplit, storiessplit;
	ArrayCtrl			page_group_list, page_list, companys;
	ArrayCtrl			companies, campaigns, programs;
	ArrayCtrl			roles, generics, stories;
	Ctrl				base;
	int					page_group = 0;
	VectorMap<int,int>	page;
	bool				save_programdata = false;
	bool				fast_exit = false;
	
	
	ImageGenTool					image_gen;
	
public:
	typedef SocialEditor CLASSNAME;
	SocialEditor(SocialTool* app);
	
	void Serialize(Stream& s) {s % page_group % page % save_programdata;}
	void Init();
	void ViewPageGroup();
	void ViewPage();
	void Data();
	void DataPage();
	void DataCompany();
	void DataCampaign();
	void DataProgram();
	void DataRole();
	void DataGeneric();
	void DataStory();
	void ToolMenu(Bar& bar);
	void SetView(int i, int j);
	void SetSubMenu(int i);
	void LoadLast();
	void StoreLast();
	void MovePageGroup(int d);
	void MovePage(int d);
	String GetStatusText();
	bool GetSaveProgramData() const {return save_programdata;}
	bool IsFastExit() const {return fast_exit;}
	void SetSaveProgramData(bool b) {save_programdata = b;}
	void SetFastExit(bool b) {fast_exit = b;}
	
	void CompanyMenu(Bar& bar);
	void CampaignMenu(Bar& bar);
	void ProgramMenu(Bar& bar);
	void StoryMenu(Bar& bar);
	
	void AddCompany();
	void RenameCompany();
	void RemoveCompany();
	void AddCampaign();
	void RenameCampaign();
	void RemoveCampaign();
	void AddProgram();
	void RenameProgram();
	void RemoveProgram();
	void AddStory();
	void RemoveStory();
	
	void InitSimplified();
	
	Callback WhenStartUpdating, WhenStopUpdating;
	SocialTool& app;
	
};


END_SOCIALLIB_NAMESPACE


#endif
