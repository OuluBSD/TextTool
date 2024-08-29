#ifndef _TextDatabase_Browser_h_
#define _TextDatabase_Browser_h_


BEGIN_TEXTLIB_NAMESPACE

struct VMapSumSorter {
	bool operator()(const VectorMap<String, int>& a, const VectorMap<String, int>& b) const {
		int asum = 0; for (int i : a.GetValues()) asum += i;
		int bsum = 0; for (int i : b.GetValues()) bsum += i;
		return asum > bsum;
	}
};

class DatabaseBrowser {
	
public:
	typedef enum : int {
		INVALID = -1,
		
		ATTR_GROUP,
		ATTR_VALUE,
		COLOR,
		ACTION,
		ACTION_ARG,
		ELEMENT,
		TYPECLASS,
		CONTRAST,
		
		TYPE_COUNT
	} ColumnType;
	using T = ColumnType;
	
	static String GetTypeString(ColumnType t);
	
	enum {
		ATTR_COLOR_ACTION,
		ATTR_ACTION_COLOR,
		COLOR_ACTION_ATTR,
		COLOR_ATTR_ACTION,
		ACTION_COLOR_ATTR,
		ACTION_ATTR_COLOR,
		TYPECLASS_CONTENT_COLOR,
		TYPECLASS_COLOR_CONTENT,
		CONTENT_TYPECLASS_COLOR,
		CONTENT_COLOR_TYPECLASS,
		COLOR_CONTENT_TYPECLASS,
		COLOR_TYPECLASS_CONTENT,
		
		MODE_COUNT
	};
	static String GetModeString(int i);
	
private:
	VectorMap<String, VectorMap<String, int>> uniq_acts;
	VectorMap<String, VectorMap<String, int>> uniq_attr;
	VectorMap<String, int> uniq_attr_values, uniq_act_args;
	Vector<int> color_counts, rm_list;
	
	int cursor[TYPE_COUNT] = {0,0,0,0,0,0,0,0};
	ColumnType order[TYPE_COUNT] = {(T)0,(T)1,(T)2,(T)3,(T)4,(T)-1,(T)-1,(T)-1};
	//VectorMap<int,int> uniq_attrs;
	//Vector<int> color_counts;
	VectorMap<hash_t,int> history;
	
	
public:
	struct Item : Moveable<Item> {
		String str;
		int count;
		int idx = -1;
		bool operator()(const Item& a, const Item& b) const {return a.count > b.count;}
	};
	
	Vector<Item> items[TYPE_COUNT];
	int mode = ATTR_COLOR_ACTION;
	int appmode = DB_SONG;
	Vector<int> phrase_parts;
	int sorting = 0;
	bool filter_mid_rhyme = false, filter_end_rhyme = false;
	WString mid_rhyme, end_rhyme;
	double mid_rhyme_distance_limit = 0.005;
	double end_rhyme_distance_limit = 0.005;
	
	bool FilterPronounciation(DatasetAnalysis& da, const PhrasePart& pp);
	
	ToolAppCtrl* ctrl = 0;
	
public:
	
	DatabaseBrowser();
	Vector<Item>& Get(ColumnType t);
	bool IsFirstInOrder(ColumnType t) const;
	const Vector<Item>& Get(ColumnType t) const;
	int GetColumnCursor(ColumnType t) const;
	int GetColumnOrder(ColumnType t) const;
	int GetMode() const {return mode;}
	void SetColumnCursor(ColumnType t, int i);
	void FillItems(ColumnType t);
	void SetCtrl(ToolAppCtrl& c) {ctrl = &c;}
	void Init();
	void Update();
	void SetMode(int i);
	void SetMidRhymeFilter(WString wrd, bool up=true);
	void SetEndRhymeFilter(WString wrd, bool up=true);
	void SetMidRhymingLimit(double d, bool up=true);
	void SetEndRhymingLimit(double d, bool up=true);
	void SetInitialData();
	void SetAttrGroup(int i);
	void SetAttrValue(int i);
	void SetColor(int i);
	void SetAction(int i);
	void SetActionArg(int i);
	void SetElement(int i);
	void SetTypeclass(int i);
	void SetContrast(int i);
	void ResetCursor();
	void ResetCursor(int c, ColumnType type);
	void SetCursor(int i, ColumnType t);
	void DataCursor(int cursor);
	void DataCursorTail(int cursor);
	ColumnType GetCur(int cursor_i) const;
	bool IsSub(int cur, int cursor_i) const;
	double GetMidRhymingLimit() const {return mid_rhyme_distance_limit;}
	double GetEndRhymingLimit() const {return end_rhyme_distance_limit;}
	void SetAll(const AttrHeader& attr, int clr, const ActionHeader& act);
	int FindAction(const String& s);
	int FindArg(const String& s);
	void RealizeUniqueAttrs();
	void RealizeUniqueActions();
	void FilterData(ColumnType t);
	void FilterAll();
	void FilterNextFrom(ColumnType t);
	ColumnType GetOrder(int i) {ASSERT(i >= 0 && i < (int)TYPE_COUNT); return order[i];}
	
	// Mode 0: Attribute - Color - Action
	// Mode 1: Action - Color - Attribute
	// Mode 2: Color - Action - Attribute
	// Mode 3: Color - Action group - Attribute - Action value
	
	void SortBy(int i);
	void Serialize(Stream& s) {s % history;}
	void Store();
	void Load();
	
	hash_t GetHash(int columns) const;
	TextDatabase& GetDatabase();
	
	static DatabaseBrowser& Single(int appmode);
	
};


END_TEXTLIB_NAMESPACE


#endif
