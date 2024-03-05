#ifndef _SocialDataCtrl_VirtualPhraseParts_h_
#define _SocialDataCtrl_VirtualPhraseParts_h_


BEGIN_SOCIALLIB_NAMESPACE


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


END_SOCIALLIB_NAMESPACE


#endif
