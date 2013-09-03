#include "Sequencer.h"


SeqLineHeaderCtrl::SeqLineHeaderCtrl() {
	
	
	
	
}

void SeqLineHeaderCtrl::Data() {
	
	
	
	
}

void SeqLineHeaderCtrl::Paint(Draw& d) {
	Size sz = GetSize();
	
	d.DrawRect(sz, White());
	
	d.DrawText(5,5, track->name);
}








SeqLineHeaderListCtrl::SeqLineHeaderListCtrl() {
	
	
	
}

void SeqLineHeaderListCtrl::Data() {
	
	
	
}

void SeqLineHeaderListCtrl::Layout() {
	SequencerParams& p = SequencerParams::Single();
	Size sz = GetSize();
	int y = p.timeline_height;
	for (SeqLineHeaderCtrl& c : list) {
		Rect r = RectC(0, y, sz.cx, p.line_height);
		c.SetRect(r);
		y += p.line_height;
	}
	
}
