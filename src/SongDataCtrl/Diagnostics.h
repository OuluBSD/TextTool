#ifndef _SongDataCtrl_Diagnostics_h_
#define _SongDataCtrl_Diagnostics_h_


class SongDataDiagnostics : public SongToolCtrl {
	Splitter hsplit;
	ArrayCtrl datasets, values;
	
public:
	typedef SongDataDiagnostics CLASSNAME;
	SongDataDiagnostics();
	
	void Data() override;
	void DataDataset();
	void ToolMenu(Bar& bar) override;
	
	
};


#endif
