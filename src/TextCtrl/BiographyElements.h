#ifndef _TextCtrl_BiographyElements_h_
#define _TextCtrl_BiographyElements_h_


BEGIN_TEXTLIB_NAMESPACE


class BiographyElementsCtrl : public ToolAppCtrl {
	Splitter hsplit, vsplit;
	ArrayCtrl categories, blocks;
	ArrayCtrl block;
	
public:
	typedef BiographyElementsCtrl CLASSNAME;
	BiographyElementsCtrl();
	
	void Data() override;
	void Do(int fn);
	void ToolMenu(Bar& bar) override;
	void DataCategory();
	void DataYear();
	void OnValueChange();
	
	
};


END_TEXTLIB_NAMESPACE


#endif
