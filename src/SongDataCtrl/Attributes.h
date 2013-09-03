#ifndef _SongDataCtrl_Attributes_h_
#define _SongDataCtrl_Attributes_h_


class Attributes : public SongToolCtrl {
	Splitter hsplit, vsplit;
	ArrayCtrl datasets, groups, values, pos_values, neg_values;
	
	VectorMap<String,Index<String>> uniq_attrs;
	
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
