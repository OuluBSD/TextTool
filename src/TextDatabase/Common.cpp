#include "TextDatabase.h"


BEGIN_TEXTLIB_NAMESPACE



int& __global_appmode();


const char* AppModeKeys[DB_COUNT][AM_COUNT+1] = {
	
	// DB_SONG
	{
		"artist",		"release",		"song",
		"typecast",		"archetype",	"lyrics",
		"db",			"rapper",0
	},
	
	// DB_TWITTER
	{
		"company",		"campaign",		"program",
		"role",			"generic",		"story",
		"social_db",	"unsafe",0
	},
	
	// DB_BLOG
	{
		"blogger",		"series",		"post",
		"topic",		"theme",		"content",
		"blog_db",		"provocative",0
	},
	
	// DB_DIALOG
	{
		"presenter",	"scriptbook",	"conversation",
		"character",	"trope",		"lines",
		"dialog_db",	"controversial",0
	},
	
	// DB_POLITICS
	{
		"politician",	"discourse",	"debate",
		"belief",		"issue",		"discussion",
		"politics_db",	"heated",0
	},
	
	// DB_LIFE
	{
		"human",		"collection",	"experience",
		"persona",		"struggle",		"lesson",
		"life_db",		"contentious",0
	},
	
	// DB_BUSINESS
	{
		"entrepreneur",	"forum",		"meeting",
		"strategy",		"topic",		"talking",
		"business_db",	"contentious",0
	},
	
	// DB_FLIRTING
	{
		"flirt",		"ritual",		"folly",
		"technique",	"seduction",	"lines",
		"flirt_db",		"risqué",0
	},
	
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
		"Real-world props and objects",
		"Digital props and objects",
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
