#ifndef _SongTool_AnalysisCtrl_h_
#define _SongTool_AnalysisCtrl_h_


class AnalysisCtrl : public Ctrl {
	ArrayCtrl data[2];
	DocEdit edit[2], full[2];
	Splitter hsplit, vsplit[2];
	
public:
	typedef AnalysisCtrl CLASSNAME;
	AnalysisCtrl();
	
	void Data();
	void DataCursor();
	
	
};

#endif
