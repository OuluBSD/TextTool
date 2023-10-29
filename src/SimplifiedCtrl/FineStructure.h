#ifndef _SimplifiedCtrl_FineStructure_h_
#define _SimplifiedCtrl_FineStructure_h_


class FineStructureCtrl : public LineListCtrl {
	Splitter hsplit;
	ArrayCtrl rhymes, props;
	
public:
	typedef FineStructureCtrl CLASSNAME;
	FineStructureCtrl();
	
	void DisableAll();
	void EnableAll();
	void Data() override;
	void DataRhyme();
	void DataRhymeParams();
	void ToolMenu(Bar& bar) override;
	
};


#endif
