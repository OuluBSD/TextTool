#ifndef _SongCtrl_SongInfoCtrl_h_
#define _SongCtrl_SongInfoCtrl_h_


BEGIN_SONGLIB_NAMESPACE


class SongEditor;

class SongInfoCtrl : public WithSongInfo<ToolAppCtrl> {
	int focus_tc = -1, focus_arch = -1, focus_lyr = -1;
	
public:
	typedef SongInfoCtrl CLASSNAME;
	SongInfoCtrl();
	
	void Data();
	void Clear();
	void DataTypecast();
	void DataArchetype();
	void DataLyrics();
	void OnValueChange();
	void SetLyrics();
	
	SongEditor* editor = 0;
	
};


END_SONGLIB_NAMESPACE


#endif
