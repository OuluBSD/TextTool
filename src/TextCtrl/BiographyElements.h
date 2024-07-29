#ifndef _TextCtrl_BiographyElements_h_
#define _TextCtrl_BiographyElements_h_


BEGIN_TEXTLIB_NAMESPACE


class BiographyElementsCtrl : public ToolAppCtrl {
	Splitter hsplit, vsplit;
	ArrayCtrl categories, elements;
	WithBiography<Ctrl> block;
	int sort_column = 0;
public:
	typedef BiographyElementsCtrl CLASSNAME;
	BiographyElementsCtrl();
	
	void Data() override;
	void Do(int fn);
	void ToolMenu(Bar& bar) override;
	void DataCategory();
	void DataElement();
	void DataYear();
	void OnValueChange();
	
	
};


END_TEXTLIB_NAMESPACE


#endif
