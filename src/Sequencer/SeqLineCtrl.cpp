#include "Sequencer.h"


SeqLineCtrl::SeqLineCtrl() {
	
	
	
	
}

void SeqLineCtrl::Data() {
	
	
	
	
}

void SeqLineCtrl::Paint(Draw& d) {
	Size sz = GetSize();
	
	d.DrawRect(sz, White());
	
	
	
}









SeqLineListCtrl::SeqLineListCtrl() {
	SequencerParams& p = SequencerParams::Single();
	Add(tl.HSizePos().TopPos(0,p.timeline_height));
	
	
}

void SeqLineListCtrl::Data() {
	
	
	
}

void SeqLineListCtrl::Layout() {
	SequencerParams& p = SequencerParams::Single();
	Size sz = GetSize();
	int y = p.timeline_height;
	for (SeqLineCtrl& c : list) {
		Rect r = RectC(0, y, sz.cx, p.line_height);
		c.SetRect(r);
		y += p.line_height;
	}
	
}
