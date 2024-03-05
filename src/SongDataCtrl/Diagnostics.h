#ifndef _SongDataCtrl_Diagnostics_h_
#define _SongDataCtrl_Diagnostics_h_


BEGIN_SONGLIB_NAMESPACE


class SongDataDiagnostics : public ToolAppCtrl {
	Splitter hsplit;
	ArrayCtrl datasets, values;
	
public:
	typedef SongDataDiagnostics CLASSNAME;
	SongDataDiagnostics();
	
	void Data() override;
	void DataDataset();
	void ToolMenu(Bar& bar) override;
	
	
};


END_SONGLIB_NAMESPACE


#endif
