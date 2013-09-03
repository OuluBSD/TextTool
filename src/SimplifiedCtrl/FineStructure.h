#ifndef _SimplifiedCtrl_FineStructure_h_
#define _SimplifiedCtrl_FineStructure_h_


class PartRhymeDrawer : public Ctrl {
	int off = 5;
	int grid_lw = 5;
	int bars_per_row = 4;
	Color bg, line, rhyme_begin, beat_begin;
	
public:
	PartRhymeDrawer();
	void Paint(Draw& d) override;
	void DrawLine(Draw& d, double pos, double gridh, int note_i);
	
};

class FineStructureCtrl : public LineListCtrl {
	Splitter hsplit;
	ArrayCtrl rhymes, notes;
	PartRhymeDrawer drawer;
	
public:
	typedef FineStructureCtrl CLASSNAME;
	FineStructureCtrl();
	
	void DisableAll();
	void EnableAll();
	void Data() override;
	void DataPart();
	void DataRhyme();
	void ToolMenu(Bar& bar) override;
	
};


#endif
