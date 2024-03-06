#include "SocialTool.h"

#define IMAGECLASS SocialToolImg
#define IMAGEFILE <SocialTool/SocialTool.iml>
#include <Draw/iml_source.h>


GUI_APP_MAIN {
	using namespace SocialLib;
	
	GuiStartup();
	SocialStartup();
	
	
	// Run main program
	bool save_socialdata = false;
	bool fast_exit = false;
	{
		SocialTool t;
		
		t.GetEditor().InitSimplified();
		
		t.Run();
		
		save_socialdata = t.GetEditor().GetSaveSocialData();
		fast_exit = t.GetEditor().IsFastExit();
	}
	
	Thread::ShutdownThreads();
	
	SocialShutdown(fast_exit, save_socialdata);
}
