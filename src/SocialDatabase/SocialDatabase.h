#ifndef _SocialDatabase_SocialDatabase_h_
#define _SocialDatabase_SocialDatabase_h_

#include <ToolBase/ToolBase.h>


struct SocialDatabase {
	
	
	
	static SocialDatabase& Single() {static SocialDatabase db; return db;}
	
};


#endif
