#ifndef _SongTool_AnalysisCtrl_h_
#define _SongTool_AnalysisCtrl_h_


class AnalysisCtrl : public Ctrl {
	MArr<ArrayCtrl> data;
	MArr<DocEdit> edit, full;
	Splitter hsplit;
	MArr<Splitter> vsplit;
	
public:
	typedef AnalysisCtrl CLASSNAME;
	AnalysisCtrl();
	
	void Data();
	void DataCursor(const SnapArg& match);
	
	
};

#endif
