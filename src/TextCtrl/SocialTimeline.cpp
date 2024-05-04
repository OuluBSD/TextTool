#include "TextCtrl.h"


BEGIN_TEXTLIB_NAMESPACE


SocialTimelineCtrl::SocialTimelineCtrl() {
	
}

void SocialTimelineCtrl::Paint(Draw& d) {
	Size sz = GetSize();
	d.DrawRect(sz, White());
	
	int x_2 = sz.cx / 2;
	d.DrawLine(x_2, 0, x_2, sz.cy, 1, Black());
}


END_TEXTLIB_NAMESPACE
