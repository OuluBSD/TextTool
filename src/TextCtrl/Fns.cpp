#include "TextCtrl.h"
#include <TextDataCtrl/TextDataCtrl.h>


BEGIN_TEXTLIB_NAMESPACE


void SongStartup() {
	TextLib::TaskManager& tm = TextLib::TaskManager::Single();
	TextDatabase& db = TextDatabase::Single();
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
	
	db.song_data.Load();
	db.song_data.a.Load();
	
	if (db.song_data.IsEmpty()) {
		TextDataLoader loader;
		loader.Run();
	}
	
	tm.Start();
	
	DatabaseBrowser::Single().Load();
	
}

void SongShutdown(bool fast_exit, bool save_songdata) {
	TextLib::TaskManager& tm = TextLib::TaskManager::Single();
	TextDatabase& db = TextDatabase::Single();
	TaskMgrConfig& m = TaskMgrConfig::Single();
	
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
	
	
}


END_TEXTLIB_NAMESPACE
