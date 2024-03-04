#ifndef _SocialDataCtrl_VirtualPhrases_h_
#define _SocialDataCtrl_VirtualPhrases_h_


String GetTypePhraseString(const Vector<int>& word_classes, const DatasetAnalysis& da);

class VirtualPhrases : public ToolAppCtrl {
	Splitter vsplit, hsplit;
	ArrayCtrl datasets, texts, parts;
	
public:
	typedef VirtualPhrases CLASSNAME;
	VirtualPhrases();
	
	void Data() override;
	void DataDataset();
	void ToolMenu(Bar& bar) override;
	void ProcessVirtualPhrases();
	void ProcessVirtualPhraseParts();
	
};


#endif
