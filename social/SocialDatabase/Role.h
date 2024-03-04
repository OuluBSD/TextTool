#ifndef _SocialDatabase_Role_h_
#define _SocialDatabase_Role_h_


struct Role : DataFile {
	Array<Generic>	generics;
	
	
	int GetStoryCount() const;
};


#endif
