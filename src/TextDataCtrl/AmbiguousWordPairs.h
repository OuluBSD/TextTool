#ifndef _TextDataCtrl_AmbiguousWordPairs_h_
#define _TextDataCtrl_AmbiguousWordPairs_h_


BEGIN_TEXTLIB_NAMESPACE


class AmbiguousWordPairs : public ToolAppCtrl {
	Splitter vsplit, hsplit;
	ArrayCtrl texts;
	
public:
	typedef AmbiguousWordPairs CLASSNAME;
	AmbiguousWordPairs();
	
	void Data() override;
	void ToolMenu(Bar& bar) override;
	void Process();
	void ProcessUsingExisting();
	
};


END_TEXTLIB_NAMESPACE


#endif
