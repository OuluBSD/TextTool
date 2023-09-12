#ifndef _SongTool_ChecklistProduction_h_
#define _SongTool_ChecklistProduction_h_


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


#endif
