#include "TextCtrl.h"
#include <TextDataCtrl/TextDataCtrl.h>


BEGIN_TEXTLIB_NAMESPACE


void AppModeStartup(int appmode) {
	EnterAppMode(appmode);
	
	TextLib::TaskManager& tm = TextLib::TaskManager::Single(appmode);
	TextDatabase& db = GetAppModeDatabase(appmode);
	MetaDatabase& mdb = MetaDatabase::Single();
	TaskMgrConfig& m = TaskMgrConfig::Single();
	
	// Load Database
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
	
	if (db.src_data.IsEmpty()) {
		TextDataLoader loader(appmode);
		loader.Run();
	}
	
	tm.Start();
	
	DatabaseBrowser::Single(appmode).Load();
	
	LeaveAppMode();
}

void AppModeShutdown(int appmode, bool fast_exit, bool save_songdata) {
	EnterAppMode(appmode);
	
	TextLib::TaskManager& tm = TextLib::TaskManager::Single(appmode);
	TextDatabase& db = GetAppModeDatabase(appmode);
	TaskMgrConfig& m = TaskMgrConfig::Single();
	
	// Deinit storing of files
	tm.Stop();
	m.Stop();
	
	if (!fast_exit) {
		DatabaseBrowser::Single(appmode).Store();
		
		db.Store();
		
		if (save_songdata) {
			db.src_data.a.Store();
			db.src_data.a.StoreJson();
		}
		
		m.Store();
	}
	
	LeaveAppMode();
}



END_TEXTLIB_NAMESPACE
