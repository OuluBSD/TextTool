#ifndef _SongTool_App_h_
#define _SongTool_App_h_


BEGIN_SONGLIB_NAMESPACE


class SongTool : public TopWindow {
	MenuBar				menu;
	ToolBar				toolbar;
	StatusBar			statusbar;
	
	SongEditor			ed;
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
	friend class SongEditor;
	
	String last_typecast, last_archetype, last_lyrics, last_part;
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
	void SetSaveSongdata(bool b) {ed.SetSaveSongdata(b);}
	void FastExit();
	void ClearSonglibTasks();
	
	SongEditor& GetEditor() {return ed;}
	
};


END_SONGLIB_NAMESPACE


#endif
