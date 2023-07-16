#ifndef _SongTool_LineEditCtrl_h_
#define _SongTool_LineEditCtrl_h_


struct SequencerParams {
	int line_height = 30;
	int timeline_height = 30;
	int seq_y = 0;
	int seq_x = 0;
	
	
	
	static SequencerParams& Single() {static SequencerParams p; return p;}
};

class TimelineCtrl : public Ctrl {
	
	
protected:
	friend class Sequencer;
	friend class LineEditCtrl;
	friend class SeqLineListCtrl;
	
public:
	typedef TimelineCtrl CLASSNAME;
	TimelineCtrl();
	
	void Data();
	void Paint(Draw& d) override;
	
};

class LineEditCtrl : public Ctrl {
	
	
protected:
	friend class Sequencer;
	ScrollBar			sb;
	TimelineCtrl		tl;
	
public:
	typedef LineEditCtrl CLASSNAME;
	LineEditCtrl();
	
	void Data();
	void Layout() override;
	void MouseWheel(Point, int zdelta, dword) override;
	bool Key(dword key, int) override;
	
};


#endif
