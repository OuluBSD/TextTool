#ifndef _SongDataCtrl_VirtualPhraseParts_h_
#define _SongDataCtrl_VirtualPhraseParts_h_


class VirtualPhraseParts : public SongToolCtrl {
	Splitter vsplit, hsplit;
	ArrayCtrl datasets, texts, parts;
	
public:
	typedef VirtualPhraseParts CLASSNAME;
	VirtualPhraseParts();
	
	void Data() override;
	void DataDataset();
	void ToolMenu(Bar& bar) override;
	void ProcessStructureNames();
	
};


#endif
