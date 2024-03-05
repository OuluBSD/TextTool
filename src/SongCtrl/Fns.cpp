#include "SongCtrl.h"
#include <SongDataCtrl/SongDataCtrl.h>


BEGIN_SONGLIB_NAMESPACE


void SongStartup() {
	SongLib::TaskManager& tm = SongLib::TaskManager::Single();
	SongDatabase& db = SongDatabase::Single();
	TaskMgrConfig& m = TaskMgrConfig::Single();
	
	// Load Database
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
	
	tm.Start();
	
	DatabaseBrowser::Single().Load();
	
}

void SongShutdown(bool fast_exit, bool save_songdata) {
	SongLib::TaskManager& tm = SongLib::TaskManager::Single();
	SongDatabase& db = SongDatabase::Single();
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


END_SONGLIB_NAMESPACE
