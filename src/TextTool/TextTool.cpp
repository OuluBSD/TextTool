#include "TextTool.h"

#ifdef flagMAIN

GUI_APP_MAIN {
	using namespace TextLib;
	
	#ifdef flagDEBUG
	TestDbVars();
	
	#endif
	
	String sharedir = "share";
	
	Index<int> appmodes;
	bool my_data = false;
	for (const String& s : CommandLine()) {
		if (s == "-my")				{my_data = true;}
		if (s == "-finshare")		{sharedir += "-fi";}
		if (s == "-song")			{appmodes << DB_SONG;}
		if (s == "-twitter")		{appmodes << DB_TWITTER;}
		if (s == "-blog")			{appmodes << DB_BLOG;}
		if (s == "-dialog")			{appmodes << DB_DIALOG;}
		if (s == "-storyboard")		{appmodes << DB_STORYBOARD;}
		if (s == "-code")			{appmodes << DB_CODE;}
		//if (s == "-marketing")		{appmodes << DB_MARKETING;}
	}
	
	MetaDatabase::Single().share = sharedir;
	
	GuiStartup();
	MetaStartup(my_data);
	
	if (appmodes.IsEmpty())
		for(int i = 0; i < DB_COUNT; i++)
			appmodes.Add(i);
	
	for (int appmode : appmodes.GetKeys()) {
		AppModeStartup(appmode);
	};
	
	
	// Run main program
	bool save_data = false;
	bool fast_exit = false;
	{
		TextTool t;
		
		t.GetOrganization().InitAppModes(appmodes);
		t.GetEditor().InitAppModes(appmodes);
		t.GetEditor().InitSimplified();
		t.GetLeads().InitSimplified();
		
		t.Run();
		
		save_data = t.GetEditor().GetSaveSongdata();
		fast_exit = t.GetEditor().IsFastExit();
	}
	
	Thread::ShutdownThreads();
	
	if (!fast_exit)
		MetaShutdown();
	
	for (int appmode : appmodes.GetKeys()) {
		AppModeShutdown(appmode, fast_exit, save_data);
	};
}

#endif
