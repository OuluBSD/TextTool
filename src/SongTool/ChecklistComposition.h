#ifndef _SongTool_ChecklistComposition_h_
#define _SongTool_ChecklistComposition_h_


#define CHK_COMPOSITION_LIST \
	CHK_COMP(CHORD_PAD, "chord pad") \
	CHK_COMP(VOCAL_MELODY, "vocal melody") \
	CHK_COMP(MID_SOLO, "mid solo (instrumental theme)") \
	CHK_COMP(HIGH_SOLO, "high solo (e.g. bridge climax solo)") \
	CHK_COMP(MID_SOLO_FILLS, "mid solo fills (e.g. between singing)") \
	CHK_COMP(HIGH_RHYTHM, "high rhythm (e.g. high muted guitar)") \
	CHK_COMP(MID_RHYTHM, "mid rhythm (low guitar, e.g. strumming)") \
	CHK_COMP(LOW_RHYTHM, "low rhythm (bass)") \
	CHK_COMP(LOW_SOLO, "low solo (bass solo)") \
	CHK_COMP(IMPACTS, "impacts") \
	CHK_COMP(TEXTURE_PAD, "texture pad (arp)")


class ChecklistComposition : public SongToolCtrl {
	ArrayCtrl list;
	Vector<String> keys;
	Vector<Option*> opts;
	Vector<EditString*> edits;
	
	enum {
		#define CHK_COMP(e, s) e,
		CHK_COMPOSITION_LIST
		#undef CHK_COMP
		ITEM_LIST
	};
	
public:
	typedef ChecklistComposition CLASSNAME;
	ChecklistComposition();
	
	void Data();
	void OnOptionChange(int i);
	void OnValueChange(int i);
	
};


#endif
