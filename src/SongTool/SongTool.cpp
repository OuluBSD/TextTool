#include "SongTool.h"

#define IMAGECLASS AppImg
#define IMAGEFILE <SongTool/App.iml>
#include <Draw/iml_source.h>


GUI_APP_MAIN {
	SetLanguage(GetSystemLNG());
	
	// Load Database
	/*Database& db = Database::Single();
	
	#ifdef flagWIN32
	db.dir = "C:\\git\\AudioAssets";
	#else
	db.dir = GetHomeDirFile("AudioAssets");
	#endif
	
	if (!DirectoryExists(db.dir)) {
		PromptOK(DeQtf("Default path not found.\nSelect AudioAssets directory."));
		db.dir = SelectDirectory();
	}
	
	db.Load();
	
	
	
	ToolApp().Run();*/
	TopWindow().Run();
}
