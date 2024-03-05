#ifndef _SongDataCtrl_AmbiguousWordPairs_h_
#define _SongDataCtrl_AmbiguousWordPairs_h_


BEGIN_SONGLIB_NAMESPACE


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


END_SONGLIB_NAMESPACE


#endif
