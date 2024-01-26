#include "SongTool.h"


GUI_APP_MAIN {
	SetDefaultCharset(CHARSET_UTF8);
	//SetLanguage(GetSystemLNG());
	
	/*WString ws = String("ˈsi.ʒər").ToWString();
	int first = ws[0];
	PhoneticNanaAnalyser anal;
	anal.Parse(ws.Begin(), ws.GetCount());
	byte nana[256];
	anal.WritePackedNana(nana, 256);*/
	
	ChFlatSkin();
	InstallRedBar();
	
	// Load task manager
	TaskMgrConfig& m = TaskMgrConfig::Single();
	m.CreateDefaultTaskRules();
	m.Load();
	m.Start();
	
	
	// Aquiring initial OpenAI token
	if (m.openai_token.IsEmpty()) {
		String token;
		bool b = EditTextNotNull(
			token,
			t_("OpenAI token"),
			t_("OpenAI token"),
			0
		);
		if (b) {
			m.openai_token = token;
			m.Store();
		}
	}
	//LOG("OpenAI token: " << m.openai_token);
	
	
	// Start OpenAI backend
	openai::OpenAI* instance = 0;
	if (!m.openai_token.IsEmpty())
		instance = &openai::start(m.openai_token.Begin());
	
	
	// For testing
	String proxy;
	String proxy_file = ConfigFile("proxy.bin");
	const Vector<String>& cmds = CommandLine();
	for(int i = 0; i < cmds.GetCount(); i++) {
		const String& cmd = cmds[i];
		
		if (cmd == "-test-openai") {
			auto completion = openai::completion().create(R"_json(
		    {
		        "model": "text-davinci-003",
		        "prompt": "Say this is a test",
		        "max_tokens": 7,
		        "temperature": 0
		    }
		    )_json");
		    LOG("Response is:\n" << completion.dump(2));
		    return;
		}
		else if (cmd == "-proxy" && i+1 < cmds.GetCount()) {
			proxy = cmds[i+1];
			i++;
		}
		else if (cmd == "-permanent-proxy" && i+1 < cmds.GetCount()) {
			proxy = cmds[i+1];
			FileOut fout(proxy_file);
			fout << proxy;
			i++;
		}
		else if (cmd == "-blue") {
			InstallBlueBar();
		}
	}
	
	if (FileExists(proxy_file))
		proxy = LoadFile(proxy_file);
	
	if (instance && proxy.GetCount())
		instance->setProxy(proxy.Begin());
	
	// Load Database
	Database& db = Database::Single();
	#ifdef flagWIN32
	db.dir = AppendFileName(GetHomeDirectory(), "SongTool");
	#else
	db.dir = GetHomeDirFile("SongTool");
	#endif
	if (!DirectoryExists(db.dir)) {
		PromptOK(DeQtf("Default path not found.\nSelect SongTool directory."));
		db.dir = SelectDirectory();
	}
	db.Load();
	
	db.song_data.Load();
	db.song_data.a.Load();
	
	if (db.song_data.IsEmpty()) {
		SongDataLoader loader;
		loader.Run();
	}
	
	SongLib::TaskManager& tm = SongLib::TaskManager::Single();
	tm.Start();
	
	DatabaseBrowser::Single().Load();
	
	// Run main program
	bool save_songdata = false;
	bool fast_exit = false;
	{
		SongTool t;
		
		/*bool advanced = false;
		for (const String& arg : CommandLine())
			if (arg == "-a")
				advanced = true;
		
		if (advanced)
			t.GetEditor().InitAdvanced();
		else*/
		t.GetEditor().InitSimplified();
		
		t.Run();
		
		save_songdata = t.GetEditor().GetSaveSongdata();
		fast_exit = t.GetEditor().IsFastExit();
	}
	
	Thread::ShutdownThreads();
	
	// Deinit storing of files
	tm.Stop();
	m.Stop();
	
	if (!fast_exit) {
		DatabaseBrowser::Single().Store();
		
		db.Store();
		
		if (save_songdata) {
			db.song_data.a.Store();
			db.song_data.a.StoreJson();
		}
		
		m.Store();
	}
	
	Thread::ShutdownThreads();
}
