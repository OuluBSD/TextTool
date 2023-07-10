#include "SongTool.h"

#define IMAGECLASS AppImg
#define IMAGEFILE <SongTool/App.iml>
#include <Draw/iml_source.h>


GUI_APP_MAIN {
	SetLanguage(GetSystemLNG());
	
	// Load Database
	Database& db = Database::Single();
	#ifdef flagWIN32
	db.dir = "C:\\git\\SongTool";
	#else
	db.dir = GetHomeDirFile("SongTool");
	#endif
	
	if (!DirectoryExists(db.dir)) {
		PromptOK(DeQtf("Default path not found.\nSelect SongTool directory."));
		db.dir = SelectDirectory();
	}
	db.Load();
	SongTool().Run();
	db.Store();
	
	TaskMgr::Single().Store();
}
