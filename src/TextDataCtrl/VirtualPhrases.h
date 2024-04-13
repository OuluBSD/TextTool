#ifndef _TextDataCtrl_VirtualPhrases_h_
#define _TextDataCtrl_VirtualPhrases_h_


BEGIN_TEXTLIB_NAMESPACE


String GetTypePhraseString(const Vector<int>& word_classes, const DatasetAnalysis& da);

class VirtualPhrases : public ToolAppCtrl {
	Splitter vsplit, hsplit;
	ArrayCtrl texts, parts;
	
public:
	typedef VirtualPhrases CLASSNAME;
	VirtualPhrases();
	
	void Data() override;
	void ToolMenu(Bar& bar) override;
	void ProcessVirtualPhrases();
	void ProcessVirtualPhraseParts();
	void ProcessVirtualPhrasePartsUsingExisting();
	
};


END_TEXTLIB_NAMESPACE


#endif
