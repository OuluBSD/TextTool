#ifndef _SongTool_App_h_
#define _SongTool_App_h_



class SongTool : public TopWindow {
	MenuBar			menu;
	FrontPage		fp;
	CalendarPage	cal;
	Editor			ed;
	SeqCtrl			seq;
	AI_Tasks		ai;
	TimeCallback	tc;
	
	int page = 0;
	Rect last_window;
	bool is_maximized = false;
	bool started = false;
	
	void Load() {LoadFromFile(*this, ConfigFile("cookie.bin"));}
	void Store() {StoreToFile(*this, ConfigFile("cookie.bin"));}
	
public:
	typedef SongTool CLASSNAME;
	SongTool();
	~SongTool();
	
	void MainMenu(Bar& b);
	void SetOpenAIToken();
	void Data();
	void SetView(int i);
	void Serialize(Stream& s) override {s % page % last_window % is_maximized % ed;}
	void Layout() override {SaveWindowPos();}
	void ShowOrphanedFiles();
	void MakeTasks();
	void SaveWindowPos();
	void LoadWindowPos();
};


#endif
