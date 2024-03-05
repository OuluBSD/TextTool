#include "SocialTool.h"

#define IMAGECLASS SocialToolImg
#define IMAGEFILE <SocialTool/SocialTool.iml>
#include <Draw/iml_source.h>


GUI_APP_MAIN {
	using namespace SocialLib;
	
	GuiStartup();
	
	{
		SocialTool app;
		app.Run();
	}
	
	
}
