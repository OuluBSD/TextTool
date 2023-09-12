#ifndef _SongTool_ChecklistMusicVideoProduction_h_
#define _SongTool_ChecklistMusicVideoProduction_h_


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


#endif
