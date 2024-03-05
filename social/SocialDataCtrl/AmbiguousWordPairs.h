#ifndef _SocialDataCtrl_AmbiguousWordPairs_h_
#define _SocialDataCtrl_AmbiguousWordPairs_h_


BEGIN_SOCIALLIB_NAMESPACE


class AmbiguousWordPairs : public ToolAppCtrl {
	Splitter vsplit, hsplit;
	ArrayCtrl datasets, texts;
	
public:
	typedef AmbiguousWordPairs CLASSNAME;
	AmbiguousWordPairs();
	
	void Data() override;
	void DataDataset();
	void ToolMenu(Bar& bar) override;
	void Process();
	
};


END_SOCIALLIB_NAMESPACE


#endif
