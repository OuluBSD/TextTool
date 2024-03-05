#ifndef _SocialDataCtrl_VirtualPhraseStructs_h_
#define _SocialDataCtrl_VirtualPhraseStructs_h_


BEGIN_SOCIALLIB_NAMESPACE


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


END_SOCIALLIB_NAMESPACE


#endif
