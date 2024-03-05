#ifndef _SongDataCtrl_VirtualPhrases_h_
#define _SongDataCtrl_VirtualPhrases_h_


BEGIN_SONGLIB_NAMESPACE


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


END_SONGLIB_NAMESPACE


#endif
