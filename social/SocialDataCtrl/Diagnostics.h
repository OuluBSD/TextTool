#ifndef _SocialDataCtrl_Diagnostics_h_
#define _SocialDataCtrl_Diagnostics_h_


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


#endif
