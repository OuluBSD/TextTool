#ifndef _TextDataCtrl_VirtualPhraseParts_h_
#define _TextDataCtrl_VirtualPhraseParts_h_


BEGIN_TEXTLIB_NAMESPACE


class VirtualPhraseParts : public ToolAppCtrl {
	Splitter vsplit, hsplit;
	ArrayCtrl texts, parts;
	
public:
	typedef VirtualPhraseParts CLASSNAME;
	VirtualPhraseParts();
	
	void Data() override;
	void ToolMenu(Bar& bar) override;
	void Do(int fn);
	
};


END_TEXTLIB_NAMESPACE


#endif
