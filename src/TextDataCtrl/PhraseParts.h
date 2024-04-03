#ifndef _TextDataCtrl_PhraseParts_h_
#define _TextDataCtrl_PhraseParts_h_


BEGIN_TEXTLIB_NAMESPACE


class PhraseParts : public ToolAppCtrl {
	Splitter vsplit, hsplit;
	ArrayCtrl datasets, texts, parts;
	
public:
	typedef PhraseParts CLASSNAME;
	PhraseParts();
	
	void Data() override;
	void DataDataset();
	void ToolMenu(Bar& bar) override;
	void Process();
	void ProcessUsingExisting();
	
};


END_TEXTLIB_NAMESPACE


#endif
