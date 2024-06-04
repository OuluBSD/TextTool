#include "TextCtrl.h"
#include <TextDataCtrl/TextDataCtrl.h>


BEGIN_TEXTLIB_NAMESPACE


void MetaStartup(bool my_data) {
	MetaDatabase& mdb = MetaDatabase::Single();
	
	if (my_data) {
		// Load Database
		#ifdef flagWIN32
		mdb.dir = AppendFileName(GetHomeDirectory(), "MyTextTool");
		#else
		mdb.dir = GetHomeDirFile("MyTextTool");
		#endif
	}
	else {
		String dir;
		#ifdef flagDEBUG
		#ifdef flagWIN32
		dir = AppendFileName(GetHomeDirectory(), "TextTool");
		#else
		dir = GetHomeDirFile("TextTool");
		#endif
		#endif
		
		if (dir.IsEmpty() || !DirectoryExists(dir))
			dir = ConfigFile("");
		
		mdb.dir = dir;
	}
	
	if (!DirectoryExists(mdb.dir)) {
		PromptOK(DeQtf("Default path not found.\nSelect MyTextTool directory."));
		mdb.dir = SelectDirectory();
	}
	
	mdb.Load();
	//Owner::CopyOld();
	
	LeadDataTemplate::Single().Load();
}

void MetaShutdown() {
	ProfileData::StoreAll();
	MetaDatabase::Single().Store();
	LeadDataTemplate::Single().Store();
}

void AppModeStartup(int appmode) {
	MetaDatabase& mdb = MetaDatabase::Single();
	EnterAppMode(appmode);
	
	TextLib::TaskManager& tm = TextLib::TaskManager::Single(appmode);
	TextDatabase& db = GetAppModeDatabase(appmode);
	TaskMgrConfig& m = TaskMgrConfig::Single();
	
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
