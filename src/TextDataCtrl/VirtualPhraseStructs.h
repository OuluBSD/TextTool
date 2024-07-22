#ifndef _TextDataCtrl_VirtualPhraseStructs_h_
#define _TextDataCtrl_VirtualPhraseStructs_h_


BEGIN_TEXTLIB_NAMESPACE


class VirtualPhraseStructs : public ToolAppCtrl {
	Splitter vsplit, hsplit;
	ArrayCtrl texts, parts;
	
public:
	typedef VirtualPhraseStructs CLASSNAME;
	VirtualPhraseStructs();
	
	void Data() override;
	void ToolMenu(Bar& bar) override;
	void Do(int fn);
	
};


END_TEXTLIB_NAMESPACE


#endif
