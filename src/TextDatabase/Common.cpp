#include "TextDatabase.h"


BEGIN_TEXTLIB_NAMESPACE


String GetTextModeString(int i) {
	switch (i) {
		case TXT_NORMAL:		return "Normal";
		case TXT_PRE_REPEAT:	return "Pre-Repeat";
		case TXT_REPEAT:		return "Repeat";
		case TXT_TWIST:			return "Twist";
		default: TODO			return "";
	}
}

int& __global_appmode();


const char* AppModeKeys[DB_COUNT][AM_COUNT+1] = {
	
	// DB_SONG
	{
		"artist",		"release",		"song",		"song",
		"typecast",		"archetype",	"lyrics",	"lyrics",
		"db",			"rapper",		"listener",
		"music audience",
		"pop/rock/edm",
		"verse", "prechorus", "chorus", "bridge","storyteller","narcissistic",
		0
	},
	
	// DB_TWITTER
	{
		"company",		"campaign",		"program",	"tweet thread",
		"role",			"generic",		"story",	"tweets",
		"social_db",	"unsafe",		"reader",
		"twitter audience",
		"news/entertainment/sports",
		"thread", "prehook", "hook", "twist","storyteller","narcissistic",
		0
	},
	
	// DB_BLOG
	{
		"blogger",		"series",		"post",		"post",
		"persona",		"niche",		"content",	"content",
		"blog_db",		"provocative",	"reader",
		"blog's audience",
		"fashion/travel/food",
		"body", "preconclusion", "conclusion", "call to action","storyteller","narcissistic",
		0
	},
	
	// DB_DIALOG
	{
		"presenter",	"scriptbook",	"conversation",	"conversation",
		"character",	"trope",		"lines",		"lines",
		"dialog_db",	"controversial","observer",
		"conversation's audience",
		"personal/professional/casual",
		"main discussion", "opening", "arguments", "conclusion","storyteller","narcissistic",
		0
	},
	
	// DB_STORYBOARD
	{
		"director",			"reel"	,		"sequence",		"sequence",
		"style",			"approach",		"captions",		"captions",
		"storyboard_db",	"provocative",	"viewer",
		"story's viewers",
		"action/adventure/romance",
		"key frames", "action sequences", "cinematic shots", "dialogue scenes","storyteller","narcissistic",
		0
	},
	
	
	
	#if 0
	TODO // missing values... see ""
	
	// DB_POLITICS
	{
		"politician",	"discourse",	"debate",		"debate",
		"belief",		"issue",		"discussion",	"discussion",
		"politics_db",	"heated",		"citizen",
		"debate's audience",
		"politics/policy/events",
		"statements", "rebuttals", "cross-examination", "closing arguments","","",
		0
	},
	
	// DB_LIFE
	{
		"human",		"collection",	"experience",	"experience",
		"persona",		"struggle",		"lesson",		"lesson",
		"life_db",		"contentious",	"observer",
		"storytelling audience",
		"existentialism/morality/metaphysics",
		"exposition", "conflict", "climax", "resolution","","",
		0
	},
	
	// DB_BUSINESS
	{
		"entrepreneur",	"forum",		"meeting",		"meeting",
		"strategy",		"topic",		"talking",		"talking",
		"business_db",	"contentious",	"reader",
		"meeting's audience",
		"negotiation/strategy/planning",
		"agenda", "new information", "decision-making", "follow-up tasks","","",
		0
	},
	
	// DB_FLIRTING
	{
		"flirt",		"ritual",		"folly",		"folly",
		"technique",	"seduction",	"lines",	"lines",
		"flirt_db",		"risqué",		"partner",
		"conversation's audience",
		"romance/flirting/seduction",
		"playful banter", "compliments", "sexual innuendos", "plans for future dates","","",
		0
	},
	#endif
};

const char* AppModeKeysN[DB_COUNT][AM_COUNT+1] = {
	{
		"artists","releases","songs",
		"songs","typecasts","archetypes",
		"lyrics","lyrics",0,0,0,0,0,0},
	{
		"companies","campaigns","programs",
		"tweet threads","roles","generics",
		"stories","tweets",0,0,0,0,0},
	{
		"bloggers","series","posts",
		"posts","personas","niches",
		"contents","contents",0,0,0,0,0},
	{
		"presenters","scriptbooks","conversations",
		"conversations","characters","tropes",
		"lines","lines",0,0,0,0,0},
	{
		"directors","reels","sequences",
		"storyboard sketches","styles","approaches",
		"captions","captions",0,0,0,0,0},
};

const char* AppModeLabels[DB_COUNT][AML_COUNT] = {
	
	// DB_SONG
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
		
		"rhymes with internal rhyme scheme",
		"rhyme pairs/triplets/quads/etc.",
		"poetic parts of rhyming text lines",
		"Not a folktale",
		"This is a folktale",
		
		"Not narcissistic",
		"This is narcissistic",
	},
	
	// DB_TWITTER
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
		
		"is coherent primarily in internal references",
		"tweets with same context",
		"coherent parts of contextually same lines",
		"Not a anecdote",
		"This is a anecdote",
		
		"Not narcissistic",
		"This is narcissistic",
	},
	
	// DB_BLOG
	{
		"Writing style:",
		"Real-world props and objects",
		"Digital props and objects",
		"Writer",
		"Vibe of text",
		"Series",
		"Blogger (if different):",
		"Reference post:",
		"Origins of the post:",
		"Active post structure",
		"Speed",
		"Safe",
		"Unsafe",
		
		"natural_features",
		"electronic_features",
		"person_visual",
		"vibe_of_text",
		"text_style",
		
		"is relying on beliefs and own research",
		"posts with same context",
		"coherent parts of contextually same lines",
		"Not a memory",
		"This is a memory",
		
		"Not narcissistic",
		"This is narcissistic",
	},
	
	// DB_DIALOG
	{
		"Talking style:",
		"Real-world props and objects",
		"Digital props and objects",
		"Speaker",
		"Vibe of talking",
		"Series",
		"Speaker (if different):",
		"Reference post:",
		"Origins of the post:",
		"Active conversation structure",
		"Speed",
		"Safe",
		"Unsafe",
		
		"natural_features",
		"electronic_features",
		"person_visual",
		"vibe_of_talking",
		"talking_style",
		
		"is relying on talking over others",
		"dialogue lines with same context",
		"coherent parts of contextually same dialogue lines",
		"Not a flashback",
		"This is a flashback",
		
		"Not narcissistic",
		"This is narcissistic",
	},
	
	// DB_STORYBOARD
	{
		"Imagination style:",
		"Real-world props and objects",
		"Digital props and objects",
		"Drawer",
		"Vibe of drawing",
		"Series",
		"Drawer (if different):",
		"Reference sequence:",
		"Origins of the sequence:",
		"Active sequence structure",
		"Speed",
		"Safe",
		"Unsafe",
		
		"natural_features",
		"electronic_features",
		"person_visual",
		"vibe_of_imagination",
		"imagination_style",
		
		"is relying on strong emotional reactions",
		"storyboard images with same context",
		"coherent parts of contextually same storyboard images",
		"Not a folktale",
		"This is a folktale",
		
		"Not narcissistic",
		"This is narcissistic",
	},
};

String GetAppModeLabel(int amlkey) {
	int appmode = __global_appmode() - 1;
	ASSERT(appmode >= 0 && appmode < DB_COUNT);
	ASSERT(amlkey >= 0 && amlkey < AML_COUNT);
	return AppModeLabels[appmode][amlkey];
}

String GetAppModeLabel(int appmode, int amlkey) {
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


String RemUscore(String s) {
	s.Replace("_", " ");
	return s;
}


END_TEXTLIB_NAMESPACE
