#include "TextTool.h"


GUI_APP_MAIN {
	using namespace TextLib;
	
	GuiStartup();
	SongStartup();
	//SocialStartup();
	
	
	// Run main program
	bool save_songdata = false;
	bool fast_exit = false;
	{
		TextTool t;
		
		t.GetEditor().InitSimplified();
		
		t.Run();
		
		save_songdata = t.GetEditor().GetSaveSongdata();
		fast_exit = t.GetEditor().IsFastExit();
	}
	
	Thread::ShutdownThreads();
	
	//SocialShutdown(fast_exit, save_songdata);
	SongShutdown(fast_exit, save_songdata);
}
