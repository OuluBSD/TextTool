#include "TextDatabase.h"


BEGIN_TEXTLIB_NAMESPACE



int& __global_appmode();


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

const char* AppModeLabels[DB_COUNT][AML_COUNT] = {
	{
		"Singing style:",
		"Acoustic instruments",
		"Digital instruments",
		"Singer",
		"Vibe of voice",
		"Album",
		"Artist (if different):",
		"Reference song:",
		"Origins of the song:",
		"Active song structure",
		"Song BPM",
		"Not a rap song",
		"This is a rap song",
		
		"acoustic_instruments",
		"electronic_instruments",
		"vocalist_visual",
		"vibe_of_voice",
		"musical_style",
	},
	{
		"Talking style:",
		"",
		"",
		"Speaker",
		"Vibe of text",
		"Campaign",
		"Company (if different):",
		"Reference program:",
		"Origins of the program:",
		"Active program structure",
		"Speed",
		"Safe",
		"Unsafe",
		
		"natural_features",
		"electronic_features",
		"person_visual",
		"vibe_of_text",
		"text_style",
	},
};

String GetAppModeLabel(int amlkey) {
	int appmode = __global_appmode() - 1;
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	ASSERT(amlkey >= 0 && amlkey < AML_COUNT);
	return AppModeLabels[appmode][amlkey];
}

String GetAppModeKeyEntities() {
	int appmode = __global_appmode() - 1;
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	return AppModeKeysN[appmode][0];
}

String GetAppModeKeySnapshots() {
	int appmode = __global_appmode() - 1;
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	return AppModeKeysN[appmode][1];
}

String GetAppModeKeyComponents() {
	int appmode = __global_appmode() - 1;
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	return AppModeKeysN[appmode][2];
}

String GetAppModeKey(int appmode, int amkey) {
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	return AppModeKeys[appmode][amkey];
}

String GetAppModeKey(int amkey) {
	int appmode = __global_appmode() - 1;
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	return AppModeKeys[appmode][amkey];
}

String GetAppModeKeyN(int appmode, int amkey) {
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	return AppModeKeysN[appmode][amkey];
}

String GetAppModeKeyN(int amkey) {
	int appmode = __global_appmode() - 1;
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	return AppModeKeysN[appmode][amkey];
}

String GetAppModeKeyCap(int amkey) {
	int appmode = __global_appmode() - 1;
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	return Capitalize(AppModeKeys[appmode][amkey]);
}

String GetAppModeKeyCapN(int amkey) {
	int appmode = __global_appmode() - 1;
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	return Capitalize(AppModeKeysN[appmode][amkey]);
}



END_TEXTLIB_NAMESPACE
