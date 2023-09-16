#ifndef _SongTool_Pitching_h_
#define _SongTool_Pitching_h_


class Pitching : public SongToolCtrl {
	Splitter hsplit, vsplit0, vsplit1;
	DocEdit native, english, feedback, pitching;
	Button translate, make_feedback, make_pitch;
	Ctrl top_bar;
	
public:
	typedef Pitching CLASSNAME;
	Pitching();
	
	void Data() override;
	void DoMainAction(int i=0) override;
	void ImportReferenceStruct();
	void CheckErrors();
	void ConvertToNative();
	
};


#endif
