#ifndef _SimplifiedCtrl_RhymeStructure_h_
#define _SimplifiedCtrl_RhymeStructure_h_


class RhymeSequenceDrawer : public Ctrl {
	Color bg;
	
public:
	RhymeSequenceDrawer();
	void Paint(Draw& d) override;
	void DrawLine(Draw& d, double pos, double gridh, int note_i);
	
};

class RhymeStructureCtrl : public SongToolCtrl {
	RhymeSequenceDrawer drawer;
	
public:
	typedef RhymeStructureCtrl CLASSNAME;
	RhymeStructureCtrl();
	
	void Data() override;
	void ToolMenu(Bar& bar) override;
	
};

#endif
