#ifndef _TextDataCtrl_StructuredScript_h_
#define _TextDataCtrl_StructuredScript_h_


BEGIN_TEXTLIB_NAMESPACE


class StructuredScriptCtrl : public ToolAppCtrl {
	Splitter hsplit;
	ArrayCtrl names, lines;
	
public:
	typedef StructuredScriptCtrl CLASSNAME;
	StructuredScriptCtrl();
	
	void Data() override;
	void DataScript();
	void PasteScript();
	void ToolMenu(Bar& bar) override;
	
	
};


END_TEXTLIB_NAMESPACE


#endif
