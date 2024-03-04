#ifndef _SongDataCtrl_PhraseParts_h_
#define _SongDataCtrl_PhraseParts_h_


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
	
};


#endif
