#include "SongTool.h"


GUI_APP_MAIN {
	GuiStartup();
	SongStartup();
	
	
	// Run main program
	bool save_songdata = false;
	bool fast_exit = false;
	{
		SongTool t;
		
		t.GetEditor().InitSimplified();
		
		t.Run();
		
		save_songdata = t.GetEditor().GetSaveSongdata();
		fast_exit = t.GetEditor().IsFastExit();
	}
	
	Thread::ShutdownThreads();
	
	SongShutdown(fast_exit, save_songdata);
}
