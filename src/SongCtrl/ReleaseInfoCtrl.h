#ifndef _SongCtrl_ReleaseInfoCtrl_h_
#define _SongCtrl_ReleaseInfoCtrl_h_


class Editor;

class ReleaseInfoCtrl : public WithProductInfo<ToolAppCtrl> {
	
	
public:
	typedef ReleaseInfoCtrl CLASSNAME;
	ReleaseInfoCtrl();
	
	void Data();
	void Clear();
	void OnValueChange();
	
	Editor* editor = 0;
	
};


#endif
