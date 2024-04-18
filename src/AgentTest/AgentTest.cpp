#include "AgentTest.h"

CONSOLE_APP_MAIN {
	SetDefaultCharset(CHARSET_UTF8);
	
	TextDatabase& db = GetDatabase();
	MetaDatabase& mdb = MetaDatabase::Single();
	#ifdef flagWIN32
	mdb.dir = AppendFileName(GetHomeDirectory(), "TextTool");
	#else
	mdb.dir = GetHomeDirFile("TextTool");
	#endif
	if (!DirectoryExists(mdb.dir)) {
		PromptOK(DeQtf("Default path not found.\nSelect TextTool directory."));
		mdb.dir = SelectDirectory();
	}
	db.Load();
	
	db.src_data.Load();
	db.src_data.a.Load();
	
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
