#ifndef _SongTool_BasicInfoCtrl_h_
#define _SongTool_BasicInfoCtrl_h_

class Editor;

class BasicInfoCtrl : public WithInfo<SongToolCtrl> {
	
	
public:
	typedef BasicInfoCtrl CLASSNAME;
	BasicInfoCtrl();
	
	void Data();
	void Clear();
	void OnValueChange();
	
	Editor* editor = 0;
	
};


#endif
