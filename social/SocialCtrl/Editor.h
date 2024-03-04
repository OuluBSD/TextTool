#ifndef _SocialCtrl_Editor_h_
#define _SocialCtrl_Editor_h_


class SocialTool;
class ToolAppCtrl;

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
	
	Ctrl				subsplit;
	Splitter			hsplit, menusplit, programsplit, storiessplit;
	ArrayCtrl			page_group_list, page_list, companys;
	ArrayCtrl			companies, campaigns, programs;
	ArrayCtrl			roles, generics, stories;
	Ctrl				base;
	int					page_group = 0;
	VectorMap<int,int>	page;
	bool				save_storydata = false;
	bool				fast_exit = false;
	
	
	ImageGenTool					image_gen;
	
public:
	typedef SocialEditor CLASSNAME;
	SocialEditor(SocialTool* app);
	
	void Serialize(Stream& s) {s % page_group % page % save_storydata;}
	void Init();
	void ViewPageGroup();
	void ViewPage();
	void Data();
	void DataPage();
	void DataCompany();
	void DataCampaing();
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
	bool GetSaveSocialdata() const {return save_storydata;}
	bool IsFastExit() const {return fast_exit;}
	void SetSaveSocialdata(bool b) {save_storydata = b;}
	void SetFastExit(bool b) {fast_exit = b;}
	
	void CompanyMenu(Bar& bar);
	void CampaingMenu(Bar& bar);
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


#endif
