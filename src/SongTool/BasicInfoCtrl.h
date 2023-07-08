#ifndef _SongTool_BasicInfoCtrl_h_
#define _SongTool_BasicInfoCtrl_h_

class Editor;

class BasicInfoCtrl : public WithInfo<Ctrl> {
	
	
public:
	typedef BasicInfoCtrl CLASSNAME;
	BasicInfoCtrl();
	
	void Data();
	void OnValueChange();
	
	Editor* editor = 0;
	
};


#endif
