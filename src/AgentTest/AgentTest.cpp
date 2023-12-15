#include "AgentTest.h"

CONSOLE_APP_MAIN {
	SetDefaultCharset(CHARSET_UTF8);
	
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
	
	LOG("Database loaded");
	
	String txt = "nu- nu- nu- nu- nuu";
	
	RhymeContainer rc;
	MockupPhraseParser parser;
	if (!parser.Parse(txt) || !parser.Process(rc)) {
		LOG("error: " + parser.GetError());
		return;
	}
	
	rc.Dump();
	
	SongAgent::Solver solver;
	if (!solver.Load(rc)) {
		LOG("error: could not load rhyme container");
		return;
	}
	
	solver.Process();
}
