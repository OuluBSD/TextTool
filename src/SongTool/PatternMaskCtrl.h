#ifndef _SongTool_PatternMaskCtrl_h_
#define _SongTool_PatternMaskCtrl_h_


class PatternMaskCtrl : public SongToolCtrl {
	MArr<ArrayCtrl> data;
	MArr<DocEdit> lyrics;
	MArr<Splitter> vsplit;
	Splitter hsplit;
	
public:
	typedef PatternMaskCtrl CLASSNAME;
	PatternMaskCtrl();
	
	void Data();
	void SelectLine(const SnapArg& match);
	
	
};

#endif
