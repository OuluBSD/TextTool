#ifndef _SongTool_PatternMaskCtrl_h_
#define _SongTool_PatternMaskCtrl_h_


class PatternMaskCtrl : public Ctrl {
	ArrayCtrl data[GENDER_COUNT];
	DocEdit lyrics[GENDER_COUNT];
	Splitter hsplit, vsplit[GENDER_COUNT];
	
public:
	typedef PatternMaskCtrl CLASSNAME;
	PatternMaskCtrl();
	
	void Data();
	void SelectLine(int match);
	
	
};

#endif
