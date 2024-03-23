#include "TextTool.h"


GUI_APP_MAIN {
	using namespace TextLib;
	
	Index<int> appmodes;
	for (const String& s : CommandLine()) {
		if (s == "-song") {appmodes << DB_SONG;}
		if (s == "-social") {appmodes << DB_SOCIAL;}
		if (s == "-dialog") {appmodes << DB_DIALOG;}
	}
	
	GuiStartup();
	
	if (appmodes.IsEmpty())
		for(int i = 0; i < DB_COUNT; i++)
			appmodes.Add(i);
	
	for (int appmode : appmodes.GetKeys()) {
		switch (appmode) {
			case DB_SONG: SongStartup(); break;
			case DB_SOCIAL: SocialStartup(); break;
			default: break;
		}
	};
	
	
	// Run main program
	bool save_songdata = false;
	bool fast_exit = false;
	{
		TextTool t;
		
		t.GetEditor().InitAppModes(appmodes);
		t.GetEditor().InitSimplified();
		
		t.Run();
		
		save_songdata = t.GetEditor().GetSaveSongdata();
		fast_exit = t.GetEditor().IsFastExit();
	}
	
	Thread::ShutdownThreads();
	
	for (int appmode : appmodes.GetKeys()) {
		switch (appmode) {
			case DB_SONG: SongShutdown(fast_exit, save_songdata); break;
			case DB_SOCIAL: SocialShutdown(fast_exit, save_songdata); break;
			default: break;
		}
	};
}
