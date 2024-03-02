#ifndef _EditorCtrl_SongInfoCtrl_h_
#define _EditorCtrl_SongInfoCtrl_h_


class Editor;

class SongInfoCtrl : public WithSongInfo<SongToolCtrl> {
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
	
	Editor* editor = 0;
	
};


#endif
