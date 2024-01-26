#ifndef _Database_Browser_h_
#define _Database_Browser_h_


class DatabaseBrowser {
	struct Dataset : Moveable<Dataset> {
		VectorMap<hash_t,int> attr_his, color_his, group_his;
		void Serialize(Stream& s) {s % attr_his % color_his % group_his;}
	};
	VectorMap<String, VectorMap<String, int>> uniq_acts;
	int cursor[4] = {0,0,0,0};
	VectorMap<int, Dataset> datasets;
	
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
	};
	struct ActionValue : Moveable<ActionValue> {
		String value;
		int count;
	};
	
	Vector<Attr>		attrs;
	Vector<ColorGroup>	colors;
	Vector<ActionGroup> groups;
	Vector<ActionValue> values;
	int ds_i = -1;
	Vector<int> data;
	int sorting = 0;
	
public:
	
	DatabaseBrowser();
	void SetDataset(int i);
	void SetAttr(int i);
	void SetColor(int i);
	void SetGroup(int i);
	void SetValue(int i);
	
	void DataAttr();
	void DataColor();
	void DataGroup();
	void DataValue();
	
	void SortBy(int i);
	void Serialize(Stream& s) {s % datasets;}
	void Store();
	void Load();
	
	static DatabaseBrowser& Single() {static DatabaseBrowser db; return db;}
	
};


#endif
