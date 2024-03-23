#include "TextCtrl.h"
#include <TextDataCtrl/TextDataCtrl.h>


BEGIN_TEXTLIB_NAMESPACE


void SongStartup() {
	EnterAppMode(DB_SONG);
	
	TextLib::TaskManager& tm = TextLib::TaskManager::Single(DB_SONG);
	TextDatabase& db = GetAppModeDatabase(DB_SONG);
	TaskMgrConfig& m = TaskMgrConfig::Single();
	
	// Load Database
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
	
	db.src_data.Load();
	db.src_data.a.Load();
	
	if (db.src_data.IsEmpty()) {
		TextDataLoader loader(DB_SONG);
		loader.Run();
	}
	
	tm.Start();
	
	DatabaseBrowser::Single().Load();
	
	LeaveAppMode();
}

void SongShutdown(bool fast_exit, bool save_songdata) {
	EnterAppMode(DB_SONG);
	
	TextLib::TaskManager& tm = TextLib::TaskManager::Single(DB_SONG);
	TextDatabase& db = GetAppModeDatabase(DB_SONG);
	TaskMgrConfig& m = TaskMgrConfig::Single();
	
	// Deinit storing of files
	tm.Stop();
	m.Stop();
	
	if (!fast_exit) {
		DatabaseBrowser::Single().Store();
		
		db.Store();
		
		if (save_songdata) {
			db.src_data.a.Store();
			db.src_data.a.StoreJson();
		}
		
		m.Store();
	}
	
	LeaveAppMode();
}


void SocialStartup() {
	EnterAppMode(DB_SOCIAL);
	
	TaskManager& tm = TaskManager::Single(DB_SOCIAL);
	TextDatabase& db = GetAppModeDatabase(DB_SOCIAL);
	TaskMgrConfig& m = TaskMgrConfig::Single();
	
	// Load SocialDatabase
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
	
	db.src_data.Load();
	db.src_data.a.Load();
	
	if (db.src_data.IsEmpty()) {
		TextDataLoader loader(DB_SOCIAL);
		loader.Run();
	}
	
	tm.Start();
	
	DatabaseBrowser::Single().Load();
	
	LeaveAppMode();
}

void SocialShutdown(bool fast_exit, bool save_socialdata) {
	EnterAppMode(DB_SOCIAL);
	
	TaskManager& tm = TaskManager::Single(DB_SOCIAL);
	TextDatabase& db = GetAppModeDatabase(DB_SOCIAL);
	TaskMgrConfig& m = TaskMgrConfig::Single();
	
	// Deinit storing of files
	tm.Stop();
	m.Stop();
	
	if (!fast_exit) {
		DatabaseBrowser::Single().Store();
		
		db.Store();
		
		if (save_socialdata) {
			db.src_data.a.Store();
			db.src_data.a.StoreJson();
		}
		
		m.Store();
	}
	
	LeaveAppMode();
}


END_TEXTLIB_NAMESPACE
