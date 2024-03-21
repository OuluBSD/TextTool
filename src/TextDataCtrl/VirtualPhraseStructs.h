#ifndef _TextDataCtrl_VirtualPhraseStructs_h_
#define _TextDataCtrl_VirtualPhraseStructs_h_


BEGIN_TEXTLIB_NAMESPACE


class VirtualPhraseStructs : public ToolAppCtrl {
	Splitter vsplit, hsplit;
	ArrayCtrl datasets, texts, parts;
	
public:
	typedef VirtualPhraseStructs CLASSNAME;
	VirtualPhraseStructs();
	
	void Data() override;
	void DataDataset();
	void ToolMenu(Bar& bar) override;
	void Process();
	
};


END_TEXTLIB_NAMESPACE


#endif
