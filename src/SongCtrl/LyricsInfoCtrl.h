#ifndef _SongCtrl_LyricsInfoCtrl_h_
#define _SongCtrl_LyricsInfoCtrl_h_


BEGIN_SONGLIB_NAMESPACE


class SongEditor;

class LyricsInfoCtrl : public WithLyricsInfo<ToolAppCtrl> {
	
	
public:
	typedef LyricsInfoCtrl CLASSNAME;
	LyricsInfoCtrl();
	
	void Data();
	void Clear();
	void OnValueChange();
	
	SongEditor* editor = 0;
	
};


END_SONGLIB_NAMESPACE


#endif
