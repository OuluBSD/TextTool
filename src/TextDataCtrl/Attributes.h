#ifndef _TextDataCtrl_Attributes_h_
#define _TextDataCtrl_Attributes_h_


BEGIN_TEXTLIB_NAMESPACE


class Attributes : public ToolAppCtrl {
	Splitter hsplit, vsplit;
	ArrayCtrl datasets, groups, values, pos_values, neg_values;
	
	int uniq_ds_i = -1;
	VectorMap<String,Index<String>> uniq_attrs;
	VectorMap<String,Index<int>> uniq_attrs_i;
	
	void RealizeTemp();
	
public:
	typedef Attributes CLASSNAME;
	Attributes();
	
	void Data() override;
	void DataDataset();
	void DataGroup();
	void DoAttributes(int fn);
	
	void ToolMenu(Bar& bar) override;
	
};


END_TEXTLIB_NAMESPACE


#endif
