#include "Sequencer.h"


LineEditCtrl::LineEditCtrl() {
	
	
	AddFrame(sb.Vert());
	
	
}

void LineEditCtrl::Data() {
	
	
	
}

void LineEditCtrl::Layout() {
	sb.SetPage(GetSize().cy);
}

void LineEditCtrl::MouseWheel(Point, int zdelta, dword c) {
	sb.Wheel(zdelta);
}

bool LineEditCtrl::Key(dword key, int) {
	return sb.VertKey(key);
}
