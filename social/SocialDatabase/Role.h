#ifndef _SocialDatabase_Role_h_
#define _SocialDatabase_Role_h_


BEGIN_SOCIALLIB_NAMESPACE


struct Role : DataFile {
	Array<Generic>	generics;
	
	
	int GetStoryCount() const;
};


END_SOCIALLIB_NAMESPACE


#endif
