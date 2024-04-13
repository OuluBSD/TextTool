#ifndef _TextDataCtrl_PhraseParts_h_
#define _TextDataCtrl_PhraseParts_h_


BEGIN_TEXTLIB_NAMESPACE


class PhraseParts : public ToolAppCtrl {
	Splitter vsplit, hsplit;
	ArrayCtrl texts, parts;
	
public:
	typedef PhraseParts CLASSNAME;
	PhraseParts();
	
	void Data() override;
	void ToolMenu(Bar& bar) override;
	void DoWords(int fn);
	void DoWordsUsingExisting(int fn);
	
};


END_TEXTLIB_NAMESPACE


#endif
