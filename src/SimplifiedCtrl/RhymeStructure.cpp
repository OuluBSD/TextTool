#include "SimplifiedCtrl.h"

RhymeSequenceDrawer::RhymeSequenceDrawer() {
	bg = GrayColor();
	
}

void RhymeSequenceDrawer::Paint(Draw& d) {
	Size sz = GetSize();
	
	d.DrawRect(sz, bg);
	
}

void RhymeSequenceDrawer::DrawLine(Draw& d, double pos, double gridh, int note_i) {
	
}



RhymeStructureCtrl::RhymeStructureCtrl() {
	Add(drawer.SizePos());
	
}

void RhymeStructureCtrl::Data() {
	
}

void RhymeStructureCtrl::ToolMenu(Bar& bar) {
	SongToolCtrl::ToolMenu(bar);
}

