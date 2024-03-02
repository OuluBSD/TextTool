#ifndef _EditorCtrl_LyricsInfoCtrl_h_
#define _EditorCtrl_LyricsInfoCtrl_h_


class Editor;

class LyricsInfoCtrl : public WithLyricsInfo<SongToolCtrl> {
	
	
public:
	typedef LyricsInfoCtrl CLASSNAME;
	LyricsInfoCtrl();
	
	void Data();
	void Clear();
	void OnValueChange();
	
	Editor* editor = 0;
	
};


#endif
