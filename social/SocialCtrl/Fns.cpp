#include "SocialCtrl.h"
#include <SocialDataCtrl/SocialDataCtrl.h>


BEGIN_SOCIALLIB_NAMESPACE


void SongStartup() {
	SocialLib::TaskManager& tm = SocialLib::TaskManager::Single();
	SocialDatabase& db = SocialDatabase::Single();
	TaskMgrConfig& m = TaskMgrConfig::Single();
	
	// Load SocialDatabase
	#ifdef flagWIN32
	db.dir = AppendFileName(GetHomeDirectory(), "SocialTool");
	#else
	db.dir = GetHomeDirFile("SocialTool");
	#endif
	if (!DirectoryExists(db.dir)) {
		PromptOK(DeQtf("Default path not found.\nSelect SocialTool directory."));
		db.dir = SelectDirectory();
	}
	db.Load();
	
	db.program_data.Load();
	db.program_data.a.Load();
	
	if (db.program_data.IsEmpty()) {
		SocialDataLoader loader;
		loader.Run();
	}
	
	tm.Start();
	
	//SocialDatabaseBrowser::Single().Load();
	
}

void SongShutdown(bool fast_exit, bool save_programdata) {
	SocialLib::TaskManager& tm = SocialLib::TaskManager::Single();
	SocialDatabase& db = SocialDatabase::Single();
	TaskMgrConfig& m = TaskMgrConfig::Single();
	
	// Deinit storing of files
	tm.Stop();
	m.Stop();
	
	if (!fast_exit) {
		//SocialDatabaseBrowser::Single().Store();
		
		db.Store();
		
		if (save_programdata) {
			db.program_data.a.Store();
			db.program_data.a.StoreJson();
		}
		
		m.Store();
	}
	
	
}


END_SOCIALLIB_NAMESPACE
