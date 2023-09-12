#ifndef _SongTool_ChecklistRelease_h_
#define _SongTool_ChecklistRelease_h_


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
