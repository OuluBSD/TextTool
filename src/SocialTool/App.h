#ifndef _SocialTool_App_h_
#define _SocialTool_App_h_

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
	friend class Editor;
	
	String last_typecast, last_archetype, last_lyrics, last_part;
	String last_artist, last_release, last_song;
	
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
	void Serialize(Stream& s) override {s % page % last_window % is_maximized % ed % last_artist % last_release % last_song % last_typecast % last_archetype % last_lyrics % last_part;}
	void Layout() override {SaveWindowPos();}
	void ShowOrphanedFiles();
	//void MakeTasks();
	void SaveWindowPos();
	void LoadWindowPos();
	void StartUpdating();
	void MovePageGroup(int d);
	void MovePage(int d);
	void MovePart(int d);
	void SetBar();
	void MainBar(Bar& bar);
	void SetSaveSocialdata(bool b) {ed.SetSaveSocialdata(b);}
	void FastExit();
	void ClearSociallibTasks();
	
	SocialEditor& GetEditor() {return ed;}
	
};



#endif
