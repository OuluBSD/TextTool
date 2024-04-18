#include "TextTool.h"

#ifdef flagMAIN

GUI_APP_MAIN {
	using namespace TextLib;
	
	#ifdef flagDEBUG
	TestDbVars();
	
	#endif
	
	
	Index<int> appmodes;
	for (const String& s : CommandLine()) {
		if (s == "-song")			{appmodes << DB_SONG;}
		if (s == "-twitter")		{appmodes << DB_TWITTER;}
		if (s == "-blog")			{appmodes << DB_BLOG;}
		if (s == "-dialog")			{appmodes << DB_DIALOG;}
		if (s == "-storyboard")		{appmodes << DB_STORYBOARD;}
	}
	
	GuiStartup();
	
	if (appmodes.IsEmpty())
		for(int i = 0; i < DB_COUNT; i++)
			appmodes.Add(i);
	
	for (int appmode : appmodes.GetKeys()) {
		AppModeStartup(appmode);
	};
	
	
	// Run main program
	bool save_songdata = false;
	bool fast_exit = false;
	{
		TextTool t;
		
		t.GetEditor().InitAppModes(appmodes);
		t.GetEditor().InitSimplified();
		t.GetLeads().InitSimplified();
		
		t.Run();
		
		save_songdata = t.GetEditor().GetSaveSongdata();
		fast_exit = t.GetEditor().IsFastExit();
	}
	
	Thread::ShutdownThreads();
	
	for (int appmode : appmodes.GetKeys()) {
		AppModeShutdown(appmode, fast_exit, save_songdata);
	};
}

#endif
