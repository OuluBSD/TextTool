#ifndef _TextCtrl_SnapInfoCtrl_h_
#define _TextCtrl_SnapInfoCtrl_h_


BEGIN_TEXTLIB_NAMESPACE


class ToolEditor;

class SnapInfoCtrl : public WithProductInfo<ToolAppCtrl> {
	
	
public:
	typedef SnapInfoCtrl CLASSNAME;
	SnapInfoCtrl();
	
	void Data();
	void Clear();
	void OnValueChange();
	
	ToolEditor* editor = 0;
	
};


END_TEXTLIB_NAMESPACE


#endif
