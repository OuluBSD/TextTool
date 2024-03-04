#ifndef _SongDataCtrl_Attributes_h_
#define _SongDataCtrl_Attributes_h_


class Attributes : public ToolAppCtrl {
	Splitter hsplit, vsplit;
	ArrayCtrl datasets, groups, values, pos_values, neg_values;
	
	VectorMap<String,Index<String>> uniq_attrs;
	VectorMap<String,Index<int>> uniq_attrs_i;
	
public:
	typedef Attributes CLASSNAME;
	Attributes();
	
	void Data() override;
	void DataDataset();
	void DataGroup();
	void DoAttributes(int fn);
	
	void ToolMenu(Bar& bar) override;
	
};


#endif
