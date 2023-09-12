#ifndef _SongTool_ChecklistComposition_h_
#define _SongTool_ChecklistComposition_h_


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


class ChecklistBase : public SongToolCtrl {
	ArrayCtrl list;
	Vector<Option*> opts;
	Vector<EditString*> edits;
	
	void Init();
	
protected:
	Vector<String> descs;
	Vector<String> keys;
	
public:
	typedef ChecklistBase CLASSNAME;
	ChecklistBase();
	
	void Data();
	void OnOptionChange(int i);
	void OnValueChange(int i);
	virtual void InitKeys() = 0;
	
};

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

#endif
