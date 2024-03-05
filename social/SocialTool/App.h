#ifndef _SocialTool_App_h_
#define _SocialTool_App_h_


BEGIN_SOCIALLIB_NAMESPACE


class SocialEditor;


class SocialTool : public TopWindow {
	
protected:
	friend class SocialEditor;
	
	MenuBar				menu;
	ToolBar				toolbar;
	StatusBar			statusbar;
	
	FrontPage			fp;
	CalendarPage		cal;
	SocialEditor		ed;
	Tasks				ai;
	TimeCallback		tc;
	bool				skip_data = false;
	
	int page = 0;
	Rect last_window;
	bool is_maximized = false;
	bool started = false;
	
	void Load() {LoadFromFile(*this, ConfigFile("cookie.bin"));}
	void Store() {StoreToFile(*this, ConfigFile("cookie.bin"));}
	
protected:
	friend class SocialEditor;
	
	String last_role, last_generic, last_story, last_story_part;
	String last_company, last_campaign, last_program;
	
public:
	typedef SocialTool CLASSNAME;
	SocialTool();
	~SocialTool();
	
	void MainMenu(Bar& b);
	void SetOpenAIToken();
	void Init();
	void PostInit() {PostCallback(THISBACK(Init));}
	void Data();
	void SetView(int i);
	void Serialize(Stream& s) override {s % page % last_window % is_maximized % ed % last_company % last_campaign % last_program % last_role % last_generic % last_story % last_story_part;}
	void Layout() override {SaveWindowPos();}
	void ShowOrphanedFiles();
	//void MakeTasks();
	void SaveWindowPos();
	void LoadWindowPos();
	void StartUpdating();
	void MovePageGroup(int d);
	void MovePage(int d);
	void SetBar();
	void MainBar(Bar& bar);
	void SetSaveProgramData(bool b) {ed.SetSaveProgramData(b);}
	void FastExit();
	void ClearSocialLibTasks();
	
	SocialEditor& GetEditor() {return ed;}
	
};


END_SOCIALLIB_NAMESPACE

#endif
