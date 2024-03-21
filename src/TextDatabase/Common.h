#ifndef _TextDatabase_Common_h_
#define _TextDatabase_Common_h_

BEGIN_TEXTLIB_NAMESPACE


String GetAppModeKeyEntities();
String GetAppModeKeySnapshots();
String GetAppModeKeyComponents();

enum {
	AM_ENTITY,
	AM_RELEASE,
	AM_COMPONENT,
	AM_TYPECLASS,
	AM_CONTENT,
	AM_SCRIPT,
	
	AM_COUNT,
};
	
extern const char* AppModeKeys[DB_COUNT][AM_COUNT+1];
extern const char* AppModeKeysN[DB_COUNT][AM_COUNT+1];

String GetAppModeKey(int amkey);
String GetAppModeKeyN(int amkey);


END_TEXTLIB_NAMESPACE

#endif
