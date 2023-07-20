#ifndef _SongTool_AnalysisCtrl_h_
#define _SongTool_AnalysisCtrl_h_


class AnalysisCtrl : public Ctrl {
	ArrayCtrl data[GENDER_COUNT];
	DocEdit edit[GENDER_COUNT], full[GENDER_COUNT];
	Splitter hsplit, vsplit[GENDER_COUNT];
	
public:
	typedef AnalysisCtrl CLASSNAME;
	AnalysisCtrl();
	
	void Data();
	void DataCursor(int match);
	
	
};

#endif
