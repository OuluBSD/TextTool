#ifndef _SongDataCtrl_VirtualPhraseParts_h_
#define _SongDataCtrl_VirtualPhraseParts_h_


BEGIN_SONGLIB_NAMESPACE


class VirtualPhraseParts : public ToolAppCtrl {
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


END_SONGLIB_NAMESPACE


#endif
