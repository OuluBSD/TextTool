#ifndef _SongTool_SongBriefing_h_
#define _SongTool_SongBriefing_h_

#if 0

class SongBriefing : public SongToolCtrl {
	Splitter vsplit;
	ArrayCtrl list;
	WithKeyValueList<Ctrl> values;
	
	// TODO remove
	//ArrayCtrl poll;
	
	
	enum {
		ATTR_REFERENCE_SONG,
		ATTR_BIRTH_OF_SONG,
		ATTR_CONTENT_VISION,
		ATTR_DATASET,
		ATTR_TYPECAST,
		ATTR_CONTRAST,
		ATTR_ARCHETYPE,
		ATTR_IS_RAPPER,
		
		
		/*#define ATTR_ITEM(e, g, i0, i1) ITEM_GENERIC_##e,
		ATTR_LIST
		#undef ATTR_ITEM
		
		#define ATTR_ITEM(e, g, i0, i1) ITEM_IMPACT_##e,
		ATTR_LIST
		#undef ATTR_ITEM*/
		
		ITEM_COUNT
	};
	
	
	//DropList* SetAgreementValues(DropList* dl, const char* positive, const char* negative, int idx, const char* key);
	
public:
	typedef SongBriefing CLASSNAME;
	SongBriefing();
	
	void Data();
	void OnListCursor();
	void OnValueChange();
	//void OnPollValueChange(int i, const char* key);
	
};


#endif
#endif
