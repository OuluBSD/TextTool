#ifndef _SocialDataCtrl_PhraseParts_h_
#define _SocialDataCtrl_PhraseParts_h_


BEGIN_SOCIALLIB_NAMESPACE


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


END_SOCIALLIB_NAMESPACE


#endif
