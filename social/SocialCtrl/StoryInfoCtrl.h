#ifndef _SocialCtrl_LyricsInfoCtrl_h_
#define _SocialCtrl_LyricsInfoCtrl_h_


class Editor;

class LyricsInfoCtrl : public WithLyricsInfo<ToolAppCtrl> {
	
	
public:
	typedef LyricsInfoCtrl CLASSNAME;
	LyricsInfoCtrl();
	
	void Data();
	void Clear();
	void OnValueChange();
	
	Editor* editor = 0;
	
};


#endif
