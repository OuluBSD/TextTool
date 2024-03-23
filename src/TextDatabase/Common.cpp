#include "TextDatabase.h"


BEGIN_TEXTLIB_NAMESPACE



extern int __global_appmode;


const char* AppModeKeys[DB_COUNT][AM_COUNT+1] = {
	//{"entity","release","component","typeclass","content","scripts",0},
	{"artist","release","song","typecast","archetype","lyrics","db","rapper",0},
	{"company","campaign","program","role","generic","story","social_db","unsafe",0},
	{"","","","","","",""},
	{"","","","","","",""},
	{"","","","","","",""},
};

const char* AppModeKeysN[DB_COUNT][AM_COUNT+1] = {
	{"artists","releases","songs","typecasts","archetypes","lyrics",0,0,0},
	{"companies","campaigns","programs","roles","generics","stories",0,0,0},
	{"","","","","","",""},
	{"","","","","","",""},
	{"","","","","","",""},
};

String GetAppModeKeyEntities() {
	int appmode = __global_appmode - 1;
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	return AppModeKeysN[appmode][0];
}

String GetAppModeKeySnapshots() {
	int appmode = __global_appmode - 1;
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	return AppModeKeysN[appmode][1];
}

String GetAppModeKeyComponents() {
	int appmode = __global_appmode - 1;
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	return AppModeKeysN[appmode][2];
}

String GetAppModeKey(int amkey) {
	int appmode = __global_appmode - 1;
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	return AppModeKeys[appmode][amkey];
}

String GetAppModeKeyN(int amkey) {
	int appmode = __global_appmode - 1;
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	return AppModeKeysN[appmode][amkey];
}

String GetAppModeKeyCap(int amkey) {
	int appmode = __global_appmode - 1;
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	return Capitalize(AppModeKeys[appmode][amkey]);
}

String GetAppModeKeyCapN(int amkey) {
	int appmode = __global_appmode - 1;
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	return Capitalize(AppModeKeysN[appmode][amkey]);
}



END_TEXTLIB_NAMESPACE
