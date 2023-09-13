#ifndef _SongTool_ChecklistProduction_h_
#define _SongTool_ChecklistProduction_h_


#define CHK_COMPOSITION_LIST \
	CHK_COMP(CHORD_PAD, t_("chord pad")) \
	CHK_COMP(VOCAL_MELODY, t_("vocal melody")) \
	CHK_COMP(DRUMKIT, t_("drumkit")) \
	CHK_COMP(HAND_DRUMS, t_("hand drums")) \
	CHK_COMP(LOW_RHYTHM, t_("low rhythm (bass)")) \
	CHK_COMP(MID_RHYTHM, t_("mid rhythm (low guitar, e.g. strumming)")) \
	CHK_COMP(HIGH_RHYTHM, t_("high rhythm (e.g. high muted guitar)")) \
	CHK_COMP(LOW_SOLO, t_("low solo (bass solo)")) \
	CHK_COMP(MID_SOLO, t_("mid solo (instrumental theme)")) \
	CHK_COMP(HIGH_SOLO, t_("high solo (e.g. bridge climax solo)")) \
	CHK_COMP(MID_SOLO_FILLS, t_("mid fills (e.g. between singing)")) \
	CHK_COMP(IMPACTS, t_("impacts")) \
	CHK_COMP(TEXTURE_PAD, t_("texture pad (arp)"))

class ChecklistComposition : public ChecklistBase {
	
	enum {
		#define CHK_COMP(e, s) e,
		CHK_COMPOSITION_LIST
		#undef CHK_COMP
		ITEM_LIST
	};
	
public:
	typedef ChecklistComposition CLASSNAME;
	ChecklistComposition();
	
	void InitKeys() override;
	
};



#define CHK_PRODUCTION_LIST \
	//CHK_PRODUCTION(, "") \

class ChecklistProduction : public ChecklistBase {
	
	enum {
		#define CHK_PRODUCTION(e, s) e,
		CHK_PRODUCTION_LIST
		#undef CHK_PRODUCTION
		ITEM_LIST
	};
	
public:
	typedef ChecklistProduction CLASSNAME;
	ChecklistProduction();
	
	void InitKeys() override;
	
};


#define CHK_MUSICVID_LIST \
	//CHK_MUSICVID(, "") \

class ChecklistMusicVideoProduction : public ChecklistBase {
	
	enum {
		#define CHK_MUSICVID(e, s) e,
		CHK_MUSICVID_LIST
		#undef CHK_COMP
		ITEM_LIST
	};
	
public:
	typedef ChecklistMusicVideoProduction CLASSNAME;
	ChecklistMusicVideoProduction();
	
	void InitKeys() override;
	
};




#define CHK_RELEASE_LIST \
	//CHK_RELEASE(, "") \

class ChecklistRelease : public ChecklistBase {
	
	enum {
		#define CHK_RELEASE(e, s) e,
		CHK_RELEASE_LIST
		#undef CHK_RELEASE
		ITEM_LIST
	};
	
public:
	typedef ChecklistRelease CLASSNAME;
	ChecklistRelease();
	
	void InitKeys() override;
	
};



#endif
