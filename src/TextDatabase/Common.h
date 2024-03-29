#ifndef _TextDatabase_Common_h_
#define _TextDatabase_Common_h_

BEGIN_TEXTLIB_NAMESPACE


String GetAppModeKeyEntities();
String GetAppModeKeySnapshots();
String GetAppModeKeyComponents();

enum {
	AM_ENTITY,
	AM_SNAPSHOT,
	AM_COMPONENT,
	AM_TYPECLASS,
	AM_CONTENT,
	AM_SCRIPT,
	AM_DB,
	AM_UNSAFE,
	AM_CLIENT,
	
	AM_COUNT,
};

enum {
	AML_TALKINGSTYLE,
	AML_NATURAL_TOOLS,
	AML_ELECTRONIC_TOOLS,
	AML_SPEAKER,
	AML_VIBE_OF_TEXT,
	
	AML_SNAPSHOT,
	AML_ENTITY_IF_DIFFERENT,
	AML_REFERENCE_COMPONENT,
	AML_COMPONENT_ORIGINS,
	AML_COMPONENT_STRUCTURE,
	AML_SPEED,
	AML_SAFE_DESC,
	AML_UNSAFE_DESC,
	
	AML_FIELD_NATURAL_TOOLS,
	AML_FIELD_ELECTRONIC_TOOLS,
	AML_FIELD_PERSON_VISUALLY,
	AML_FIELD_VIBE_OF_TEXT,
	AML_FIELD_TEXT_STYLE,
	
	AML_INTERNAL_REFS,
	AML_INTERNAL_REF_TYPES,
	AML_PARTS_OF_SAME_LINES,
	
	AML_COUNT,
};

extern const char* AppModeLabels[DB_COUNT][AML_COUNT];
extern const char* AppModeKeys[DB_COUNT][AM_COUNT+1];
extern const char* AppModeKeysN[DB_COUNT][AM_COUNT+1];

String GetAppModeLabel(int amlkey);
String GetAppModeLabel(int appmode, int amlkey);
String GetAppModeKey(int appmode, int amkey);
String GetAppModeKey(int amkey);
String GetAppModeKeyN(int appmode, int amkey);
String GetAppModeKeyN(int amkey);
String GetAppModeKeyCap(int amkey);
String GetAppModeKeyCapN(int amkey);


#define __entity GetAppModeKey(AM_ENTITY)
#define __entities GetAppModeKeyN(AM_ENTITY)
#define __Entity GetAppModeKeyCap(AM_ENTITY)
#define __Entities GetAppModeKeyCapN(AM_ENTITY)
#define __snap GetAppModeKey(AM_SNAPSHOT)
#define __snaps GetAppModeKeyN(AM_SNAPSHOT)
#define __Snap GetAppModeKeyCap(AM_SNAPSHOT)
#define __Snaps GetAppModeKeyCapN(AM_SNAPSHOT)
#define __comp GetAppModeKey(AM_COMPONENT)
#define __comps GetAppModeKeyN(AM_COMPONENT)
#define __Comp GetAppModeKeyCap(AM_COMPONENT)
#define __Comps GetAppModeKeyCapN(AM_COMPONENT)
#define __typeclass GetAppModeKey(AM_TYPECLASS)
#define __typeclasses GetAppModeKeyN(AM_TYPECLASS)
#define __Typeclass GetAppModeKeyCap(AM_TYPECLASS)
#define __Typeclasses GetAppModeKeyCapN(AM_TYPECLASS)
#define __content GetAppModeKey(AM_CONTENT)
#define __contents GetAppModeKeyN(AM_CONTENT)
#define __Content GetAppModeKeyCap(AM_CONTENT)
#define __Contents GetAppModeKeyCapN(AM_CONTENT)
#define __script GetAppModeKey(AM_SCRIPT)
#define __scripts GetAppModeKeyN(AM_SCRIPT)
#define __Script GetAppModeKeyCap(AM_SCRIPT)
#define __Scripts GetAppModeKeyCapN(AM_SCRIPT)

#define __comp_(x) GetAppModeKey(x, AM_COMPONENT)
#define __comps_(x) GetAppModeKeyN(x, AM_COMPONENT)

#define __db GetAppModeKey(AM_DB)
#define __unsafe GetAppModeKey(AM_UNSAFE)
#define __client GetAppModeKey(AM_CLIENT)
#define __Client GetAppModeKeyCap(AM_CLIENT)


END_TEXTLIB_NAMESPACE

#endif
