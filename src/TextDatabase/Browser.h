#ifndef _TextDatabase_Browser_h_
#define _TextDatabase_Browser_h_


BEGIN_TEXTLIB_NAMESPACE


class DatabaseBrowser {
	VectorMap<String, VectorMap<String, int>> uniq_acts;
	int cursor[4] = {0,0,0,0};
	VectorMap<int,int> uniq_attrs;
	Vector<int> color_counts;
	VectorMap<hash_t,int> history;
	
	
public:
	struct Attr : Moveable<Attr> {
		String group, value;
		int count;
		int attr_i;
		
		bool operator()(const Attr& a, const Attr& b) const {return a.count > b.count;}
	};
	struct ColorGroup : Moveable<ColorGroup> {
		String group;
		int clr_i;
		Color clr;
		int count;
		
		bool operator()(const ColorGroup& a, const ColorGroup& b) const {return a.count > b.count;}
	};
	struct ActionGroup : Moveable<ActionGroup> {
		String group;
		int count;
		bool operator()(const ActionGroup& a, const ActionGroup& b) const {return a.count > b.count;}
	};
	struct ActionValue : Moveable<ActionValue> {
		String value;
		int count;
		bool operator()(const ActionValue& a, const ActionValue& b) const {return a.count > b.count;}
	};
	
	Vector<Attr>		attrs;
	Vector<ColorGroup>	colors;
	Vector<ActionGroup> groups;
	Vector<ActionValue> values;
	int mode = -1;
	Vector<int> data;
	int sorting = 0;
	bool filter_mid_rhyme = false, filter_end_rhyme = false;
	WString mid_rhyme, end_rhyme;
	double mid_rhyme_distance_limit = 0.005;
	double end_rhyme_distance_limit = 0.005;
	
	bool FilterPronounciation(DatasetAnalysis& da, const PhrasePart& pp);
	
	ToolAppCtrl* ctrl = 0;
	
public:
	
	DatabaseBrowser();
	void SetCtrl(ToolAppCtrl& c) {ctrl = &c;}
	void Init();
	void Update();
	void SetMode(int i);
	void SetMidRhymeFilter(WString wrd, bool up=true);
	void SetEndRhymeFilter(WString wrd, bool up=true);
	void SetMidRhymingLimit(double d, bool up=true);
	void SetEndRhymingLimit(double d, bool up=true);
	void SetAttr(int i);
	void SetColor(int i);
	void SetGroup(int i);
	void SetValue(int i);
	void DataAttr();
	void DataColor();
	void DataGroup();
	void DataValue();
	int GetCursorValue(int cursor_i) const;
	int GetCur(int cursor_i) const;
	bool IsSub(int cur, int cursor_i) const;
	double GetMidRhymingLimit() const {return mid_rhyme_distance_limit;}
	double GetEndRhymingLimit() const {return end_rhyme_distance_limit;}
	
	// Mode 0: Attribute - Color - Action
	void SetAttr0(int i);
	void SetColor0(int i);
	void SetGroup0(int i);
	void SetValue0(int i);
	void DataAttr0();
	void DataColor0();
	void DataGroup0();
	void DataValue0();
	
	// Mode 1: Action - Color - Attribute
	void SetAttr1(int i);
	void SetColor1(int i);
	void SetGroup1(int i);
	void SetValue1(int i);
	void DataAttr1();
	void DataColor1();
	void DataGroup1();
	void DataValue1();
	
	// Mode 2: Color - Action - Attribute
	void SetAttr2(int i);
	void SetColor2(int i);
	void SetGroup2(int i);
	void SetValue2(int i);
	void DataAttr2();
	void DataColor2();
	void DataGroup2();
	void DataValue2();
	
	// Mode 3: Color - Action group - Attribute - Action value
	void SetAttr3(int i);
	void SetColor3(int i);
	void SetGroup3(int i);
	void SetValue3(int i);
	void DataAttr3();
	void DataColor3();
	void DataGroup3();
	void DataValue3();
	
	void SortBy(int i);
	void Serialize(Stream& s) {s % history;}
	void Store();
	void Load();
	
	hash_t GetHash(bool attr, bool clr, bool group, bool value) const;
	TextDatabase& GetDatabase();
	
	static DatabaseBrowser& Single(int appmode);
	
};


END_TEXTLIB_NAMESPACE


#endif
