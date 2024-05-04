#ifndef _TextCtrl_SocialTimeline_h_
#define _TextCtrl_SocialTimeline_h_


BEGIN_TEXTLIB_NAMESPACE


class LeadsCtrl;

class SocialTimelineCtrl : public Ctrl {
	
public:
	typedef SocialTimelineCtrl CLASSNAME;
	SocialTimelineCtrl();
	
	void Paint(Draw& d);
	
};


END_TEXTLIB_NAMESPACE


#endif
