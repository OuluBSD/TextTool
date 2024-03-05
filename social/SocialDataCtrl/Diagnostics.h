#ifndef _SocialDataCtrl_Diagnostics_h_
#define _SocialDataCtrl_Diagnostics_h_


BEGIN_SOCIALLIB_NAMESPACE


class ProgramDataDiagnostics : public ToolAppCtrl {
	Splitter hsplit;
	ArrayCtrl datasets, values;
	
public:
	typedef ProgramDataDiagnostics CLASSNAME;
	ProgramDataDiagnostics();
	
	void Data() override;
	void DataDataset();
	void ToolMenu(Bar& bar) override;
	
	
};


END_SOCIALLIB_NAMESPACE


#endif
