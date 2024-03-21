#include "AgentTest.h"

CONSOLE_APP_MAIN {
	SetDefaultCharset(CHARSET_UTF8);
	
	TextDatabase& db = GetDatabase();
	#ifdef flagWIN32
	db.dir = AppendFileName(GetHomeDirectory(), "TextTool");
	#else
	db.dir = GetHomeDirFile("TextTool");
	#endif
	if (!DirectoryExists(db.dir)) {
		PromptOK(DeQtf("Default path not found.\nSelect TextTool directory."));
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
