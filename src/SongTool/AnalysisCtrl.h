#ifndef _SongTool_AnalysisCtrl_h_
#define _SongTool_AnalysisCtrl_h_


class AnalysisCtrl : public Ctrl {
	PArr<ArrayCtrl> data;
	PArr<DocEdit> edit, full;
	Splitter hsplit;
	PArr<Splitter> vsplit;
	
public:
	typedef AnalysisCtrl CLASSNAME;
	AnalysisCtrl();
	
	void Data();
	void DataCursor(const SnapArg& match);
	
	
};

#endif
