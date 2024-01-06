#ifndef _SongDataCtrl_VirtualPhraseStructs_h_
#define _SongDataCtrl_VirtualPhraseStructs_h_


class VirtualPhraseStructs : public SongToolCtrl {
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


#endif
