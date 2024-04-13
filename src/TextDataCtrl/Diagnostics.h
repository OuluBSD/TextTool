#ifndef _TextDataCtrl_Diagnostics_h_
#define _TextDataCtrl_Diagnostics_h_


BEGIN_TEXTLIB_NAMESPACE


class TextDataDiagnostics : public ToolAppCtrl {
	Splitter hsplit;
	ArrayCtrl values;
	
public:
	typedef TextDataDiagnostics CLASSNAME;
	TextDataDiagnostics();
	
	void Data() override;
	void ToolMenu(Bar& bar) override;
	
	
};


END_TEXTLIB_NAMESPACE


#endif
