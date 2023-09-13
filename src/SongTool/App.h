#ifndef _SongTool_App_h_
#define _SongTool_App_h_



class SongTool : public TopWindow {
	typedef enum {
		MODE_INVALID,
		MODE_SIMPLIFIED,
		MODE_ADVANCED,
	} AppMode;
	
	MenuBar				menu;
	FrontPage			fp;
	CalendarPage		cal;
	Editor				ed;
	SeqCtrl				seq;
	Tasks				ai;
	TimeCallback		tc;
	AppMode				mode = MODE_INVALID;
	
	int page = 0;
	Rect last_window;
	bool is_maximized = false;
	bool started = false;
	
	void Load() {LoadFromFile(*this, ConfigFile("cookie.bin"));}
	void Store() {StoreToFile(*this, ConfigFile("cookie.bin"));}
	
protected:
	friend class Editor;
	
	String last_artist, last_release, last_song;
	
public:
	typedef SongTool CLASSNAME;
	SongTool();
	~SongTool();
	
	void MainMenu(Bar& b);
	void SetOpenAIToken();
	void Init();
	void PostInit() {PostCallback(THISBACK(Init));}
	void Data();
	void SetView(int i);
	void Serialize(Stream& s) override {s % page % last_window % is_maximized % ed % last_artist % last_release % last_song;}
	void Layout() override {SaveWindowPos();}
	void ShowOrphanedFiles();
	void MakeTasks();
	void SaveWindowPos();
	void LoadWindowPos();
	void StartUpdating();
	void DoMainAction(int action);
	
	Editor& GetEditor() {return ed;}
	
};


#endif
