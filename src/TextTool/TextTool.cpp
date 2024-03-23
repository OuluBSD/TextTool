#include "TextTool.h"


GUI_APP_MAIN {
	using namespace TextLib;
	
	bool have_all = true;
	bool have_song = 0, have_social = 0;
	
	for (const String& s : CommandLine()) {
		if (s == "-social") {have_all = false; have_social = true;}
		if (s == "-song") {have_all = false; have_song = true;}
	}
	
	GuiStartup();
	if (have_all || have_song)		SongStartup();
	if (have_all || have_social)	SocialStartup();
	
	
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
	
	if (have_all || have_social)	SocialShutdown(fast_exit, save_songdata);
	if (have_all || have_song)		SongShutdown(fast_exit, save_songdata);
}
